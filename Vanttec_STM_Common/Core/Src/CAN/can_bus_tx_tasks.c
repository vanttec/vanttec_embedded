/*
 * can_bus_tx_tasks.c
 *
 *  Created on: Jan 15, 2022
 *      Author: abiel
 */

#include "CAN/can_bus_tx_tasks.h"
#include "cmsis_os.h"
#include "SBUS/sbus.h"
#include "CAN/can.h"

static const osThreadAttr_t sbusTask_attributes = {
  .name = "sbusTxCan",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void start_can_tx_tasks(){
	osThreadNew(can_sbus_tx_task, NULL, &sbusTask_attributes);
}

extern SBUS_Data sbusData;
void can_sbus_tx_task(void * param){
	for(;;){
		for(uint8_t i = 0; i < 16; i++){
			queue_can_msg_short(SBUS_ID_START + i, sbusData.channels[i]);
		}

		osDelay(SBUS_DELAY);
	}
}
