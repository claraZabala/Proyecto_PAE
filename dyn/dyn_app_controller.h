//
// Created by ClaraUni on 08/06/2020.
//

#ifndef P4_PC_JOYSTICK2_HABITACION_DYN_APP_CONTROLLER_H
#define P4_PC_JOYSTICK2_HABITACION_DYN_APP_CONTROLLER_H
#include <stdio.h>

uint8_t left_ir, center_ir, right_ir, target_ir;

void autonomous_movement();

void update_ir_values();

int is_bot_near_wall();

int is_near_wall(uint8_t ir_value);

int is_safe(uint8_t ir_value);

int is_left_safe();

int is_center_safe();

int is_right_safe();

#endif //P4_PC_JOYSTICK2_HABITACION_DYN_APP_CONTROLLER_H