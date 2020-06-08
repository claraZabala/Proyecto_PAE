//
// Created by banco on 06/05/2020.
//

#include <stdio.h>
#include "dyn_app_motor.h"
#include "dyn_instr.h"
#include "movement_simulator.h"
#include "math.h"


/*
 *
 */
uint8_t lft_id = 1; //id del motor izquierdo
uint8_t rgt_id = 2; //id del motor derecho

float speed_num_format(uint16_t spd) {
    int16_t v = spd & 0x300;
    if (spd & 0x400) {
        v *= -1;
    }
    float ratio = (v/1023.0) * 100;
    return ratio;
}

/**
 * Función que imprime por pantalla la velocidad de las dos ruedas del motor
 */
void spd_indicator(){
    uint16_t ls = get_left_speed();
    uint16_t rs = get_right_speed();
    float lft_spd = speed_num_format(ls);
    float rgt_spd = speed_num_format(rs);
    printf("Current Speed:\nL: %.2f%% / R: %.2f%%\n", lft_spd, rgt_spd);
}

/**
 * Getter de la velocidad del motor izquierdo.
 * Lee los valores de los registros 20 y 21 del motor izquierdo.
 * @return velocidad actual del motor izquierdo
 */
uint16_t get_left_speed(){
    uint8_t spd_l;
    uint8_t spd_h;
    uint16_t spd;

    dyn_read_byte(lft_id, 0x20, &spd_l);
    dyn_read_byte(lft_id, 0x21, &spd_h);

    spd = spd_h;

    return (spd << 8) + spd_l;
}

/**
 * Getter de la velocidad del motor derecho.
 * Lee los valores de los registros 20 y 21 del motor derecho.
 * @return velocidad actual del motor derecho
 */
uint16_t get_right_speed(){
    uint8_t spd_l;
    uint8_t spd_h;
    uint16_t spd;

    dyn_read_byte(rgt_id, 0x20, &spd_l);
    dyn_read_byte(rgt_id, 0x21, &spd_h);

    spd = spd_h;

    return (spd << 8) + spd_l;
}

/**
 * Función general que cambia la velocidad para cada una de las ruedas.
 * @param lft_spd velocidad de la rueda izquierda (16 bits)
 * @param rgt_spd velocidad de la rueda derecha (16 bits)
 */
void set_speed(uint16_t lft_spd, uint16_t rgt_spd){
    //en esta función dividimos los parámentros de velocidad de 2 bytes para adaptarlo a los argumentos de 1 byte de las
    // instrucciones high y low
    uint8_t lft_spd_l = (uint8_t) (lft_spd & 0xff);
    uint8_t lft_spd_h = (uint8_t) ((lft_spd >> 8) & 0xff);
    uint8_t rgt_spd_l = (uint8_t) (rgt_spd & 0xff);
    uint8_t rgt_spd_h = (uint8_t) ((rgt_spd >> 8) & 0xff);

    set_speed_8(lft_spd_h, lft_spd_l, rgt_spd_h, rgt_spd_l);
}

/**
 * Función para cambiar la velocidad de las dos ruedas, usando los argumentos de 8 bits H y L para cada rueda.
 * Envía un paquete de instrucción para los registros 20 y 21 con la velocidad como parámetro, para cada motor.
 * @param lft_spd_h  valor de mayor peso de la velocidad de la rueda izquierda
 * @param lft_spd_l valor de menor peso de la velocidad de la rueda izquierda
 * @param rgt_spd_h valor de mayor peso de la velocidad de la rueda derecha
 * @param rgt_spd_l valor de menor peso de la velocidad de la rueda derecha
 */
void set_speed_8(uint8_t lft_spd_h, uint8_t lft_spd_l, uint8_t rgt_spd_h, uint8_t rgt_spd_l){
    dyn_write_byte(lft_id, 0x20, lft_spd_l);
    dyn_write_byte(lft_id, 0x21, lft_spd_h);
    dyn_write_byte(rgt_id, 0x20, rgt_spd_l);
    dyn_write_byte(rgt_id, 0x21, rgt_spd_h);
}

/**
 * Movimiento rectilíneo. Establece la misma velocidad a las dos ruedas.
 * @param spd velocidad para las dos ruedas
 */
void straight_move(uint16_t spd){
    set_speed(spd, spd);
}

/**
 * Avanzar hacia adelante a una velocidad fija.
 */
void move_forward(){
    straight_move(0x300);
}

/**
 * Retroceder a una velocidad fija.
 */
void move_backwards(){
    straight_move(0x500);
}

/**
 * Parar los dos motores. Establece la velocidad a 0.
 */
void stop(){
    straight_move(0);
}

/**
 * Giro a la izquierda a la vez que se avanza hacia adelante.
 */
void turn_left(){
    set_speed(0x250, 0x300);
}

/**
 * Giro a la derecha a la vez que se avanza hacia adelante.
 */
void turn_right(){
    set_speed(0x300, 0x250);
}

/**
 * Vueltas a la izquierda (velocidades opuestas).
 */
void spin_left(){
    set_speed(0x500, 0x300);
}

/**
 * Vueltas a la derecha (velocidades opuestas).
 */
void spin_right(){
    set_speed(0x300, 0x500);
}

/**
 * Pivotar sobre la rueda izquierda.
 */
void pivot_left(){
    set_speed(0, 0x300);
}

/**
 * Pivotar sobre la rueda derecha.
 */
void pivot_right(){
    set_speed(0x300, 0);
}

void pivot_90_right(){
    float theta = get_theta();
    while (fabs(theta-get_theta())!=M_PI_2){
        pivot_right();
    }
}

void pivot_90_left(){
    float theta = get_theta();
    while (get_theta() < theta + M_PI_2){
        pivot_left();
    }
}