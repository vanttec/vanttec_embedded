#include "Sub/sub_main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

extern CAN_HandleTypeDef hcan2;

/* Definitions for uavcan task */
osThreadId_t canTaskHandleUpd;
const osThreadAttr_t canTask_attributes = {
  .name = "mainTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t Bar30Task_attributes = {
  .name = "bar30Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxData[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);
}


void can_task(void*){
	CAN_FilterTypeDef  		sFilterConfig;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = 0;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);

	HAL_CAN_Start(&hcan2);

	CAN_TxHeaderTypeDef txHeader;
	txHeader.IDE = CAN_ID_STD;
	txHeader.StdId = 0x123;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = 8; //only 1 byte will be sent
	txHeader.TransmitGlobalTime = DISABLE;

	uint8_t byte_to_send = 0;
	uint8_t txData[8] = {0};
	uint32_t txMailbox;

	//HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) == HAL_OK;

	while(1){
		for(int i = 0; i < 8; i++)
			txData[i] = byte_to_send++;
		HAL_CAN_AddTxMessage(&hcan2, &txHeader, txData, &txMailbox);
		osDelay(pdMS_TO_TICKS(1));
	}
}

void createTasks_sub(){
	canTaskHandleUpd = osThreadNew(can_task, NULL, &canTask_attributes);
	osThreadNew(can_bar30_task, NULL, &Bar30Task_attributes);
}

static void can_bar30_task(void * param){
	bar30_init();
	for(;;){
		float Depth = depth();
		queue_can_msg_float(BAR30_ID, Depth);
		osDelay(1000);
	}
}

