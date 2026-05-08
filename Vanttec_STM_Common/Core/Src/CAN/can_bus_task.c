/*
 * can_bus_task.c
 *
 *  Created on: Jan 9, 2022
 *      Author: abiel
 */

#include <string.h>
#include "CAN/can_bus.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "../../../libs/vanttec_CANLib/src/Vanttec_CANLib/CANMessage.h"
#include "CAN/can_bus_task.h"
#include "SBUS/sbus.h"
#include "CAN/can.h"
#include "CAN/can_bus_parser.h"

extern CAN_HandleTypeDef hcan2;
extern SBUS_Data sbusData;

osMessageQueueId_t debugMessageQueue;
osMessageQueueId_t txMessageQueue;

void can_init(){
	CAN_FilterTypeDef filter;
	filter.FilterBank = 20;
	filter.FilterMode = CAN_FILTERMODE_IDMASK;
	filter.FilterScale = CAN_FILTERSCALE_32BIT;
	filter.FilterFIFOAssignment = CAN_RX_FIFO0;
	filter.FilterActivation = CAN_FILTER_ENABLE;
	filter.FilterIdHigh = 0;
	filter.FilterIdLow = 0;
	filter.FilterMaskIdHigh = 0;
	filter.FilterMaskIdLow = 0;
	filter.SlaveStartFilterBank = 14;

	HAL_StatusTypeDef ret = HAL_CAN_ConfigFilter(&hcan2, &filter);
	if(ret != HAL_OK) Error_Handler();
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);

	//Initialize queues
	//First char -> size
	debugMessageQueue = osMessageQueueNew(64, 8, NULL);
	if(debugMessageQueue == NULL)
		Error_Handler();

	txMessageQueue = osMessageQueueNew(64, sizeof(CAN_TX_QUEUE_OBJ), NULL);
	if(txMessageQueue == NULL)
		Error_Handler();
}

void can_tx_update(){
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;
	txHeader.IDE = CAN_ID_STD;
	txHeader.StdId = 0x111;
	txHeader.RTR = CAN_RTR_DATA;

	CAN_TX_QUEUE_OBJ txOut;

	while(osMessageQueueGet(txMessageQueue, &txOut, NULL, 0) == osOK){
		txHeader.DLC = txOut.msg_size;
		HAL_StatusTypeDef ret = HAL_CAN_AddTxMessage(&hcan2, &txHeader, txOut.buf, &txMailbox);
		if(ret != HAL_OK){
			// Mailbox full, requeue message and stop trying for now
			osMessageQueuePut(txMessageQueue, &txOut, 0, 0);
			break;
		}
	}
}

void can_tx_task(void * params){
	for(;;){
		can_tx_update();

		osDelay(can_tx_task_delay);
	}
}


