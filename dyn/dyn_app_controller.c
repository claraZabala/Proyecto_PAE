//
// Created by  on 08/06/2020.
//

#include <stdlib.h>
#include "dyn_app_controller.h"
#include "dyn_app_motor.h"
#include "dyn_app_sensor.h"

#define SAFETY_INTERVAL_MIN 2
#define SAFETY_INTERVAL_MAX 10
#define EPSILON 15

int get_min(){
    if (center_ir < left_ir){
        if(center_ir < right_ir){
            return 0;
        } else{
            return 1;
        }
    } else{
        if (left_ir < right_ir){
            return -1;
        } else{
            return 1;
        }
    }
}

int is_bot_near_wall(){
    return (is_near_wall(left_ir) | is_near_wall(right_ir)) | is_near_wall(center_ir);
}

int is_near_wall(uint8_t ir_value){
    return (ir_value <= (uint8_t) SAFETY_INTERVAL_MAX);
}

int is_bot_safe() {
    return is_left_safe() && is_center_safe() && is_right_safe();
}

int is_safe(uint8_t ir_value){
    return ((uint8_t) SAFETY_INTERVAL_MIN < ir_value);
}

int is_left_safe(){
    return (is_safe(left_ir));
}

int is_center_safe(){
    return (is_safe(center_ir));
}

int is_right_safe(){
    return (is_safe(right_ir));
}

void update_ir_values(){
    left_ir = read_left_ir();
    center_ir = read_center_ir();
    right_ir = read_right_ir();

}

int get_max_side() {
    return (left_ir <= right_ir);
}

void autonomous_movement(){
    update_ir_values();
    int min = get_min();

    if (!is_bot_near_wall()){
        move_forward();
        move_fast_forward();
        /*
        if (min == 0){
            move_forward();
        } else if (min == 1){
            turn_right();
        } else if (min == -1) {
            turn_left();
        }
         */
    }
    else if (is_bot_safe()){
        move_forward();
        if (!min){
            if (get_max_side()){
                turn_right();
            } else {
                turn_left();
            }
        } else {
            move_forward();
        }
    }
    else {
        stop();
        if (!is_center_safe()){
            if (get_max_side()){
                turn_right();
            } else {
                turn_left();
            }
        }
        else if (is_left_safe()){
            turn_left();
        }
        else if (is_right_safe()){
            turn_right();
        }
        else {
            move_forward();
        }
    }
}

int is_target_ahead(){
    int diff;
    if( center_ir > target_ir){
        diff = (int) center_ir - target_ir;
    } else {
        diff = (int) target_ir - center_ir;
    }

    return diff < EPSILON;
}

void init_controller(){
    target_set = 0;
    lesgo = 0;
}

void autonomous_movement_v2(){
    update_ir_values();

    if (!is_bot_near_wall()){
        if (!target_set) {
            if(lesgo){
                if (is_near_wall(center_ir)){
                    lesgo = 0;
                    target_set = 0;
                } else {
                    move_forward();
                }

            } else {
                int min = get_min();
                if (min == 0) {
                    lesgo = 0;
                } else if (min == 1) {
                    target_ir = right_ir;
                    target_set = 1;
                } else if (min == -1) {
                    target_ir = left_ir;
                    target_set = -1;
                }
            }
        } else {
            if(is_target_ahead()){
                lesgo = 1;
                target_set = 0;
                stop();
            } else {
                if(target_set == -1){
                    spin_left();
                } else {
                    spin_right();
                }
            }
        }
    } else {

        if (!target_set) {
            if(lesgo) {
                if (is_near_wall(center_ir)){
                    lesgo = 0;
                    target_set = 0;
                    stop();
                } else {
                    move_forward();
                }
            } else {
                if (get_max_side()) {
                    target_ir = right_ir;
                    target_set = 1;
                } else {
                    target_ir = left_ir;
                    target_set = -1;
                }
            }
        } else {
            if(is_target_ahead()){
                lesgo = 1;
                target_set = 0;
                stop();
            } else {
                if(target_set == -1){
                    spin_left();
                } else {
                    spin_right();
                }
            }
        }
    }

    if(!is_bot_safe()){
        stop();
        lesgo = 0;
        target_set = 0;
        if(is_safe(center_ir)){
            if(get_max_side()){
                turn_right();
            } else {
                turn_left();
            }
        }

    }

}