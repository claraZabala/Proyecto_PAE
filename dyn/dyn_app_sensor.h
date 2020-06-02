#ifndef DYN_SENSOR_H_
#define DYN_SENSOR_H_

#include <stdint.h>

// TODO comments

uint8_t read_left_ir();

uint8_t read_center_ir();

uint8_t read_right_ir();

/**
 * Funci�n test..
 * @param left
 * @param center
 * @param right
 */
void force_write(uint8_t left, uint8_t center, uint8_t right);

#endif /* DYN_SENSOR_H_ */
