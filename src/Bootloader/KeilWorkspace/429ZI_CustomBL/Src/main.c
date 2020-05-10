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
#include "crc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define MAX_CMD_LEN	256
uint8_t cmd_rx_buffer[MAX_CMD_LEN];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	// uint8_t msg[] = "Hello Make BootLoader\r\n"; 
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_CRC_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
				read_bootloader_cmd();
			}
			else {
				// Jump to User App
				jump_to_user_app();
			}

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#define USER_APP_FLASH_BASE_ADDR	0x08008000

void jump_to_user_app(void) 
{
	void (*app_reset_handler)(void);
	
	uint32_t msp_value = *(volatile uint32_t *)(USER_APP_FLASH_BASE_ADDR);
	
	__set_MSP(msp_value);
	
	uint32_t reset_handler_addr = *(volatile uint32_t*)(USER_APP_FLASH_BASE_ADDR + 4);
	app_reset_handler = (void *)reset_handler_addr;
	
	app_reset_handler();
}

void read_bootloader_cmd(void)
{
	uint8_t cmd_len = 0;
	
	while(1) {
		memset(cmd_rx_buffer, 0, MAX_CMD_LEN);
		HAL_UART_Receive(&huart1, &cmd_rx_buffer[0], 1, HAL_MAX_DELAY);
		cmd_len = cmd_rx_buffer[0];
		HAL_UART_Receive(&huart1, &cmd_rx_buffer[1], cmd_len, HAL_MAX_DELAY);
		
		switch(cmd_rx_buffer[1]) {
			case GET_VER:
				bootloader_getver_cmd(cmd_rx_buffer);
				break;
			case GET_HELP:
				break;
			case GET_ID:
				break;
			default:
				break;
		}
	}
}

void print_debug_msg(char *format, ...)
{
	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	HAL_UART_Transmit(&huart5, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
	va_end(args);
}

void send_ack(int reply_len)
{
		uint8_t ack_buf[2];
		ack_buf[0] = ACK;
		ack_buf[1] = reply_len;
		HAL_UART_Transmit(&huart1, ack_buf, 2, HAL_MAX_DELAY);
}

void send_nack(void)
{
		uint8_t nack;
		nack = NACK;
		HAL_UART_Transmit(&huart1, &nack, 1, HAL_MAX_DELAY);
}

uint8_t check_crc(uint8_t *pData, uint32_t len, uint32_t crc_host)
{
	uint32_t crcValue = 0xff;
	
	for(uint32_t i = 0; i < len; i++) {
		uint32_t i_data = pData[i];
		crcValue = HAL_CRC_Accumulate(&hcrc, &i_data, 1);
	}
	
	if(crcValue == crc_host)
	{
		return 0;
	}
	else {
		return 1;
	}
}

void bootloader_getver_cmd(uint8_t *pBuffer)
{
	uint8_t bl_version;
	
	print_debug_msg("start bootloader_getver_cmd!\r\n");
	
	uint32_t command_len = pBuffer[0] + 1;
	uint32_t host_crc = *((uint32_t *)(pBuffer + command_len - 4));
	
	if (! check_crc(&pBuffer[0],command_len-4,host_crc))
    {
        print_debug_msg("BL_DEBUG_MSG:checksum success !!\n");
        // checksum is correct..
        send_ack(1);
        // bl_version=get_bootloader_version();
        print_debug_msg("BL_DEBUG_MSG:BL_VER : %d %#x\n",bl_version,bl_version);
        // bootloader_uart_write_data(&bl_version,1);

    }else
    {
        print_debug_msg("BL_DEBUG_MSG:checksum fail !!\n");
        //checksum is wrong send nack
        send_nack();
    }
	
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
