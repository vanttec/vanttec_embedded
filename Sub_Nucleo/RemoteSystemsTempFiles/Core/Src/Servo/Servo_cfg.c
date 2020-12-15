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
		GPIO_PIN_1,
		TIM2,
		&TIM2->CCR2,
		TIM_CHANNEL_2,
		72000000,
		0.75,
		2.5
	},
	// Servo Motor 3 Configurations
	{
		GPIOA,
		GPIO_PIN_2,
		TIM2,
		&TIM2->CCR3,
		TIM_CHANNEL_3,
		72000000,
		0.65,
		2.3
	},
	// Servo Motor 4 Configurations
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
	// Servo Motor 5 Configurations
	{
		GPIOA,
		GPIO_PIN_6,
		TIM3,
		&TIM3->CCR1,
		TIM_CHANNEL_1,
		72000000,
		0.65,
		2.3
	},
	// Servo Motor 6 Configurations
	{
		GPIOB,
		GPIO_PIN_0,
		TIM3,
		&TIM3->CCR3,
		TIM_CHANNEL_1,
		72000000,
		0.75,
		2.5
	},

};
