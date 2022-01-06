/*
 * sbus.c
 *
 *  Created on: Jan 6, 2022
 *      Author: abiel
 */

#include "SBUS/sbus.h"
#include <string.h>

static int min(int a, int b){
	return (a > b) ? b : a;
}

HAL_StatusTypeDef SBUS_Init(SBUS_Data *data, UART_HandleTypeDef *huart){
	if(data == NULL || huart == NULL) return HAL_ERROR;

	//set buffers to 0 (mostly for ease of debugging)
	memset(data->rxBuffer, 0, sizeof(data->rxBuffer));
	memset(data->channels, 0, sizeof(data->channels));
	memset(data->msgBuffer, 0, sizeof(data->msgBuffer));
	data->timestamp = 0;
	data->msgLen = 0;

	data->huart = huart;

	return HAL_UART_Receive_DMA(huart, data->rxBuffer, SBUS_MSG_LEN);
}


void SBUS_RxCallback(SBUS_Data *data, UART_HandleTypeDef *huart){
	if(data == NULL || data->huart != huart) return;

	//Check if header is needed to be searched
	int i = 0;
	if(data->msgLen == 0){
		//Find header in rx
		for(; i < SBUS_MSG_LEN; i++){
			if(data->rxBuffer[i] == SBUS_HEADER){
				//Begin message
				data->msgBuffer[0] = SBUS_HEADER;
				data->msgLen = 1;
				break;
			}
		}
	}

	//Copy remaining data
	int copyLen = min(SBUS_MSG_LEN - data->msgLen, SBUS_MSG_LEN - i);
	memcpy(data->msgBuffer + 1, data->rxBuffer + i + 1, copyLen);
	data->msgLen += copyLen;

	//Check if message was able to be completed
	if(data->msgLen == SBUS_MSG_LEN){
		//Message is complete! parse
		SBUS_Parse(data);
		data->msgLen = 0; //Reset message
	}

	HAL_UART_Receive_DMA(huart, data->rxBuffer, SBUS_MSG_LEN);
}

static int16_t ch1;

void SBUS_Parse(SBUS_Data *data){
	if(data->msgBuffer[0] != SBUS_HEADER && data->msgBuffer[SBUS_MSG_LEN - 1] != SBUS_FOOTER){
		//Data invalid!
		return;
	}

	uint16_t *ch_ = data->channels;
	uint8_t *buf_ = data->msgBuffer;

	//Parse message
	ch_[0]  = buf_[1]       | buf_[2]  << 8 & 0x07FF;
	ch_[1]  = buf_[2]  >> 3 | buf_[3]  << 5 & 0x07FF;
	ch_[2]  = buf_[3]  >> 6 | buf_[4]  << 2  |
								   buf_[5] << 10 & 0x07FF;
	ch_[3]  = buf_[5]  >> 1 | buf_[6]  << 7 & 0x07FF;
	ch_[4]  = buf_[6]  >> 4 | buf_[7]  << 4 & 0x07FF;
	ch_[5]  = buf_[7]  >> 7 | buf_[8]  << 1  |
								   buf_[9] << 9 & 0x07FF;
	ch_[6]  = buf_[9]  >> 2 | buf_[10] << 6 & 0x07FF;
	ch_[7]  = buf_[10] >> 5 | buf_[11] << 3 & 0x07FF;
	ch_[8]  = buf_[12]      | buf_[13] << 8 & 0x07FF;
	ch_[9]  = buf_[13] >> 3 | buf_[14] << 5 & 0x07FF;
	ch_[10] = buf_[14] >> 6 | buf_[15] << 2  |
								   buf_[16] << 10 & 0x07FF;
	ch_[11] = buf_[16] >> 1 | buf_[17] << 7 & 0x07FF;
	ch_[12] = buf_[17] >> 4 | buf_[18] << 4 & 0x07FF;
	ch_[13] = buf_[18] >> 7 | buf_[19] << 1  |
								   buf_[20] << 9 & 0x07FF;
	ch_[14] = buf_[20] >> 2 | buf_[21] << 6 & 0x07FF;
	ch_[15] = buf_[21] >> 5 | buf_[22] << 3 & 0x07FF;

	data->timestamp = xTaskGetTickCountFromISR();
}
