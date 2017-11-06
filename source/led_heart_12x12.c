#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "heart_effects/led_heart_effects.h"

#define BIG_HEART_SIZE 28

#define COL9 PB4
#define COL10 PB5
#define COL11 PB6
#define COL12 PB7

#define ROW1 PB3
#define ROW2 PB2
#define ROW3 PB1
#define ROW4 PB0

volatile int column = 0;
volatile int n = 0;

#define HEART_SIZE_1 28
#define HEART_SIZE_2 26
#define HEART_SIZE_3 28

struct point heart_1[HEART_SIZE] = {
  {6, 2}, {7, 1}, {8, 1}, {9, 1}, {10, 1}, {11, 2}, {11, 3},
  {11, 4}, {11, 5}, {10, 6}, {9, 7}, {8, 8}, {7, 9}, {6, 10},

  {5, 10}, {4, 9}, {3, 8}, {2, 7}, {1, 6}, {0, 5}, {0, 4},
  {0, 3}, {0, 2}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 2}
};

struct point heart_2[HEART_SIZE_2] = {
  {6, 2}, {7, 1}, {8, 1}, {9, 1}, {10, 2}, {11, 3},
  {11, 4}, {11, 5}, {10, 6}, {9, 7}, {8, 8}, {7, 9}, {6, 10},

  {5, 10}, {4, 9}, {3, 8}, {2, 7}, {1, 6}, {0, 5}, {0, 4},
  {0, 3}, {1, 2}, {2, 1}, {3, 1}, {4, 1}, {5, 2}
};

struct point heart_3[HEART_SIZE_3] = {
  {6, 2}, {7, 1}, {8, 0}, {9, 0}, {10, 1}, {11, 2}, {11, 3},
  {11, 4}, {11, 5}, {10, 6}, {9, 7}, {8, 8}, {7, 9}, {6, 10},

  {5, 10}, {4, 9}, {3, 8}, {2, 7}, {1, 6}, {0, 5}, {0, 4},
  {0, 3}, {0, 2}, {1, 1}, {2, 0}, {3, 0}, {4, 1}, {5, 2}
};



unsigned int reverse_bit(unsigned int bit, uint8_t size) {
	unsigned int s = 0;

	while (size--) {
		s <<= 1;
		s += bit & 1;
		bit >>= 1;
	}

	return s;
}

ISR (TIMER2_COMP_vect) {
	if (column < 8) {
		PORTC = ~(1 << (column)); // Выбираем следующий разряд
	}
	else {
		PORTB &= ~(1 << (column - 8 + 4));
	}

	PORTB |= reverse_bit(states[column] & 15, 4);
	PORTA = (uint8_t) (states[column] >> 4);

	n++;

	if( n >= 2) {
		PORTC = 0xff;
		PORTB |= (1 << COL9) | (1 << COL10) | (1 << COL11) | (1 << COL12);

		//0b11110000;
		PORTB &= ~((1 << ROW1) | (1 << ROW2) | (1 << ROW3) | (1 << ROW4));
		PORTA = 0x00;

		n = 0;

		if (column++ > COLUMNS_N - 1)
			column = 0;
	}
}

// Обработчик прерывания по переполнению таймера 2
ISR (TIMER2_OVF_vect)
{
	PORTC = 0xff; // Гасим все разряды
	PORTB |= (1 << COL9) | (1 << COL10) | (1 << COL11) | (1 << COL12);
}

unsigned int xy_to_matrix(uint8_t a) {
	return 1 << a;
}

void smooth_off(int time) {
	for(OCR2 = 0; OCR2 < 255; OCR2++) {
		delay(time);
	}

	null_states();
	OCR2 = 0;
}

void draw_fade_heart(int time, int counts) {
	null_states();
	draw_heart(0);

	int step = 1;
	int count = 0;
	OCR2 = 254;

	while (count < counts * 2) {
		OCR2 += step;
		delay(time);

		if (OCR2 > 254 || OCR2 < 1) {
			step = -step;
			count++;
		}
	}

	OCR2 = 0;
}

int main(void)
{
	DDRC = 0xff;
	PORTC = 0xff;

	DDRB = 0xff;//(1 << PB0) | (1 << PB1);
	PORTB = 0x00;
	PORTB |= (1 << PB0) | (1 << PB1);

	DDRA = 0xff;
	PORTA = 0x00;

	TCCR2 |= (1 << WGM21)|(1 << WGM20)|(1 << CS21);
	TIMSK |= (1 << TOIE2)|(1 << OCIE2);
	OCR2 = 0;

	sei();

	while (1) {
		OCR2 = 0;

		heart_effects(DRAW_HEART_TWO_WAYS, 30, 1, REVERSE);
		delay(100);
		heart_effects(DRAW_HEART_TWO_WAYS, 30, 1, DIRECT);
		delay(100);

		heart_effects(ONE_WAY_RUNNING_HEART_BLINK, 80, 1, DIRECT);
		delay(100);
		heart_effects(ONE_WAY_RUNNING_HEART_BLINK, 80, 1, REVERSE);
		delay(100);

		heart_effects(ONE_WAY_CHAIN, 80, 1, DIRECT);
		delay(100);
		heart_effects(ONE_WAY_CHAIN, 80, 1, REVERSE);
		delay(100);

		heart_effects(HALF_HEART, 300, 4, REVERSE);
		delay(100);

		heart_effects(TWO_WAYS_CHAIN_BLINK, 90, 1, DIRECT);
		delay(100);

		draw_fade_heart(3, 5);
		delay(100);

		heart_effects(COOL_HEART, 60, 1, REVERSE);
		delay(100);

		heart_effects(BLINK_HEART, 150, 4, REVERSE);
		delay(100);

		heart_effects(BEAT_HEART, 500, 6, DIRECT);
		delay(100);

		smooth_off(7);
		delay(1500);

		OCR2 = 0;

		unsigned int text_tmp[TEXT_SIZE_2];

		for(int i = 0; i < TEXT_SIZE_2; i++) {
			text_tmp[i] = text_2[i];
		}

		for(int n = 0; n < TEXT_SIZE_2 - 1; n++) {
			for(int i = 0; i < COLUMNS_N; i++) {
				states[COLUMNS_N - 1 - i] = text_tmp[i];
			}

			shift(text_tmp, TEXT_SIZE_2, 1, 150);
		}

		null_states();
	}


	return 1;
}
