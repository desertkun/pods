/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include <sh1106.h>
#include "stm32f1xx.h"
#include "gpio.h"
#include "dma.h"
#include "spi.h"
#include "main.h"

#include "font7x10.h"
#include "font5x7.h"
#include "bitmap_tile.h"
			
void SystemClock_Config(void);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI1_Init();

	SH1106_Init();
	SH1106_Flush();

	SH1106_Orientation(LCD_ORIENT_NORMAL);
	SH1106_Contrast(127);


	int a = 0;
	int dir = 0;

	for(;;)
	{
		if (HAL_GPIO_ReadPin(CTL_A_GPIO_Port, CTL_A_Pin))
			continue;

		SH1106_Fill(0x00);
		//LCD_PutStr(a,0, "test",fnt7x10);

		LCD_Line(a, 0, a, 63);
		LCD_Line(0, a >> 1, 127, a >> 1);

		SH1106_Flush();

		if (dir == 0)
		{
			a++;

			if ( a >= 127)
			{
				dir = 1;

				SH1106_Fill(0xFF);
				SH1106_Flush();
			}
		}
		else
		{
			a--;

			if ( a <= 0)
			{
				dir = 0;

				SH1106_Fill(0xFF);
				SH1106_Flush();
			}
		}

		//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		//HAL_Delay(5);
	}
}

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
    _Error_Handler(__FILE__, __LINE__);
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
    _Error_Handler(__FILE__, __LINE__);
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
