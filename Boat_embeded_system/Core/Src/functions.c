#include "functions.h"

void autonomous_Mode(uint16_t thrusterRight, uint16_t thrusterLeft, int powerR, int powerL) {
    SERVO_RawMove(thrusterRight,powerR);
    SERVO_RawMove(thrusterLeft,powerL);
}

float map(float x, float in_min, float in_max, float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void read_values (float channel4, float channel2, float channel5){
    /*
TODO Agregar libreria para implementar pulseIn
//Read channel frequecies
channel4 = pulseIn(PIN_X8R_4, HIGH);
//Serial.print("CH 4 ");
//Serial.println(channel4);
channel2 = pulseIn(PIN_X8R_2, HIGH);
//Serial.print("CH 2 ");
//Serial.println(channel2);
channel5 = pulseIn(PIN_X8R_5, HIGH);
//Serial.print("CH 5 ");
//Serial.println(channel5);*/
}

void RGB_color(bool red_light_value, bool green_light_value, bool blue_light_value){
    HAL_GPIO_WritePin(GPIOA, red_light_pin, red_light_value ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, green_light_pin, green_light_value ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, blue_light_pin, blue_light_value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void power_Difference(float channel4, float channel2, float channel5,
                      uint16_t thrusterRight, uint16_t thrusterLeft){
    //void for controlled movement
    float R;
    float L;

    float Tx;
    float Tz;
    float Tport;
    float Tstbd;

    if ((channel4 > 1450 && channel4 < 1550) && (channel2 > 1450 && channel2 < 1550)){     //Control stable
        R=1500;
        L=1500;
        SERVO_RawMove(thrusterRight,R);
        SERVO_RawMove(thrusterLeft,L);      //thrusters at zero
    }
    else if ((channel4 < 1450 || channel4 > 1550) || (channel2 < 1450 || channel2 > 1550)) {    //Control for moving
        Tx = map(channel2, 975, 2025, -70, 70);
        Tz = map(channel4, 975, 2025, -5, 5);

        Tstbd = (Tx / 2) - (Tz / 0.41);
        if (Tstbd > 35){
            Tstbd = 35;
        }
        else if (Tstbd < -35){
            Tstbd = -35;
        }

        Tport = (Tx / (2 * 1.27)) + (Tz / (0.41 * 1.27));
        if (Tport > 27){
            Tport = 27;
        }
        else if (Tstbd < -27){
            Tstbd = -27;
        }

        R = ((Tstbd / 35 * 400)+1500);
        L = ((Tport / 35 * 400)+1500);
        SERVO_RawMove(thrusterRight,R);
        SERVO_RawMove(thrusterLeft,L);
    }
}

void select(float channel4, float channel2, float channel5,
            uint16_t thrusterRight, uint16_t thrusterLeft,
            int powerR, int powerL) {
    //Use channel 5 to select current mode
    if (channel5 < 1300) {
        power_Difference(channel4, channel2, channel5, thrusterRight, thrusterLeft);
        RGB_color(true, true, false); // Yellow
    }
    else if ( channel5 > 1600) {
        autonomous_Mode(thrusterRight,thrusterLeft,powerR,powerL);
        RGB_color(false, false, true); // Blue
    }
    else {
        SERVO_RawMove(thrusterRight,powerR);
        SERVO_RawMove(thrusterLeft,powerL);
        RGB_color(true, false, false); // Red
    }
}
