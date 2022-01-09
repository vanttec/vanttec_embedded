/*
 * can_bus_task.c
 *
 *  Created on: Jan 9, 2022
 *      Author: abiel
 */

#include "CAN/can_bus_task.h"
#include "CAN/can_bus.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "CANMessage.h"

extern CAN_HandleTypeDef hcan2;

void can_tx_update(){
	tx_data.heartbeat = HAL_GetTick();
}

void can_tx_task(void * params){
	CAN_TxHeaderTypeDef txHeader;
	uint8_t buffer[8];
	uint32_t txMailbox;

	txHeader.IDE = CAN_ID_STD;
	txHeader.StdId = 0x111;
	txHeader.RTR = CAN_RTR_DATA;

	HAL_CAN_Start(&hcan2);

	for(;;){
		can_tx_update();

		txHeader.DLC = can_pack_long(HEARTBEAT_ID, tx_data.heartbeat, buffer, 8);
		HAL_StatusTypeDef ret = HAL_CAN_AddTxMessage(&hcan2, &txHeader, buffer, &txMailbox);
			//Error_Handler();

		osDelay(can_tx_task_delay);
	}
}
