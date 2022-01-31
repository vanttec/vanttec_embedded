/*
 * can.c
 *
 *  Created on: Jan 15, 2022
 *      Author: abiel
 */

#include "CAN/can.h"
#include "stm32f4xx_hal.h"
#include <string.h>

extern CAN_HandleTypeDef hcan2;

osStatus_t handle_debug_msg_queue(){
	uint8_t buf[8];
	buf[0] = DEBUG_LOG_ID;
	uint32_t txMailbox;

	osStatus_t ret = osMessageQueueGet(debugMessageQueue, buf+1, NULL, 0);
	if(ret != osOK) return ret;

	CAN_TxHeaderTypeDef txHeader;
	txHeader.IDE = CAN_ID_STD;
	txHeader.StdId = 0x111;
	txHeader.RTR = CAN_RTR_DATA;

	//Find first 0 in data
	uint8_t i;
	for(i = 0; i < 8; i++) if(buf[i] == 0) break;

	txHeader.DLC = i;
	HAL_CAN_AddTxMessage(&hcan2, &txHeader, buf, &txMailbox);
	return ret;
}

void send_can_debug_msg(const char *str, int strlen){
	if(str == NULL || strlen == 0) return;

	int i;
	for(i = 0; i < strlen % 7; i += 7){
		osMessageQueuePut(debugMessageQueue, str + i, 1, 0);
	}

	//Put remaining bytes into queue
	uint8_t buf[7];
	memcpy(buf, str + i, strlen % 7);
	osMessageQueuePut(debugMessageQueue, buf, 1, 0);
}

void queue_can_msg_byte(uint8_t id, uint8_t data){
	CAN_TX_QUEUE_OBJ out;
	out.msg_size = can_pack_byte(id, data, out.buf, sizeof(out.buf));
	osMessageQueuePut(txMessageQueue, &out, 0, 10);
}

void queue_can_msg_long(uint8_t id, uint32_t data){
	CAN_TX_QUEUE_OBJ out;
	out.msg_size = can_pack_long(id, data, out.buf, sizeof(out.buf));
	osMessageQueuePut(txMessageQueue, &out, 0, 10);
}

void queue_can_msg_short(uint8_t id, uint16_t data){
	CAN_TX_QUEUE_OBJ out;
	out.msg_size = can_pack_short(id, data, out.buf, sizeof(out.buf));
	osMessageQueuePut(txMessageQueue, &out, 0, 10);
}

void queue_can_msg_float(uint8_t id, float data){
	CAN_TX_QUEUE_OBJ out;
	out.msg_size = can_pack_float(id, data, out.buf, sizeof(out.buf));
	osMessageQueuePut(txMessageQueue, &out, 0, 10);
}
