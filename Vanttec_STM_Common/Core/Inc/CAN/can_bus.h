/*
 * can_bus.h
 */

#ifndef SRC_CAN_CAN_BUS_H_
#define SRC_CAN_CAN_BUS_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

// --- NEW STANDARD CAN IDs ---
// Transmitted by Main PCB (Acting as Telemetry Gateway)
#define CAN_ID_GATEWAY_HB       0x010 // Replaces old MAIN_STATUS
#define CAN_ID_MAIN_PWM_0_3     0x101
#define CAN_ID_MAIN_PWM_4_7     0x102
#define CAN_ID_MAIN_DEBUG       0x111
#define CAN_ID_PING             0x112

// Received from Jetson & Other PCBs
#define CAN_ID_JETSON_MOTORS_01 0x120
#define CAN_ID_JETSON_MOTORS_23 0x121
#define CAN_ID_JETSON_MOTORS_45 0x122
#define CAN_ID_JETSON_MOTORS_67 0x123
#define CAN_ID_JETSON_HB        0x01E

// Telemetry Data Received by Main PCB
#define CAN_ID_ACTUATOR         0x150
#define CAN_ID_BATTERY          0x200
#define CAN_ID_GPS_LATLON       0x300
#define CAN_ID_GPS_HEADING      0x301

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	float motorSetpoints[8];
	uint32_t jetsonHBTick;
} can_rx;

extern volatile can_rx can_rx_data;

typedef struct {
    CAN_TxHeaderTypeDef header;
	uint8_t payload[8];
} CAN_TX_QUEUE_OBJ;

#ifdef __cplusplus
}
#endif

#endif /* SRC_CAN_CAN_BUS_H_ */
