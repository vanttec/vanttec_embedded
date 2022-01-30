#include <stdbool.h>
#include "stm32f4xx_hal.h"

enum BoatState {
	BoatState_Autonomous,
	BoatState_Teleoperated,
	BoatState_Disabled
};

//Function only runs once, all tasks should be added here
void createTasks_boat();

/**
 * Tasks
 */
void mainTask_boat(void *params);

void boat_autonomous_loop();
void boat_teleoperated_loop();
void boat_disabled_loop();
