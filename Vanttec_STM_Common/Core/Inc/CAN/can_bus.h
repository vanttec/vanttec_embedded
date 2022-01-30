/*
 * can_bus.h
 *
 *  Created on: Jan 7, 2022
 *      Author: abiel
 */

#ifndef SRC_CAN_CAN_BUS_H_
#define SRC_CAN_CAN_BUS_H_

#include <stdint.h>

#define HEARTBEAT_ID 0x01
//16 SBUS channels
#define SBUS_ID_START 0x02
#define SBUS_ID_END 0x12
#define DEBUG_LOG_ID 0x13
//8 motor channels
#define MOTOR_ID_START 0x14
#define MOTOR_ID_END 0x1C
//Jetson heartbeat
#define JETSON_HEARTBEAT_ID 0x1D

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t motorSetpoints[8];
	uint32_t jetsonHBTick;
} can_rx;

typedef struct {
	uint32_t heartbeat;
	uint16_t sbusData[16];
} can_tx;

extern can_rx can_rx_data;
extern can_tx can_tx_data;

typedef struct {
	uint8_t buf[8];
	uint8_t msg_size;
} CAN_TX_QUEUE_OBJ;

#ifdef __cplusplus
}
#endif

#endif /* SRC_CAN_CAN_BUS_H_ */
