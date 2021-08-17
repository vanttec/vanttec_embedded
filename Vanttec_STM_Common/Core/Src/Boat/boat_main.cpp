#include "cmsis_os.h"
#include "Boat/boat_main.h"
#include "ros.h"
#include "Boat/functions.h"
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt8.h>

//X8R receiver channels
static X8RChanIn channels[X8R_CHANNELS];

//ROS objects
ros::NodeHandle *nh;
std_msgs::UInt8 flag;

int leftPower = 1500;
int rightPower = 1500;

ros::Subscriber<std_msgs::UInt16> lsub("lpwm", [](const std_msgs::UInt16& msg){leftPower = msg.data;});
ros::Subscriber<std_msgs::UInt16> rsub("rpwm", [](const std_msgs::UInt16& msg){rightPower = msg.data;});
ros::Publisher stm32("stm32", &flag);

/* Definitions for main task */
osThreadId_t mainTaskHandle;
const osThreadAttr_t mainTask_attributes = {
  .name = "mainTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};

void handleTIMCaptureCallback(TIM_HandleTypeDef *htim){
	for(unsigned int i = 0; i < sizeof(channels); i++){
		auto & chan(channels[i]);
		//If the desired channel is being read
		if(chan.htim == htim && chan.htim->Channel == chan.active_chan){
			if(!chan.waitingForSecond){
				//First measurement, measure rising edge
				chan.firstCap = HAL_TIM_ReadCapturedValue(chan.htim, chan.chan);
				chan.waitingForSecond = true;

				//Switch to measuring falling edge
				__HAL_TIM_SET_CAPTUREPOLARITY(chan.htim, chan.chan, TIM_INPUTCHANNELPOLARITY_FALLING);
			} else {
				//Second measurement, measure falling edge
				chan.secondCap = HAL_TIM_ReadCapturedValue(chan.htim, chan.chan);
				chan.waitingForSecond = true;

				//Calculate value
				if(chan.secondCap > chan.firstCap){
					chan.val = chan.secondCap - chan.firstCap;
				}

				//Reset counter and set to measure rising edge
				__HAL_TIM_SET_COUNTER(chan.htim, 0);
				__HAL_TIM_SET_CAPTUREPOLARITY(chan.htim, chan.chan, TIM_INPUTCHANNELPOLARITY_RISING);
			}
		}
	}
}

void mainTask(void *){
	while(1){
		//TODO set correct channel mappings
		uint32_t channel4, channel2, channel5;
		channel4 = channels[0].val;
		channel2 = channels[1].val;
		channel5 = channels[2].val;

		//TODO assign correct servo channels
		select(channel4, channel2, channel5, 0, 1, rightPower, leftPower);

		flag.data = 1;
		stm32.publish(&flag);

		//TODO setup serial peripheral for rosserial
		nh->spinOnce();
		osDelay(pdMS_TO_TICKS(5));
	}
}

void createTasks(){
	//TODO Assign TIM Capture Callback
	//Otherwise, X8R PWM will not be read
	memset(channels, 0, sizeof(channels));

	nh = new ros::NodeHandle();

	mainTaskHandle = osThreadNew(mainTask, NULL, &mainTask_attributes);
}
