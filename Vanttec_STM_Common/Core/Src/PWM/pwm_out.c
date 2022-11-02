/*
 * pwm_out.c
 *
 *  Created on: Jan 25, 2022
 *      Author: abiel
 */

#include "PWM/pwm_out.h"
#include <math.h>

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
static uint8_t init = 0;

#define PWM_TIMER_LEN 2
static TIM_HandleTypeDef* timer_handles[] = {&htim3, &htim4};
static uint32_t timer_channels[] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};

static float pwm_map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static float pwm_clamp(float x, float min_val, float max_val){
	if(x < min_val) return min_val;
	if(x > max_val) return max_val;
	return x;
}

void pwm_init(){
	//Initialize all PWM duty cycle to 0
	if(init != 0) return;

	for(int i = 0; i < 8; i++)
		pwm_set(i, 0);

	//Start all PWM timers
	HAL_StatusTypeDef ret;
	for(int i = 0; i < PWM_TIMER_LEN; i++){
		for(int j = 0; j < 4; j++){
			ret = HAL_TIM_PWM_Start(timer_handles[i], timer_channels[j]);
		}
	}
}

void pwm_set_raw(uint8_t ch, uint16_t output){
	if(ch > 8) return;

	TIM_HandleTypeDef *timer;
	uint32_t timer_channel;
	if(ch < 4){
		timer = timer_handles[0];
		timer_channel = timer_channels[ch];
	} else {
		timer = timer_handles[1];
		timer_channel = timer_channels[ch - 4];
	}

	__HAL_TIM_SET_COMPARE(timer, timer_channel, output);
}

void pwm_set(uint8_t ch, float output){
	output = pwm_clamp(output, -1, 1);

	const uint16_t maxVal = __HAL_TIM_GET_AUTORELOAD(timer_handles[0]);
	output = pwm_map(output, -1, 1, 1100, 1900); //1ms - 2ms

	if(output < 0) output = 0;
	else if(output > maxVal) output = maxVal;

	pwm_set_raw(ch, (uint16_t) roundf(output));
}
