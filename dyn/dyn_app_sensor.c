/*
 * dyn_sensor.c
 *
 *  Created on: 18 mar. 2020
 *
 * TODO: High-level functions like "distance_wall_front", etc
 * TODO: Generate another file for motors, with functions like "move_forward", etc
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

/**
 * Funci�n que fuerza la escritura del valor de los sensores
 * (S�lo es para esta pr�ctica, para comprobar que la lectura
 * de los sensores es correcta, luego esto no se podr� hacer)
 */
void force_write(uint8_t left, uint8_t center, uint8_t right){
    dyn_write_byte(sen_id, 0x1A, left);
    dyn_write_byte(sen_id, 0x1B, center);
    dyn_write_byte(sen_id, 0x1C, right);
}



