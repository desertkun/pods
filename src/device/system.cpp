#include <device/system.h>

extern "C"
{
	#include "stm32f1xx.h"
	#include "gpio.h"
	#include "spi.h"
	#include "main.h"

	void SystemClock_Config(void);
	void _Error_Handler(char *file, int line);
}

namespace device
{

bool System::getSystemFlag()
{
	return HAL_GPIO_ReadPin(DEVICE_ID_GPIO_Port, DEVICE_ID_Pin);
}

void System::init()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_SPI2_Init();
	MX_SPI1_Init();
}

};


extern "C" {

	void _Error_Handler(char *file, int line)
	{
	  while(1)
	  {
	  }
	}


	#ifdef  USE_FULL_ASSERT
	void assert_failed(uint8_t* file, uint32_t line)
	{
	}
	#endif

	void SystemClock_Config(void)
	{

	  RCC_OscInitTypeDef RCC_OscInitStruct;
	  RCC_ClkInitTypeDef RCC_ClkInitStruct;

		/**Initializes the CPU, AHB and APB busses clocks
		*/
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = 16;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
		_Error_Handler((char*)__FILE__, __LINE__);
	  }

		/**Initializes the CPU, AHB and APB busses clocks
		*/
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	  {
		_Error_Handler((char*)__FILE__, __LINE__);
	  }

		/**Configure the Systick interrupt time
		*/
	  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

		/**Configure the Systick
		*/
	  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	  /* SysTick_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	}

}
