#include "PWM/pwm_out.h"
#include "main.h"
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
	if(ch >= 8) return;

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

uint16_t pwm_get_raw(uint8_t ch){
	if(ch >= 8) return 0;
	TIM_HandleTypeDef *timer;
	uint32_t timer_channel;
	if(ch < 4){
		timer = timer_handles[0];
		timer_channel = timer_channels[ch];
	} else {
		timer = timer_handles[1];
		timer_channel = timer_channels[ch - 4];
	}
	return __HAL_TIM_GET_COMPARE(timer, timer_channel);
}

extern I2C_HandleTypeDef hi2c2;

pca9685_handle_t g_pcaHandle = {
	.i2c_handle = &hi2c2,
	.device_address = 0x43, // 7-bit address: 0x40 base + A0(1) + A1(2) pulled HIGH
	.inverted = false
};

void pwm_set_ibt2_motor(float speed){
	speed = pwm_clamp(speed, -1.0f, 1.0f);
	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim4); // PWM3 and PWM5 use the same 50 Hz period.

	if (speed > 0.0f) {
		uint16_t rpwm = (uint16_t)roundf(speed * arr);
		pwm_set_raw(3, rpwm); // Logical PWM3 -> RPWM
		pwm_set_raw(5, 0);    // Logical PWM5 -> LPWM
	} else if (speed < 0.0f) {
		uint16_t lpwm = (uint16_t)roundf(-speed * arr);
		pwm_set_raw(3, 0);    // Logical PWM3 -> RPWM
		pwm_set_raw(5, lpwm); // Logical PWM5 -> LPWM
	} else {
		pwm_set_raw(3, 0);
		pwm_set_raw(5, 0);
	}
}

void pwm_set_ball_enable(uint8_t enable){
	HAL_GPIO_WritePin(BALL_MOTOR_EN_GPIO_Port, BALL_MOTOR_EN_Pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void pwm_set_pump_enable(uint8_t enable){
	HAL_GPIO_WritePin(PUMP_EN_GPIO_Port, PUMP_EN_Pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void pwm_set_pca_servo(uint8_t ch, float setpoint){
	if (ch >= 16) return;
	setpoint = pwm_clamp(setpoint, -1.0f, 1.0f);
	// 50 Hz frame = 20ms = 4096 counts on PCA9685
	// 1.0ms pulse = 205 counts, 1.5ms pulse = 307 counts, 2.0ms pulse = 410 counts
	uint16_t off_time = (uint16_t)roundf(pwm_map(setpoint, -1.0f, 1.0f, 205.0f, 410.0f));
	pca9685_set_channel_pwm_times(&g_pcaHandle, ch, 0, off_time);
}
