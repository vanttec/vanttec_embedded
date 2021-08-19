#include "Sub/sub_main.h"
#include "canard.h"
#include "bxcan.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "o1heap.h"
#define NUNAVUT_ASSERT assert
#include "uavcan/node/Heartbeat_1_0.h"

#define BXCAN_INTERFACE 1 //CAN2

static CanardInstance canard;
static O1HeapInstance* heap;

static CanardMicrosecond getMicroseconds(){
	return HAL_GetTick() * 1000;
}

static void* canardAllocate(CanardInstance* const ins, const size_t amount){
	O1HeapInstance* const heap = (O1HeapInstance*) ins->user_reference;
	assert(o1heapDoInvariantsHold(heap));
	return o1heapAllocate(heap, amount);
}

static void canardFree(CanardInstance* const ins, void* const pointer){
	O1HeapInstance* const heap = (O1HeapInstance*) ins->user_reference;
	o1heapFree(heap, pointer);
}

/* Definitions for uavcan task */
osThreadId_t uavcanTaskHandle;
const osThreadAttr_t uavcanTask_attributes = {
  .name = "mainTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};


void uavcan_init(uint8_t nodeId, uint32_t bitrate){
	BxCANTimings timings;
	assert(bxCANComputeTimings(HAL_RCC_GetPCLK1Freq(), bitrate, &timings));

	//Configure CAN on CAN2
	bxCANConfigure(0, timings, true);
	assert(bxCANConfigure(BXCAN_INTERFACE, timings, false));

	alignas(O1HEAP_ALIGNMENT) static uint8_t heap_arena[1024 * 16] = {0};
	heap = o1heapInit(heap_arena, sizeof(heap_arena), vPortEnterCritical, vPortExitCritical);
	assert(heap != NULL);

	canard = canardInit(&canardAllocate, &canardFree);
	canard.user_reference = heap;
	canard.node_id = nodeId;
	canard.mtu_bytes = CANARD_MTU_CAN_CLASSIC;
}

void canardProcessTx(){
	const CanardFrame* frame = canardTxPeek(&canard);
	CanardMicrosecond us = getMicroseconds();

	while(frame != NULL){
		//TODO only transmit if has not timedout waiting in queue
		bool ret = bxCANPush(BXCAN_INTERFACE, us, frame->timestamp_usec, frame->extended_can_id, frame->payload_size, frame->payload);
		if(!ret){
			//Transmission did not work!
			//Do something just for debugging
			us = getMicroseconds();
		}

		canardTxPop(&canard);
		canard.memory_free(&canard, (void*) frame);

		frame = canardTxPeek(&canard);
	}
}

void canardProcessRx(){
	CanardFrame frame = {0};
	uint8_t buffer[CANARD_MTU_CAN_CLASSIC] = {0};
	frame.payload = &buffer;
	for(uint16_t i = 0; i < 1; ++i){
		bool res = bxCANPop(BXCAN_INTERFACE, &(frame.extended_can_id), &(frame.payload_size), buffer);
		if(!res) break;

		frame.timestamp_usec = getMicroseconds();

		CanardTransfer transfer = {0};
		CanardRxSubscription *subs = NULL;

		int8_t canardRes = canardRxAccept2(&canard, &frame, 0, &transfer, &subs);
		if(canardRes > 0){
			assert(subs != NULL);
			//TODO handle rx
			canard.memory_free(&canard, (void*) transfer.payload);
		}
	}
}

void canardSendHeartbeat(){
	static CanardTransferID heartbeatId = 0;
	uavcan_node_Heartbeat_1_0 heartbeat{0};
	heartbeat.uptime = HAL_GetTick();
	heartbeat.mode.value = uavcan_node_Mode_1_0_OPERATIONAL;

	uint8_t buf[uavcan_node_Heartbeat_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_] = {0};
	size_t serSize = sizeof(buf);
	auto ret = uavcan_node_Heartbeat_1_0_serialize_(&heartbeat, &buf[0], &serSize);
	assert(ret >= 0);
	const CanardTransfer transfer = {
		.timestamp_usec = getMicroseconds() + 10000000,
		.priority = CanardPriorityNominal,
		.transfer_kind = CanardTransferKindMessage,
		.port_id = uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_,
		.remote_node_id = CANARD_NODE_ID_UNSET,
		.transfer_id = heartbeatId++,
		.payload_size = serSize,
		.payload = &buf[0]
	};

	canardTxPush(&canard, &transfer);
}

void testuavcan_task(void*){
	while(1){
		canardSendHeartbeat();
		canardProcessRx();
		canardProcessTx();
		canardProcessTx();
		osDelay(pdMS_TO_TICKS(5));
	}
}

void createTasks_sub(){
	//uavcan_init(8, 1000000U);
	//uavcan_init(8, 100000u);
	uavcan_init(8, 100000U);
	uavcanTaskHandle = osThreadNew(testuavcan_task, NULL, &uavcanTask_attributes);
}
