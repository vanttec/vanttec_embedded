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

//0-7 channels, -1 to 1 range output for 50Hz servos (ch 0-3)
void pwm_set(uint8_t ch, float output);

void pwm_set_raw(uint8_t ch, uint16_t output);

uint16_t pwm_get_raw(uint8_t ch);

#include "pca9685.h"

extern pca9685_handle_t g_pcaHandle;

// Ball-shooter H-bridge control (-1.0 to 1.0) on logical PWM3 and PWM5.
void pwm_set_ibt2_motor(float speed);

// Enable pin control for Ball Thrower Motor (IBT_2 R_EN & L_EN) and Water Pump
void pwm_set_ball_enable(uint8_t enable);
void pwm_set_pump_enable(uint8_t enable);

// PCA9685PW Servo Header driver (ch 0..15, setpoint -1.0 to 1.0)
void pwm_set_pca_servo(uint8_t ch, float setpoint);

#endif /* INC_PWM_PWM_OUT_H_ */
