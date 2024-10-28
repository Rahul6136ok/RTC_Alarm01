/*
 * msp.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Rahul
 */
#include "main.h"
#include "stm32f4xx_hal.h"

extern void Error_handler(void);

/*
 * @breif Initialize the msp
 * @retval None
 */
void HAL_MspInit(void)
{
	//Here will do low level processor specific inits.
	  //1. Set up the priority grouping of the arm cortex mx processor
	  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	  //2. Enable the required system exceptions of the arm cortex mx processor
	  SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault system exceptions

	  //3. configure the priority for the system exceptions
	  HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	  HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	  HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

/*
 * @brief UART MSP Init
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  *   //here we are going to do the low level inits. of the USART2 peripheral
  *
  * @retval None
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_uart;

    // Enable the clock for USART2 and GPIOA peripherals
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure PA2 (TX)
    gpio_uart.Pin = GPIO_PIN_2; // UART2_TX
    gpio_uart.Mode = GPIO_MODE_AF_PP; // Alternate function push-pull
    gpio_uart.Pull = GPIO_PULLUP; // Pull-up resistor
    gpio_uart.Speed = GPIO_SPEED_FREQ_LOW; // Speed setting
    gpio_uart.Alternate = GPIO_AF7_USART2; // Alternate function for USART2_TX
    HAL_GPIO_Init(GPIOA, &gpio_uart);

    // Configure PA3 (RX)
    gpio_uart.Pin = GPIO_PIN_3; // UART2_RX
    gpio_uart.Mode = GPIO_MODE_AF_OD; // Alternate function open-drain
    gpio_uart.Pull = GPIO_PULLUP; // Pull-up resistor
    gpio_uart.Speed = GPIO_SPEED_FREQ_LOW; // Speed setting
    gpio_uart.Alternate = GPIO_AF7_USART2; // Alternate function for USART2_RX
    HAL_GPIO_Init(GPIOA, &gpio_uart);

    // Enable the USART2 interrupt and set the priority
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
  * @brief  Initializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
/*
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef RCC_RTCPeriClkInit;


	//1. Turn on the LSE
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

	if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
	{
		Error_handler();
	}

	//2. Select LSE as RTCCLK
	RCC_RTCPeriClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	RCC_RTCPeriClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if(HAL_RCCEx_PeriphCLKConfig(&RCC_RTCPeriClkInit)!= HAL_OK)
	{
		Error_handler();
	}
	//3. Enable the RTC Clock
	__HAL_RCC_RTC_ENABLE();
}


void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef RCC_RTCPeriClkInit = {0};

    // 1. Turn on the LSE oscillator
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE; // Set the oscillator type
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;                   // Enable the LSE
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;             // No PLL for RTC

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        // Handle oscillator config error
        Error_handler();
    }

    // 2. Select LSE as RTC clock source
    RCC_RTCPeriClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;    // Select RTC peripheral
    RCC_RTCPeriClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;    // Choose LSE as the clock source
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_RTCPeriClkInit) != HAL_OK)
    {
        // Handle peripheral clock config error
        Error_handler();
    }

    // 3. Enable the RTC Clock
    __HAL_RCC_RTC_ENABLE();
}

*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef RCC_RTCPeriClkInit = {0};

    // 1. Turn on the LSI oscillator
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI; // Set the oscillator type
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;                   // Enable the LSI
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;             // No PLL for RTC

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        // Handle oscillator config error
        Error_handler();
    }

    // 2. Select LSI as RTC clock source
    RCC_RTCPeriClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;    // Select RTC peripheral
    RCC_RTCPeriClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;    // Choose LSI as the clock source
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_RTCPeriClkInit) != HAL_OK)
    {
        // Handle peripheral clock config error
        Error_handler();
    }

    // 3. Enable the RTC Clock
    __HAL_RCC_RTC_ENABLE();

    // 4. Enable the RTC alarm IRQ in the NVIC
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 15 ,0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}
