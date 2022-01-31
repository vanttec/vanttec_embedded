/*
 * can.h
 *
 *  Created on: Jan 15, 2022
 *      Author: abiel
 */

#ifndef INC_CAN_CAN_H_
#define INC_CAN_CAN_H_

#include "can_bus.h"
#include <stdint.h>
#include "cmsis_os.h"

extern osMessageQueueId_t debugMessageQueue;
extern osMessageQueueId_t txMessageQueue;

osStatus_t handle_debug_msg_queue();

void send_can_debug_msg(const char *str, int strlen);

void queue_can_msg_byte(uint8_t id, uint8_t data);

void queue_can_msg_long(uint8_t id, uint32_t data);

void queue_can_msg_short(uint8_t id, uint16_t data);

void queue_can_msg_float(uint8_t id, float data);

#endif /* INC_CAN_CAN_H_ */
