
#include "spi.h"
//#include "dma.h"
#include "device/input.h"
#include "device/network.h"

extern "C" {

#include "stm32f1xx.h"

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

/*

extern DMA_HandleTypeDef hdma_spi2_tx;

void DMA1_Channel5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

*/

};
