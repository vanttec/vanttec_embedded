/*
 * mainTask.cpp
 *
 *  Created on: Oct 13, 2020
 *      Author: abiel
 */

#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt8.h>

extern "C"{
#include "mainTask.h"
#include "functions.h"
}

static uint16_t thrusterRight, thrusterLeft;
static uint16_t powerR, powerL;

void right_cb( const std_msgs::UInt16& cmd_msg){
    powerR = cmd_msg.data; //1100-1900
}

void left_cb( const std_msgs::UInt16& cmd_msg){
    powerL = cmd_msg.data; //1100-1900
}

void mainTask(void *params){
    powerR = 0;
    powerL = 0;
    thrusterRight = 0;
    thrusterLeft = 1;

    //X8R Receiver channels
    float channel4; //perpendicular axis lever - rotation
    float channel2; //parallel axis lever - displacement
    float channel5; //choice lever

    //TODO Implement read PWM (again)
    //X8R auxiliar variables
    float channel4Read1; //First captured value to measure pulse width
    float channel4Read2;//Second captured value to measure pulse width
    uint8_t channel4Status = 0;//Defines if waiting the first or second read

    float channel2Read1; //First captured value to measure pulse width
    float channel2Read2;//Second captured value to measure pulse width
    uint8_t channel2Status = 0;//Defines if waiting the first or second read

    float channel5Read1; //First captured value to measure pulse width
    float channel5Read2;//Second captured value to measure pulse width
    uint8_t channel5Status = 0;//Defines if waiting the first or second read

    ros::NodeHandle  nh;

    std_msgs::UInt8 flag;
    ros::Subscriber<std_msgs::UInt16> rsub("rpwm", right_cb);
    ros::Subscriber<std_msgs::UInt16> lsub("lpwm", left_cb);
    ros::Publisher stm32("stm32", &flag);

    for(;;){
        select_boat(channel4, channel2, channel5, thrusterRight, thrusterLeft, powerR, powerL);
        flag.data = 1;
        stm32.publish( &flag );
        nh.spinOnce();
        HAL_Delay(1);
    }
}
