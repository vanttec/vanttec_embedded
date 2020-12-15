/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "usb_host.h"
#include <stdint.h>
#include "stm32g4xx_hal.h"
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt8.h>
#include "ros.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
extern "C"{
#include "Servo/Servo.h"
#include "functions.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

}


void thrustersControl(void *argument);
void pumpControlTask(void *argument);
void pressureMeasure(void *argument);
void hydrophoneTask(void *argument);

//X8R Receiver channels
float channel4; //perpendicular axis lever - rotation
float channel2; //parallel axis lever - displacement
float channel5; //choice lever

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

//Thrusters declaration
uint16_t thrusterRight = 0;
uint16_t thrusterLeft = 1;

//autonomous navigation ints
int powerR = 1500;
int powerL = 1500;

ros::NodeHandle  nh;

void right_cb( const std_msgs::UInt16& cmd_msg){
    powerR = cmd_msg.data; //1100-1900
}

void left_cb( const std_msgs::UInt16& cmd_msg){
    powerL = cmd_msg.data; //1100-1900
}

std_msgs::UInt8 flag;
ros::Subscriber<std_msgs::UInt16> rsub("rpwm", right_cb);
ros::Subscriber<std_msgs::UInt16> lsub("lpwm", left_cb);
ros::Publisher stm32("stm32", &flag);
/* USER CODE END PV */
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim8;
TIM_HandleTypeDef htim17;

USART_HandleTypeDef husart2;

/*const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*[[noreturn]] void boatUpdate_Task(const void *){
    while(true){

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void){
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

  	/* USER CODE BEGIN Init */

  	/* USER CODE END Init */

  	/* Configure the system clock */
  	SystemClock_Config();

  	/* USER CODE BEGIN SysInit */

  	/* USER CODE END SysInit */

  	  MX_GPIO_Init();
  	  MX_TIM3_Init();
  	  MX_I2C1_Init();
  	  MX_USART2_Init();
  	  MX_TIM17_Init();
  	  MX_TIM2_Init();
  	  /* USER CODE BEGIN 2 */

  	  /* USER CODE END 2 */
  	/* CReación de las tareas.
  	 * Notas:
  	 * - Para agregar o remover tareas se deberpa hacer de forma manual
  	 *   siguiendo el formato mostrado a continuación.
  	 * - Las funciones de las tareas se deben declarar al inicio del código
  	 *   y definirse al final del mismo
  	 * - No es necesario declarar tareas en CubeMX, por lo que se debe evitar
  	 *   hacerlo, pues podría alterar el formato de las tareas previamente
  	 *   declaradas
  	 * - No alterar la prioridad ni el periodo de las tareas directamente en
  	 * 	 la declaración de estas, a menos se deban desarrollar tareas críticas
  	 * 	 en el tiempo.
  	 * - EL periodo de las tareas se debe establecer en las funciones de las
  	 *   tareas usando el comando osDelay(). Evitar en la medida de lo posible
  	 *   periodos coincidentes en las tareas*/
  	  /* creation of thrustersControl */
  	  osKernelInitialize();
  	  osThreadNew(thrustersControl,NULL,NULL);
  	  /* creation of pumpControlTask */
  	  osThreadNew(pumpControlTask,NULL,NULL);

  	  /* creation of pressureMeasure */

  	  osThreadNew(pressureMeasure,NULL,NULL);
  	  /* creation of hydrophoneTask */

  	  osThreadNew(hydrophoneTask,NULL,NULL);
  	  /* Init scheduler */
  	  osKernelStart();
    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
  	while (1){
  		select(channel4, channel2, channel5, thrusterRight, thrusterLeft, powerR, powerL);
  		flag.data = 1;
  		stm32.publish( &flag );
  		nh.spinOnce();
	    HAL_Delay(1);
    }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 75;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */

void thrustersControl(void *argument)
{
  /* EScribir aquí todo el código relacionado con el control de los motores
   * Nota: Recomiendo mover los controles de los servos del archivo "unctions.h"
   * para que se ejecuten en tiempo real. BAsarse en la librería "Servo.h"
   * NO CAMBIAR LAS CONFIGURACIONES DE LOS MOTORES EN ESTE ESPACIO.
   * En caso de requerir cambiar configuraciones notificarlo y hacerlo en el archivo
   * "Servo.cfg"*/
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_pumpControlTask */
/**
* @brief Function implementing the pumpControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pumpControlTask */
void pumpControlTask(void *argument)
{
  /* En este espacio se controlará lo relacionado con la bomba.
   * Nota: El control de la bomba es similar a los motores, con la diferencia de que
   * en este espacio se permite configurar los puertos de la bomba, pues es necesario
   * cambiar su dirección de giro y su Duty Cycle, para que esta absorba o
   * expulse los marcadores.
   * REVISIÓN: Al ser una tarea controlada de manera externa, se debe considerar
   * migrar de RTOS a maneja de interrupciones
   *  */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_pressureMeasure */
/**
* @brief Function implementing the pressureMeasure thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pressureMeasure */
void pressureMeasure(void *argument)
{
  /* Lectura del sensor de presión, la cual debe realizarse de manera constante
   * Nota: La lectura de datos se realiza mediante protocolo I2C, por lo que el
   * código no debería ser complicado de realizar. INvestigar si ya hay una
   * liberría existente que podamos usar.*/
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END pressureMeasure */
}

/* USER CODE BEGIN Header_hydrophoneTask */
/**
* @brief Function implementing the hydrophoneTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_hydrophoneTask */
void hydrophoneTask(void *argument)
{
  /* USER CODE BEGIN hydrophoneTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END hydrophoneTask */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
