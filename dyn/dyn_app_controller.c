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


int paret = 0;
/**
 * Funció per detectar quina és la paret més propera.
 * @return 0 si es tracta de la paret detectada pel sensor central,
 *         1 si es tracta de la paret detectada pel sensor dret,
 *        -1 si es tracta de la paret detectada pel sensor esquerra
 */
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

/**
 * Funció que ens diu si el robot està a prop d'alguna paret
 * @return 0 si algun dels sensors està a menys de la distància màxima
 *         1 altrament
 */
int is_bot_near_wall(){
    return (is_near_wall(left_ir) | is_near_wall(right_ir)) | is_near_wall(center_ir);
}

/**
 * Funció que ens diu si un valor de lectura d'un sensor és menor que la distància màxima
 * @param ir_value valor lectura sensor
 * @return 0 si està a prop de la paret
 *         1 altrament
 */
int is_near_wall(uint8_t ir_value){
    return (ir_value <= (uint8_t) SAFETY_INTERVAL_MAX);
}

/**
 * Funció que ens diu si el robot està en zona segura
 * @return 0 si el robot està a més distancia de la mínima de qualsevol dels valors que poden llegir els sensors
 *         1 si hi ha algun sensor a menys de la distància mínima recomenada
 */
int is_bot_safe() {
    return is_left_safe() && is_center_safe() && is_right_safe();
}

/**
 *
 * @param ir_value
 * @return
 */
int is_safe(uint8_t ir_value){
    return ((uint8_t) SAFETY_INTERVAL_MIN < ir_value);
}

/**
 * Funció que comprova si el valor de lectura del sensor esquerre es menor que la distància mínima
 * @return 0 si el valor del sensor és major que la distància mínima
 *         1 si el robot està en zona perillosa
 */
int is_left_safe(){
    return (is_safe(left_ir));
}

/**
 * Funció que comprova si el valor de lectura del sensor central es menor que la distància mínima
 * @return 0 si el valor del sensor és major que la distància mínima
 *         1 si el robot està en zona perillosa
 */
int is_center_safe(){
    return (is_safe(center_ir));
}

/**
 * Funció que comprova si el valor de lectura del sensor dret es menor que la distància mínima
 * @return 0 si el valor del sensor és major que la distància mínima
 *         1 si el robot està en zona perillosa
 */
int is_right_safe(){
    return (is_safe(right_ir));
}

/**
 * Funció que actualitza els valors de lectura dels tres sensors i els guarda a variables globals
 */
void update_ir_values(){
    left_ir = read_left_ir();
    center_ir = read_center_ir();
    right_ir = read_right_ir();
}

/**
 * Funció per al moviment autònom del robot
 */
