#ifndef _STM32_HARDWARE_H_
#define _STM32_HARDWARE_H_

extern "C"
{
  #include "main.h"
}

USART_HandleTypeDef UartHandle;
uint8_t Buffer_1;
uint8_t *RxBuffer = &Buffer_1;


class STM32Hardware
{
  public:
    STM32Hardware() {}

    // Initialize the ATM32
    void init()
    {
      /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
      /* UART configured as follows:
          - Word Length = 8 Bits
          - Stop Bit = One Stop bit
          - Parity = None
          - BaudRate = 57600 baud
          - Hardware flow control disabled (RTS and CTS signals) */
      UartHandle.Instance        = USART3;  // uart3 = usb, uart6 = gpio

      UartHandle.Init.BaudRate     = 57600;
      UartHandle.Init.WordLength   = USART_WORDLENGTH_8B;
      UartHandle.Init.StopBits     = USART_STOPBITS_1;
      UartHandle.Init.Parity       = USART_PARITY_NONE;
      //UartHandle.Init.HwFlowCtl    = USART_HWCONTROL_NONE;
      UartHandle.Init.Mode         = USART_MODE_TX_RX;
      //UartHandle.Init.OverSampling = USART_OVERSAMPLING_16; // compensate baud error

      if(HAL_USART_DeInit(&UartHandle) != HAL_OK)
      {
        Error_Handler();
      }
      if(HAL_USART_Init(&UartHandle) != HAL_OK)
      {
        Error_Handler();
      }

    }

    // Read a byte of data from ROS connection.
    // If no data , hal_uart-timeout, returns -1
    int read()
    {
//      BSP_LED_Toggle(LED1);
      return (HAL_USART_Receive(&UartHandle, RxBuffer, 1, 15) == HAL_OK) ? *RxBuffer : -1;
    }

    // Send a byte of data to ROS connection
    void write(uint8_t* data, int length)
    {
//      BSP_LED_Toggle(LED2);
      HAL_USART_Transmit(&UartHandle, (uint8_t*)data, (uint16_t)length, HAL_MAX_DELAY);
    }

    // Returns milliseconds since start of program
    unsigned long time(void)
    {
//      BSP_LED_Toggle(LED3);
      return HAL_GetTick();
    }

};

#endif

