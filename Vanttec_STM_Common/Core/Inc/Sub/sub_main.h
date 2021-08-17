#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "canard.h"
#include "bxcan.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

void uavcan_init(uint8_t nodeId, uint32_t bitrate);

void createTasks_sub();

#ifdef __cplusplus
} // extern "C"
#endif