void autonomous_movement() {
    //actualitzem els valors dels sensors i calculem quin és el sensor que detecta la paret més propera
    update_ir_values();
    int min = get_min();
    // min=0 -> CENTRE; min=1 -> DRETA; min=-1 -> ESQUERRA
    //inicialment la paret val 0 fins que detecti que algun des sensors està en la distància màxima de la paret
    //ESTAT INICIAL! només entrem aquí una vegada, quan comença el moviment i hem de trobar la paret més propera
    if (paret == 0) {
        //cridem is_bot_near_wall per veure si algun sensor es troba en la distància màxima
        //si cap es troba a aquesta distància ens apropem a la paret que indiqui min
        if (!is_bot_near_wall()) {
            if (min == 1) { turn_right(); }
            else if (min == -1) { turn_left(); }
            else { move_forward(); }
        }
            //quan alguna de les parets es troba a la distància necessària per a poder recórrer-la,
            //girarem a la dreta per a moure'ns sempre en sentit horari
        else {
            paret = 1;
            turn_right();
        }
    }
        //Cas en que ja estem a prop d'una paret i la volem resseguir en sentit horari
    else {
        //si ens allunyem massa de la paret (desviació o recorrer obstacle) girem a l'esquerra per seguir resseguint.
        if (!is_bot_near_wall()){ turn_left(); }
            //si ens trobem dins el rang de distància de la paret comrpovem si el robot està segur (no colisionarà)
            // i si ho està seguim avançant endavant
        else {
            //is_bot_safe comprova que els tres sensors es trobem a distància major que la mínima
            if (is_bot_safe()) {
                //en el cas de trobar-nos prop d'una paret pot passar:
                //que sigui la paret esquerra i per tant seguim recte
                if (!is_near_wall(center_ir)) { move_forward(); }
                    //que sigui la paret frontal i per tant girem a la dreta per mantenir el moviment en sentit horari
                else { pivot_right(); }
            }
            //aquí tractem el cas en que el robot es troba a una distància "perillosa" (massa aprop d'alguna paret)
            else {
                //cas més comú: es troba massa a prop de la paret frontal o esquerra (i la distància a la de la dreta
                // és segura), llavors girem a la dreta
                if (is_right_safe()) { pivot_right();}
                    //aquest cas no hauria de passar mai ja que el moviment del robot és sempre en sentit horari
                    // el controlem igualment per si un cas.
                else{
                    //Si està en un carrer sense sortida es mourà cap enrere
                    if (!is_left_safe() && !is_center_safe()){
                        move_backwards();
                    }
                    //Si la distància a la paret de la dreta és massa curta girem a l'esquerra.
                    else{
                        pivot_left();
                    }
                 }
            }
        }
    }
}

int get_max_side() {
    return (left_ir <= right_ir);
}

/*void autonomous_movement_v1(){
    update_ir_values();
    int min = get_min();

    if (!is_bot_near_wall()){
        move_forward();
        move_fast_forward();
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
}*/

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
    //pared = 0;
    //sentido = 0;
}


/*void autonomous_movement_v2(){
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

}*/

/*void autonomous_movement_v3() {
    update_ir_values();
    int min = get_min();
    if (pared == 0){
        if (!is_bot_near_wall()){
            if (sentido == 0){
                if (min == 1){
                    turn_right();
                    sentido = -1;
                } else if (min == -1){
                    turn_left();
                    sentido = 1;
                } else {
                    move_forward();
                }
            }
            else {
                if (sentido == 1){
                    if ((center_ir==(uint8_t)255 && right_ir==(uint8_t)255 && left_ir==(uint8_t)255) | min == 0){
                        move_forward();
                    }
                    else {
                        turn_left();
                    }
                }
                else {
                    if ((center_ir==(uint8_t)255 && right_ir==(uint8_t)255 && left_ir==(uint8_t)255) | min == 0){
                        move_forward();
                    }
                    else {
                        turn_right();
                    }
                }
            }
        } else {
            pared = 1;
            if (!is_near_wall(center_ir)) {
                move_forward();
            }
            else {
                if (sentido == 1) {
                    pivot_right();
                } else if (sentido == -1) {
                    pivot_left();
                }
            }
        }
    }
    else {
        if (!is_bot_near_wall()){
            pared = 0;
            if (min == 1){
                turn_right();
            } else if (min == -1) {
                turn_left();
            }
        }
        else {
            if (is_bot_safe()) {
                if (!is_near_wall(center_ir)) {
                    move_forward();
                }
                else {
                    if (sentido == 1) {
                        pivot_right();
                    } else if (sentido == -1) {
                        pivot_left();
                    }
                }
            }
            else {
                //PELIGRO
                if (is_safe(read_left_ir()) && !is_safe(read_right_ir())){
                    if (is_near_wall(read_center_ir())){
                        pivot_left();
                    }
                    else {
                        turn_left();
                    }
                } else {
                    if (is_near_wall(read_center_ir())){
                        pivot_right();
                    }
                    else {
                        turn_right();
                    }
                }
            }
        }
    }
}*/