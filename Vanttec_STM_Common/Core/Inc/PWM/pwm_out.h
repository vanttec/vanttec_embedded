/*
 * pwm_out.h
 *
 *  Created on: Jan 25, 2022
 *      Author: abiel
 */

#ifndef INC_PWM_PWM_OUT_H_
#define INC_PWM_PWM_OUT_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

void pwm_init();

//0-7 channels, -1 to 1 range output
void pwm_set(uint8_t ch, float output);

void pwm_set_raw(uint8_t ch, uint16_t out);

#endif /* INC_PWM_PWM_OUT_H_ */
