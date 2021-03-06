/*
 * File: SERVO_cfg.c
 * Driver Name: [[ SERVO Motor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "Servo/Servo_cfg.h"

const SERVO_CfgType SERVO_CfgParam[SERVO_NUM] =
{
	// Servo Motor 1 Configurations
	{
	    GPIOA,
		GPIO_PIN_0,
		TIM2,
		&TIM2->CCR1,
		TIM_CHANNEL_1,
		72000000,
		0.65,
		2.3
	},
	// Servo Motor 2 Configurations
	{
	    GPIOA,
		GPIO_PIN_3,
		TIM2,
		&TIM2->CCR4,
		TIM_CHANNEL_4,
		72000000,
		0.75,
		2.5
	},

};
