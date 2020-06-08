//
// Created by ClaraUni on 08/06/2020.
//

#ifndef P4_PC_JOYSTICK2_HABITACION_DYN_APP_CONTROLLER_H
#define P4_PC_JOYSTICK2_HABITACION_DYN_APP_CONTROLLER_H
#include <stdio.h>

uint8_t left_ir, center_ir, right_ir;

void autonomous_movement();

void update_ir_values();

#endif //P4_PC_JOYSTICK2_HABITACION_DYN_APP_CONTROLLER_H