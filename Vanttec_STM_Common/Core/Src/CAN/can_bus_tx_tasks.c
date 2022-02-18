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
#include "Sensors/battery_monitor.h"

volatile uint8_t g_sendPing;

static const osThreadAttr_t sbusTask_attributes = {
  .name = "sbusTxCan",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

static const osThreadAttr_t hbTask_attributes = {
  .name = "hbTxCan",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

static const osThreadAttr_t debugMsgTask_attributes = {
  .name = "debugMsgTx",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

static const osThreadAttr_t pingTask_attributes = {
  .name = "pingMsgTx",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

static const osThreadAttr_t batteryTask_attributes = {
  .name = "batteryMonitorTx",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};


static void can_hb_tx_task(void *param){
	for(;;){
		queue_can_msg_short(HEARTBEAT_ID, HAL_GetTick());
		osDelay(HEARTBEAT_DELAY);
	}
}


extern SBUS_Data sbusData;
static void can_sbus_tx_task(void * param){
	for(;;){
		for(uint8_t i = 0; i < 16; i++){
			queue_can_msg_short(SBUS_ID_START + i, sbusData.channels[i]);
		}

		queue_can_msg_byte(SBUS_FAILSAFE, sbusData.failsafe);

		osDelay(SBUS_DELAY);
	}
}

static void can_debug_msg_tx_task(void *param){
	for(;;){
		while(handle_debug_msg_queue() == osOK);
		osDelay(DEBUG_MSG_DELAY);
	}
}

static void can_send_ping_task(void * param){
	for(;;){
		if(g_sendPing == 1){
			queue_can_msg_byte(PING_ID, 0);
			g_sendPing = 0;
		}

		osDelay(1);
	}
}

static void can_battery_monitor_task(void * param){
	battery_monitor_init();
	for(;;){
		float voltage = battery_monitor_poll();
		queue_can_msg_float(BATTERY_VOLTAGE_ID, voltage);
		osDelay(1000);
	}
}

void start_can_tx_tasks(){
	osThreadNew(can_hb_tx_task, NULL, &hbTask_attributes);
	osThreadNew(can_sbus_tx_task, NULL, &sbusTask_attributes);
	osThreadNew(can_debug_msg_tx_task, NULL, &debugMsgTask_attributes);
	osThreadNew(can_send_ping_task, NULL, &pingTask_attributes);
	osThreadNew(can_battery_monitor_task, NULL, &batteryTask_attributes);
}
