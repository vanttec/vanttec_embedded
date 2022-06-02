/*
 * MS5837.h
 *
 *  Created on: May 3, 2022
 *      Author: Andres Estrada
 */

#ifndef INC_BAR30_MS5837_H_
#define INC_BAR30_MS5837_H_

#include "stm32f4xx_hal.h"

uint8_t MS5837_init(I2C_HandleTypeDef *i2c_channel);
void MS5837_read(I2C_HandleTypeDef *i2c_channel);

void read_bar30(void *);

float depth();
float altitude();
float temperture();
float pressure();

#endif /* INC_BAR30_MS5837_H_ */
