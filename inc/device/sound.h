
#ifndef DEVICE_SOUND_H_
#define DEVICE_SOUND_H_

#include <stdint.h>
#include <map>

extern "C" {
	#include "stm32f1xx.h"
}

#define BEEPER_HTIM              htim4
#define BEEPER_DELAY_HTIM        htim3
#define BEEPER_TIM               TIM4
#define BEEPER_CHANNEL			 TIM_CHANNEL_1

#define BEEPER_DELAY_TIM         TIM3

namespace device
{
	struct Tone {
		uint16_t frequency;
		uint8_t  duration;
	};

	class Sound
	{
	public:
		static void complete();
		static void playTone(uint16_t freq, uint32_t duration);
		static void playMelody(const Tone* tones);
		static void stop();
		static void init();
	private:
		static volatile bool s_enabled;
		static volatile Tone* s_tones;
	};

};

#endif /* DEVICE_SOUND_H_ */
