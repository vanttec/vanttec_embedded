/*
 * can_bus_tx_tasks.h
 *
 *  Created on: Jan 15, 2022
 *      Author: abiel
 */

#ifndef INC_CAN_CAN_BUS_TX_TASKS_H_
#define INC_CAN_CAN_BUS_TX_TASKS_H_

//Delay between data updates
#define HEARTBEAT_DELAY 100
#define SBUS_DELAY 10

void start_can_tx_tasks();

void can_sbus_tx_task(void * param);

#endif /* INC_CAN_CAN_BUS_TX_TASKS_H_ */
