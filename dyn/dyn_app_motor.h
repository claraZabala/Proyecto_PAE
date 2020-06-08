//
// Created by banco on 05/05/2020.
//

#include <stdint.h>

#ifndef JOYSTICK_DYN_APP_MOTOR_H
#define JOYSTICK_DYN_APP_MOTOR_H

/**
 * Función que imprime por pantalla la velocidad de las dos ruedas del motor
 */
void spd_indicator();

/**
 * Getter de la velocidad del motor izquierdo.
 * Lee los valores de los registros 20 y 21 del motor izquierdo.
 * @return velocidad actual del motor izquierdo
 */
uint16_t get_left_speed();

/**
 * Getter de la velocidad del motor derecho.
 * Lee los valores de los registros 20 y 21 del motor derecho.
 * @return velocidad actual del motor derecho
 */
uint16_t get_right_speed();

/**
 * Función general que cambia la velocidad para cada una de las ruedas.
 * @param lft_spd velocidad de la rueda izquierda (16 bits)
 * @param rgt_spd velocidad de la rueda derecha (16 bits)
 */
void set_speed(uint16_t lft_spd, uint16_t rgt_spd);

/**
 * Función para cambiar la velocidad de las dos ruedas, usando los argumentos de 8 bits H y L para cada rueda.
 * @param lft_spd_h  valor de mayor peso de la velocidad de la rueda izquierda
 * @param lft_spd_l valor de menor peso de la velocidad de la rueda izquierda
 * @param rgt_spd_h valor de mayor peso de la velocidad de la rueda derecha
 * @param rgt_spd_l valor de menor peso de la velocidad de la rueda derecha
 */
void set_speed_8(uint8_t lft_spd_h, uint8_t lft_spd_l, uint8_t rgt_spd_h, uint8_t rgt_spd_l);

/**
 * Movimiento rectilíneo. Establece la misma velocidad a las dos ruedas.
 * @param spd velocidad para las dos ruedas
 */
void straight_move(uint16_t spd);

/**
 * Avanzar hacia adelante a una velocidad fija.
 */
void move_forward();

/**
 * Retroceder a una velocidad fija.
 */
void move_backwards();

/**
 * Parar los dos motores. Establece la velocidad a 0.
 */
void stop();

/**
 * Giro a la izquierda a la vez que se avanza hacia adelante.
 */
void turn_left();

/**
 * Giro a la derecha a la vez que se avanza hacia adelante.
 */
void turn_right();

/**
 * Vueltas a la izquierda (velocidades opuestas).
 */
void spin_left();

/**
 * Vueltas a la derecha (velocidades opuestas).
 */
void spin_right();

/**
 * Pivotar sobre la rueda izquierda.
 */
void pivot_left();

/**
 * Pivotar sobre la rueda derecha.
 */
void pivot_right();

void pivot_90_right();
void pivot_90_left();

#endif //JOYSTICK_DYN_APP_MOTOR_H
