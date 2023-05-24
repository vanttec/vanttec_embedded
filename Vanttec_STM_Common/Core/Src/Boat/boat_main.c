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
#include "SEGGER_RTT.h"

#include <stdbool.h>

extern UART_HandleTypeDef huart5;
extern SBUS_Data sbusData;

static bool boatArmed = false;

static const osThreadAttr_t boatTask_attributes = {
  .name = "boatTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};


void createTasks_boat() {
	osThreadNew(mainTask_boat, NULL, &boatTask_attributes);
}

uint8_t op_mode_i = 0;

/*
 * Handle mode switching based on sbus data
 */
void mainTask_boat(void * params) {
	pwm_init();
	SBUS_Init(&sbusData, &huart5);
	enum BoatState state = BoatState_Disabled;
	for(;;){
		SBUS_Update();
		uint32_t sbus_dt = HAL_GetTick() - sbusData.timestamp;
		//Update current state
		if(sbusData.channels[4] < 900) state = BoatState_Disabled;
		else if(sbusData.channels[4] > 900 && sbusData.channels[4] < 1110) state = BoatState_Teleoperated;
		else if(sbusData.channels[4] > 1100) state = BoatState_Autonomous;

		if(sbus_dt > 1000) state = BoatState_Disabled; //Disable if sbus is not rcv

		//Arm if necessary
		if(!boatArmed && state != BoatState_Disabled){
			boat_arming_sequence();
		}
		uint8_t buffer[20];
		snprintf(buffer, 20, "%d, %d\r\n", sbusData.channels[0], sbusData.channels[4]);

		SEGGER_RTT_WriteString(0, buffer);

		if(op_mode_i > 10){
			queue_can_msg_short(OP_MODE_ID, state);
			op_mode_i = 0;
		}

		op_mode_i++;

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
	if(dt > 500) {
		boat_disabled_loop(); //Disable if jetson has not sent any recent msgs
		return;
	}

	for(int i = 0; i < 8; i++){
		pwm_set(i, can_rx_data.motorSetpoints[i]);
	}
}

void boat_teleoperated_loop(){
	//Take in motor control from SBUS, and write out to pwm
	float throttle = (sbusData.channels[1] - 1000) / 1500.0;
	float steer = (sbusData.channels[3] - 1000) / 1500.0;

	float leftMotor, rightMotor;
	leftMotor = -throttle + steer;
	rightMotor = -throttle - steer;

	//printf('leftMotor: %f\n', leftMotor);

	pwm_set(0, -leftMotor);
	pwm_set(1, rightMotor);
}

void boat_disabled_loop(){
	for(int i = 0; i < 8; i++)
		pwm_set(i, 0);
	return;
}

void boat_arming_sequence(){
	for(int i = 0; i < 8; i++)
			pwm_set_raw(i, 0);
	osDelay(1100);
	for(int i = 0; i < 8; i++)
		pwm_set_raw(i, 1500);
	osDelay(1100);
	boatArmed = true;
}
