
#include "spi.h"
//#include "dma.h"
#include "device/input.h"
#include "device/network.h"
#include "device/sound.h"

extern "C" {

#include "stm32f1xx.h"

extern TIM_HandleTypeDef BEEPER_HTIM;
extern TIM_HandleTypeDef BEEPER_DELAY_HTIM;

void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	if (pin == GPIO_PIN_5)
	{
		device::Network::interrupt();
		return;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BEEPER_DELAY_TIM)
    {
    	HAL_TIM_Base_Stop_IT(&BEEPER_DELAY_HTIM);

    	device::Sound::complete();
    }
}

extern TIM_HandleTypeDef htim3;

void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim3);
}


/*

extern DMA_HandleTypeDef hdma_spi2_tx;

void DMA1_Channel5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

*/

};
