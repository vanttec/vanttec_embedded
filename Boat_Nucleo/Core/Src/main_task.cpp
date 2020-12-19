/*
 * main_task.cpp
 *
 *  Created on: Dec 18, 2020
 *      Author: abiel
 */

#include "main_task.h"
#include "cmsis_os.h"

#include "stm32g4xx_hal.h"

#include "ros.h"
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt8.h>

#include "STM32Hardware.h"

extern "C"{
#include "functions.h"
}

//autonomous navigation ints
static int powerR = 1500;
static int powerL = 1500;

extern TIM_HandleTypeDef htim2;

void right_cb( const std_msgs::UInt16& cmd_msg){
    powerR = cmd_msg.data; //1100-1900
}

void left_cb( const std_msgs::UInt16& cmd_msg){
    powerL = cmd_msg.data; //1100-1900
}

void main_task_update(void *arg){
	//X8R Receiver channels
	float channel4 = 0; //perpendicular axis lever - rotation
	float channel2 = 0; //parallel axis lever - displacement
	float channel5 = 0; //choice lever

	//X8R auxiliar variables
	float channel4Read1 = 0; //First captured value to measure pulse width
	float channel4Read2 = 0;//Second captured value to measure pulse width
	uint8_t channel4Status = 0;//Defines if waiting the first or second read

	float channel2Read1 = 0; //First captured value to measure pulse width
	float channel2Read2 = 0;//Second captured value to measure pulse width
	uint8_t channel2Status = 0;//Defines if waiting the first or second read

	float channel5Read1 = 0; //First captured value to measure pulse width
	float channel5Read2 = 0;//Second captured value to measure pulse width
	uint8_t channel5Status = 0;//Defines if waiting the first or second read

	//Thrusters declaration
	uint16_t thrusterRight = 0;
	uint16_t thrusterLeft = 1;

	ros::NodeHandle  nh;

	std_msgs::UInt8 flag;
	ros::Subscriber<std_msgs::UInt16> rsub("rpwm", right_cb);
	ros::Subscriber<std_msgs::UInt16> lsub("lpwm", left_cb);
	ros::Publisher stm32("stm32", &flag);

	nh.initNode();
	nh.advertise(stm32);
	nh.subscribe(rsub);
	nh.subscribe(lsub);
	uint16_t i = 0;

	/**
	 * Start PWM on left and right chan
	 */
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

	for(;;){
		select(channel4, channel2, channel5, thrusterRight, thrusterLeft, powerR, powerL);
		flag.data = 1;
		stm32.publish( &flag );
		nh.spinOnce();
	    osDelay(10);
	}
}
