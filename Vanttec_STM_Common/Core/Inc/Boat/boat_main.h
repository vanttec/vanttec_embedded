#include "Boat/boat_config.h"
#include <stdbool.h>
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
	uint32_t val;
	uint32_t firstCap, secondCap; //Used to compare values to measure pulse width
	bool waitingForSecond;

	TIM_HandleTypeDef *htim;

	//Should be the same channel
	//ex: HAL_TIM_ACTIVE_CHANNEL_2 and TIM_CHANNEL_2
	HAL_TIM_ActiveChannel active_chan;
	uint16_t chan;
} X8RChanIn;

//Used to handle pulse width measurement for X8R
void handleTIMCaptureCallback(TIM_HandleTypeDef *htim);

//Function only runs once, all tasks should be added here
void createTasks();

/**
 * Tasks
 */
void mainTask();

#ifdef __cplusplus
} // extern "C"
#endif
