/*
 * boat_main.c
 *
 *  Created on: Jan 29, 2022
 *      Author: abiel
 */

#include "Boat/boat_main.h"
#include "SBUS/sbus.h"
#include "PWM/pwm_out.h"
#include "CAN/can_bus.h"

extern UART_HandleTypeDef huart5;
static SBUS_Data sbus;

static const osThreadAttr_t boatTask_attributes = {
  .name = "boatTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};


void createTasks_boat() {
	HAL_StatusTypeDef ret = SBUS_Init(&sbus, &huart5);
	if(ret != HAL_OK) Error_Handler();

	osThreadNew(mainTask_boat, NULL, &boatTask_attributes);
}

/*
 * Handle mode switching based on sbus data
 */
void mainTask_boat(void * params) {
	enum BoatState state = BoatState_Disabled;
	for(;;){
		//Update current state
		if(sbus.channels[4] < 900) state = BoatState_Disabled;
		else if(sbus.channels[4] > 900 && sbus.channels[4] < 1110) state = BoatState_Teleoperated;
		else if(sbus.channels[4] > 1100) state = BoatState_Autonomous;

		//Update based on state
		switch(state){
		case BoatState_Autonomous:
			boat_autonomous_loop();
			break;
		case BoatState_Teleoperated:
			boat_teleoperated_loop();
			break;
		case BoatState_Disabled:
			boat_disabled_loop();
			break;
		}

		//TODO handle status lights
		osDelay(10);
	}
}

void boat_autonomous_loop(){
	//Take in motor setpoints from can, and write them out to pwm
	uint32_t dt = HAL_GetTick() - can_rx_data.jetsonHBTick;
	if(dt > 500) boat_disabled_loop(); //Disable if jetson has not sent any recent msgs

	for(int i = 0; i < 8; i++){
		pwm_set(i, can_rx_data.motorSetpoints[i]);
	}
}

void boat_teleoperated_loop(){
	//Take in motor control from SBUS, and write out to pwm
	float throttle = sbus.channels[1] / 1500.0;
	float steer = sbus.channels[3] / 1500.0;

	float leftMotor, rightMotor;
	leftMotor = throttle + steer;
	rightMotor = throttle - steer;

	pwm_set(0, leftMotor);
	pwm_set(1, rightMotor);
}

void boat_disabled_loop(){
	//Do nothing
	return;
}
