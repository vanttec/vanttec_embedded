#include "Servo/Servo.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>

//X8R Receiver arduino pins
const int PIN_X8R_4 = 10;
const int PIN_X8R_2 = 9;
const int PIN_X8R_5 = 11;

//Add LED pins
const int red_light_pin = 14;
const int green_light_pin = 13;
const int blue_light_pin = 12;

void autonomous_Mode(uint16_t thrusterRight, uint16_t thrusterLeft, int powerR, int powerL);

float map(float x, float in_min, float in_max, float out_min, float out_max);

void read_values (float channel4, float channel2, float channel5);

void RGB_color(bool red_light_value, bool green_light_value, bool blue_light_value);

void power_Difference(float channel4, float channel2, float channel5,
                      uint16_t thrusterRight, uint16_t thrusterLeft);

void select(float channel4, float channel2, float channel5,
		uint16_t thrusterRight, uint16_t thrusterLeft,
            int powerR, int powerL);
