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
volatile uint16_t* Sound::s_tones = NULL;

void Sound::playMelody(const uint16_t* tones)
{
	s_tones = (volatile uint16_t*)tones;

	Sound::playNextTone();
}

void Sound::playTone(uint16_t freq, uint32_t duration)
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

	if (duration > 0)
	{
	    __HAL_TIM_SET_COUNTER(&BEEPER_DELAY_HTIM, 0);
	    __HAL_TIM_SET_AUTORELOAD(&BEEPER_DELAY_HTIM, duration);

    	__HAL_TIM_CLEAR_FLAG(&BEEPER_DELAY_HTIM, TIM_SR_UIF);
		HAL_TIM_Base_Start_IT(&BEEPER_DELAY_HTIM);
	}
}

static const uint16_t Frequencies[] = { 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902 };

void Sound::playNextTone()
{
	uint16_t nextTone = *s_tones;

	if (nextTone == 0)
	{
		Sound::stop();
		s_tones = NULL;
	}
	else
	{
		int16_t duration = nextTone >> 8;
		uint16_t frequency = Frequencies[nextTone&0xF] / ( 1 << (8-(nextTone>>4 & 0xF)) );

		playTone(frequency, duration);
	}
}

void Sound::complete()
{
	if (s_tones)
	{
		s_tones++;

		Sound::playNextTone();
	}
	else
	{
		Sound::stop();
	}
}

void Sound::stop()
{
	s_tones = NULL;

	GPIO_InitTypeDef GPIO_InitStruct;

	// Configure buzzer pin as analog input without pullup to conserve power
    GPIO_InitStruct.Pin = BUZZER_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

    HAL_TIM_PWM_Stop(&BEEPER_HTIM, BEEPER_CHANNEL);
	HAL_TIM_Base_Stop_IT(&BEEPER_DELAY_HTIM);
}

void Sound::init()
{
}

}
