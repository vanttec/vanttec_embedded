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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t motorSetpoints[8];
} can_rx_data;

typedef struct {
	uint32_t heartbeat;
} can_tx_data;

extern can_rx_data rx_data;
extern can_tx_data tx_data;

#ifdef __cplusplus
}
#endif

#endif /* SRC_CAN_CAN_BUS_H_ */
