/*
 * led_heart_effects.h
 *
 *  Created on: 23 ���� 2016 �.
 *      Author: Andrew
 */

#ifndef HEART_EFFECTS_LED_HEART_EFFECTS_H_
#define HEART_EFFECTS_LED_HEART_EFFECTS_H_

#include "led_heart_effects.h"
#include "inttypes.h"
#include "util/delay.h"

#define REVERSE 0

#define COLUMNS_N 12
#define ROWS_N 12

#define REVERSE 0
#define DIRECT 1

#define DRAW_HEART 0
#define BLINK_HEART 1
#define DRAW_HEART_TWO_WAYS 2
#define ONE_WAY_RUNNING_HEART 3
#define ONE_WAY_CHAIN 4
#define ONE_WAY_RUNNING_HEART_BLINK 5
#define TWO_WAYS_CHAIN_BLINK 6
#define TWO_WAYS_RUNNING_HEART 7
#define STEREO_LEVEL_HEART 8
#define COOL_HEART 9
#define HALF_HEART 10
#define BEAT_HEART 11

#define SMALL_HEART_SIZE 22
#define HEART_SIZE 28

#define TEXT_SIZE 23 + COLUMNS_N
#define TEXT_SIZE_2 73 + COLUMNS_N

struct point {
	unsigned int x, y;
};

volatile unsigned int states[COLUMNS_N];

struct point small_heart[SMALL_HEART_SIZE];

struct point heart[HEART_SIZE];

unsigned int text[TEXT_SIZE];
unsigned int text_2[TEXT_SIZE_2];

void drive_all(uint8_t value);

void delay(uint16_t ms);

void shift(unsigned int x[], int size, int pos, int time);

void null_states();

unsigned int y_to_matrix(unsigned int y);

void draw_heart(int time);

void blink_heart(int time);

void one_way_running_heart(int time, uint8_t direction);

void one_way_running_heart_blink(int time, uint8_t direction);

void one_way_chain(int time, uint8_t direction);

void two_ways_chain_blink(int time, uint8_t direction);

void two_ways_running_heart(int time, uint8_t direction);

void stereo_level_heart(int time, uint8_t direction);

void cool_heart(int time, uint8_t reverse);

void draw_heart_two_ways(int time, uint8_t direction);

void half_heart(int time);

void beat_heart(int time);

void heart_effects(int effect, int time, int counts, int direction);

#endif /* HEART_EFFECTS_LED_HEART_EFFECTS_H_ */
