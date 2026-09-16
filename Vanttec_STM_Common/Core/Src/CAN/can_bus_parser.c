/*
 * can_bus_parser.c
 */

#include "CAN/can_bus.h"
#include "CAN/can_bus_parser.h"
#include "telemetry.h"

// Note: g_sendPing removed because PING_ID was deprecated in the new standard HAL architecture.

void can_parse_msg(CAN_RxHeaderTypeDef *header, uint8_t *data){
	if(data == NULL) return;

	if (header->StdId == CAN_ID_JETSON_HB) {
		can_rx_data.jetsonHBTick = HAL_GetTick();
	}
	else if (header->StdId == CAN_ID_JETSON_MOTORS_01 && header->DLC >= 8) {
		float *f = (float*)data;
		can_rx_data.motorSetpoints[0] = f[0];
		can_rx_data.motorSetpoints[1] = f[1];
	}
	else if (header->StdId == CAN_ID_JETSON_MOTORS_23 && header->DLC >= 8) {
		float *f = (float*)data;
		can_rx_data.motorSetpoints[2] = f[0];
		can_rx_data.motorSetpoints[3] = f[1];
	}
	else if (header->StdId == CAN_ID_JETSON_MOTORS_45 && header->DLC >= 8) {
		float *f = (float*)data;
		can_rx_data.motorSetpoints[4] = f[0];
		can_rx_data.motorSetpoints[5] = f[1];
	}
	else if (header->StdId == CAN_ID_JETSON_MOTORS_67 && header->DLC >= 8) {
		float *f = (float*)data;
		can_rx_data.motorSetpoints[6] = f[0];
		can_rx_data.motorSetpoints[7] = f[1];
	}
	else if (header->StdId == CAN_ID_BATTERY && header->DLC >= 4) {
		float *f = (float*)data;
		status.battery_voltage = f[0];
		status.battery_pcb_last_seen = HAL_GetTick();
	}
	else if (header->StdId == CAN_ID_ACTUATOR && header->DLC >= 1) {
		status.actuator_active = data[0];
		status.actuator_pcb_last_seen = HAL_GetTick();
	}
	else if (header->StdId == CAN_ID_GPS_LATLON && header->DLC >= 8) {
		float *f = (float*)data;
		status.latitude = f[0];
		status.longitude = f[1];
		status.nav_pcb_last_seen = HAL_GetTick();
	}
	else if (header->StdId == CAN_ID_GPS_HEADING && header->DLC >= 4) {
		float *f = (float*)data;
		status.heading = f[0];
		status.nav_pcb_last_seen = HAL_GetTick();
	}
}
