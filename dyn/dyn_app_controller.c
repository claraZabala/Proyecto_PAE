//
// Created by  on 08/06/2020.
//

#include <stdlib.h>
#include "dyn_app_controller.h"
#include "dyn_app_motor.h"
#include "dyn_app_sensor.h"

#define SAFETY_INTERVAL_MIN 2
#define SAFETY_INTERVAL_MAX 10

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

    /*
    interval_status = 0;

    interval_status |= right_ir < left_ir ? 0 : 128;
    interval_status |= center_ir < left_ir || center_ir < right_ir ? 0 : 64;

    interval_status |= left_ir > SAFETY_INTERVAL_MIN ? 0 : 32; // bit 5 a 1 si esta entre 0 y 2
    interval_status |= center_ir > SAFETY_INTERVAL_MIN ? 0 : 16; // bit 4 a 1 si esta entre 0 y 2
    interval_status |= right_ir > SAFETY_INTERVAL_MIN ? 0 : 8; // bit 3 a 1 si esta entre 0 y 2

    interval_status |= left_ir > SAFETY_INTERVAL_MAX ? 0 : 4; // bit 2 a 1 si esta entre 0 y 10
    interval_status |= center_ir > SAFETY_INTERVAL_MAX ? 0 : 2; // bit 1 a 1 si esta entre 0 y 10
    interval_status |= right_ir > SAFETY_INTERVAL_MAX ? 0 : 1; // bit 0 a 1 si esta entre 0 y 10
     */
}

int get_max_side() {
    return (left_ir < right_ir);
}

void autonomous_movement(){
    update_ir_values();
    int min = get_min();
    /*if (!is_bot_near_wall()) {
        if (right_ir < left_ir) {
            while(read_center_ir()>right_ir){
                pivot_right();
            }
            while (!is_near_wall(read_center_ir()) && !is_bot_near_wall()){
                move_forward();
            }
            uint8_t center = read_center_ir();
            while (read_right_ir()>center){
                pivot_left();
            }
            move_forward();
        } else {
            while(read_center_ir()>left_ir){
                pivot_left();
            }
            while (!is_near_wall(read_center_ir()) && !is_bot_near_wall()){
                move_forward();
            }
            uint8_t center = read_center_ir();
            while (read_left_ir()>center){
                pivot_right();
            }
            move_forward();
        }
    }
    else {
        if (is_near_wall(read_center_ir())){
            stop();
        }
        while (!is_near_wall(read_center_ir())){
            move_forward();
        }
        while (!is_safe(read_center_ir())) {
            pivot_right();
        }
        move_forward();
    }*/
    if (!is_bot_near_wall()){
        if (min == 0){
            move_forward();
        } else if (min == 1){
            turn_right();
        } else if (min == -1) {
            turn_left();
        }
    }
    else if (is_bot_safe()){
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