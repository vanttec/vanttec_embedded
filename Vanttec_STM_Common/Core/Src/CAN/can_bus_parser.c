/*
 * can_bus_parser.c
 *
 *  Created on: Jan 29, 2022
 *      Author: abiel
 */

#include "CAN/can_bus.h"
#include "CAN/can_bus_parser.h"
#include "CANMessage.h"

void can_parse_msg(CAN_RxHeaderTypeDef *header, uint8_t *data){
	if(data == NULL) return;

	uint8_t id = can_parse_id(data, header->DLC);
	if(id >= MOTOR_ID_START && id <= MOTOR_ID_END){
		//Motor msg
		uint16_t data = can_parse_short(data, header->DLC);
		can_rx_data.motorSetpoints[id - MOTOR_ID_START] = data;
	} else if(id == JETSON_HEARTBEAT_ID) {
		//Jetson heartbeat
		can_rx_data.jetsonHBTick = HAL_GetTick();
	}
}
