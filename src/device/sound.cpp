/*
 * sound.cpp
 */


#include <device/sound.h>

extern "C" {
	#include "stm32f1xx.h"
	#include "gpio.h"
	#include "tim.h"

	extern TIM_HandleTypeDef BEEPER_HTIM;
}

namespace device
{

volatile bool Sound::s_enabled = false;
volatile Tone* Sound::s_tones = NULL;

void Sound::playMelody(const Tone* tones)
{
	s_tones = (volatile Tone*)tones;

	playTone(s_tones->frequency, s_tones->duration);
}

void Sound::playTone(uint16_t freq, uint32_t duration)
{
	if (freq < 100 || freq > 8000 || duration == 0)
	{
		Sound::stop();
	}
	else
	{
		GPIO_InitTypeDef GPIO_InitStruct;

		// Configure buzzer pin
	    GPIO_InitStruct.Pin = BUZZER_Pin;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_PULLUP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	    HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

	    BEEPER_TIM->ARR = SystemCoreClock / (freq * BEEPER_TIM->PSC) - 1;

	    __HAL_TIM_SET_COMPARE(&BEEPER_HTIM, BEEPER_CHANNEL, BEEPER_TIM->ARR >> 1);
	    HAL_TIM_PWM_Start(&BEEPER_HTIM, BEEPER_CHANNEL);
	}

	if (duration > 0)
	{
	    __HAL_TIM_SET_COUNTER(&BEEPER_DELAY_HTIM, 0);
	    __HAL_TIM_SET_AUTORELOAD(&BEEPER_DELAY_HTIM, duration);

    	__HAL_TIM_CLEAR_FLAG(&BEEPER_DELAY_HTIM, TIM_SR_UIF);
		HAL_TIM_Base_Start_IT(&BEEPER_DELAY_HTIM);
	}
}

void Sound::complete()
{
	if (s_tones)
	{
		s_tones++;

		if (s_tones->duration == 0)
		{
			Sound::stop();
			s_tones = NULL;
		}
		else
		{
			playTone(s_tones->frequency, s_tones->duration * 10);
		}
	}
	else
	{
		Sound::stop();
	}
}

void Sound::stop()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Configure buzzer pin as analog input without pullup to conserve power
    GPIO_InitStruct.Pin = BUZZER_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

    HAL_TIM_PWM_Stop(&BEEPER_HTIM, BEEPER_CHANNEL);
}

void Sound::init()
{
}

}
