/*
 * dyn_sensor.c
 *
 *  Created on: 18 mar. 2020
 *
 */
#include "dyn_app_sensor.h"
#include "dyn_instr.h"

uint8_t sen_id = 3; //id del sensor

/**
 * Funci�n que devuelve el bit de lectura del sensor izquierdo
 * Para ello lee del registro 1A del sensor
 */
uint8_t read_left_ir(){
    uint8_t read;
    dyn_read_byte(sen_id, 0x1A, &read);
    return read;
}

/**
 * Funci�n que devuelve el bit de lectura del sensor central
 * Para ello lee del registro 1B del sensor
 */
uint8_t read_center_ir(){
    uint8_t read;
    dyn_read_byte(sen_id, 0x1B, &read);
    return read;
}

/**
 * Funci�n que devuelve el bit de lectura del sensor derecho
 * Para ello lee del registro 1C del sensor
 */
uint8_t read_right_ir(){
    uint8_t read;
    dyn_read_byte(sen_id, 0x1C, &read);
    return read;
}



