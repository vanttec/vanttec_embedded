#include "Sub/sub_main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

/* Definitions for uavcan task */
osThreadId_t canTaskHandle;
const osThreadAttr_t canTask_attributes = {
  .name = "mainTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};


void can_task(void*){
	while(1){
		osDelay(pdMS_TO_TICKS(5));
	}
}

void createTasks_sub(){
	canTaskHandle = osThreadNew(can_task, NULL, &canTask_attributes);
}
