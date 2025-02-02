/*
 * main.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Rahul
 */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "stm32f4xx_hal.h"
#include "main.h"

void GPIO_Init(void);
void Error_handler(void);
void UART2_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq);
void RTC_Init(void);
void RTC_CalendarConfig(void);
void RTC_AlarmConfig(void);

UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;

/**
  * @brief  Print a string to console over UART.
  * @param  format: Format string as in printf.
  * @param  ...: Additional arguments providing the data to print.
  * @retval None
  */
void printmsg(char *format,...)
{
  char str[80];

  /*Extract the the argument list using VA apis */
  va_list args;
  va_start(args, format);
  vsprintf(str, format,args);
  HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
  va_end(args);
}



int main(void)
{
	HAL_Init();
	GPIO_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	UART2_Init();

	printmsg("This is RTC calendar Test programe \r\n");

	RTC_Init();




/*	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();

	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
	    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);  // Clear standby flag
	    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);  // Clear wake-up flag

	    printmsg("Woke up from STANDBY\r\n");

	    HAL_GPIO_EXTI_Callback(GPIO_PIN_0); // Read RTC and display time if needed
	}

	// Enable the wakeup pin
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	printmsg("Went to STANDBY mode\r\n");

	// Enter standby mode
	HAL_PWR_EnterSTANDBYMode();
*/
	  while(1);

	  return 0;
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config_HSE(uint8_t clock_freq)
{
  RCC_OscInitTypeDef Osc_Init;
  RCC_ClkInitTypeDef Clock_Init;
    uint8_t flash_latency=0;

  Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE ;
  Osc_Init.HSEState = RCC_HSE_ON;
  Osc_Init.PLL.PLLState = RCC_PLL_ON;
  Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  switch(clock_freq) {
  case SYS_CLOCK_FREQ_50_MHZ:
    Osc_Init.PLL.PLLM = 4;
    Osc_Init.PLL.PLLN = 50;
    Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
    Osc_Init.PLL.PLLQ = 2;
    Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
                           RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
    Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
    flash_latency = 1;
    break;

  case SYS_CLOCK_FREQ_84_MHZ:
    Osc_Init.PLL.PLLM = 4;
    Osc_Init.PLL.PLLN = 84;
    Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
    Osc_Init.PLL.PLLQ = 2;
    Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
                           RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
    Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
    flash_latency = 2;
    break;

  case SYS_CLOCK_FREQ_120_MHZ:
    Osc_Init.PLL.PLLM = 4;
    Osc_Init.PLL.PLLN = 120;
    Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
    Osc_Init.PLL.PLLQ = 2;
    Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
                           RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
    Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
    flash_latency = 3;
    break;

  default:
    return ;
  }

  if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
  {
    Error_handler();
  }

  if (HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK)
  {
    Error_handler();
  }

  /*Configure the systick timer interrupt frequency (for every 1 ms) */
  uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
  HAL_SYSTICK_Config(hclk_freq/1000);

  /**Configure the Systick
  */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */

void RTC_Init(void)
{


	hrtc.Instance =RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 0x7F;
	hrtc.Init.SynchPrediv = 0xF9;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_handler();
	}

}



void RTC_CalendarConfig(void)
{
	RTC_TimeTypeDef RTC_TimeInit;
	RTC_DateTypeDef RTC_DateInit;

	//this function does RTC Calender Config
	//Lets configure the calender for time
	//: 05:24:45 PM : 20 oct 2024 TUESDAY

	RTC_TimeInit.Hours = 5;
	RTC_TimeInit.Minutes = 25;
	RTC_TimeInit.Seconds = 45;
	RTC_TimeInit.TimeFormat = RTC_HOURFORMAT12_AM;

	HAL_RTC_SetTime(&hrtc, &RTC_TimeInit, RTC_FORMAT_BIN);


	RTC_DateInit.Date = 20;
	RTC_DateInit.Month = RTC_MONTH_OCTOBER;
	RTC_DateInit.Year = 24;
	RTC_DateInit.WeekDay = RTC_WEEKDAY_SUNDAY;

	HAL_RTC_SetDate(&hrtc, &RTC_DateInit, RTC_FORMAT_BIN);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void GPIO_Init(void)
{
  // Enable clocks for GPIOD (LEDs) and GPIOA (Button)
  //__HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef ledgpio, buttongpio;

  // Initialize PD12 for onboard LED (Green LED)
  ledgpio.Pin = GPIO_PIN_5;
  ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
  ledgpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &ledgpio);

  ledgpio.Pin = GPIO_PIN_8;
  ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
  ledgpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &ledgpio);

  // Initialize PA0 for onboard user button (User Button)
  buttongpio.Pin = GPIO_PIN_0;
  buttongpio.Mode = GPIO_MODE_IT_FALLING;  // Trigger interrupt on button press (falling edge)
  buttongpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &buttongpio);

  // Configure and enable external interrupt for PA0 (EXTI0_IRQn)
  HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void UART2_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate =115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.Mode = UART_MODE_TX;

  if ( HAL_UART_Init(&huart2) != HAL_OK )
  {
    //There is a problem
    Error_handler();
  }
}
/**
  * @brief  Returns the name of the day of the week.
  *         This function takes a number (1-7) and returns the corresponding
  *         day of the week as a string.
  * @param  number: The day of the week as a number (1 for Monday, 2 for Tuesday, etc.).
  * @retval char*: The name of the corresponding day of the week.
  */

