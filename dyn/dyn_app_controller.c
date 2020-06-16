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