char* getDayofweek(uint8_t number)
{

	// Ensure the day number is valid (between 1 and 7)
	   if (number < 1 || number > 7)
	    {
	      return "Invalid Day"; // Handle invalid input
	    }
	char *weekday[] = { "Monday", "TuesDay", "Wednesday","Thursday","Friday","Saturday","Sunday"};

    return weekday[number-1];
}


/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	RTC_TimeTypeDef RTC_TimeRead;
	RTC_DateTypeDef RTC_DateRead;

	RTC_CalendarConfig();

	HAL_RTC_GetTime(&hrtc, &RTC_TimeRead, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateRead, RTC_FORMAT_BIN);

	//printmsg("Current time is : %02d:%02d:%2d\r\n",RTC_TimeRead.Hours,\RTC_TimeRead.Minutes,RTC_TimeRead.Seconds);

	//printmsg("Current Date is : %02d-%2d-%2d <%s> \r\n",RTC_DateRead.Month,RTC_DateRead.Date,\RTC_DateRead.Year,getDayofweek(RTC_DateRead));

	printmsg("Current Time is : %02d:%02d:%02d\r\n",RTC_TimeRead.Hours,\
	     RTC_TimeRead.Minutes,RTC_TimeRead.Seconds);
	HAL_Delay(1000);

	printmsg("Current Date is : %02d-%2d-%2d  <%s> \r\n",RTC_DateRead.Date,RTC_DateRead.Month,\
	     RTC_DateRead.Year,getDayofweek(RTC_DateRead.WeekDay));

	RTC_AlarmConfig();


}

void RTC_AlarmConfig(void)
{
	RTC_AlarmTypeDef AlarmA_Set;

	memset(&AlarmA_Set,0,sizeof(AlarmA_Set));

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	// xx:45:09
	AlarmA_Set.Alarm = RTC_ALARM_A;
	AlarmA_Set.AlarmTime.Minutes = 25;
	AlarmA_Set.AlarmTime.Seconds = 50 ;
	AlarmA_Set.AlarmMask = RTC_ALARMMASK_HOURS | RTC_ALARMMASK_DATEWEEKDAY;
	AlarmA_Set.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	if (HAL_RTC_SetAlarm_IT(&hrtc,&AlarmA_Set,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	printmsg("Alarm Set Successful \r\n");



}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	printmsg("Alarm Triggered\r\n");

	RTC_TimeTypeDef RTC_TimeRead;
	HAL_RTC_GetTime(hrtc, &RTC_TimeRead, RTC_FORMAT_BIN);
	printmsg("Current Time is : %02d:%02d:%02d\r\n",RTC_TimeRead.Hours,\
		     RTC_TimeRead.Minutes,RTC_TimeRead.Seconds);

	// For buZZEr you can use led instead of Buszzer
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_Delay(2000); //depend on systick timer so priority of systic timer is max.
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);

}
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_handler(void)
{
  while(1);
}
