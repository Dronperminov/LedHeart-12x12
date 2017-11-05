#include "led_heart_effects.h"

struct point small_heart[SMALL_HEART_SIZE] = {
  {6, 3}, {7, 2}, {8, 2}, {9, 2}, {10, 3}, {10, 4}, {10, 5}, {9, 6}, {8, 7}, {7, 8}, {6, 9},
  {5, 9}, {4, 8}, {3, 7}, {2, 6}, {1, 5}, {1, 4}, {1, 3}, {2, 2}, {3, 2}, {4, 2}, {5, 3}
};

struct point heart[HEART_SIZE] = {
  {6, 2}, {7, 1}, {8, 1}, {9, 1}, {10, 1}, {11, 2}, {11, 3},
  {11, 4}, {11, 5}, {10, 6}, {9, 7}, {8, 8}, {7, 9}, {6, 10},

  {5, 10}, {4, 9}, {3, 8}, {2, 7}, {1, 6}, {0, 5}, {0, 4},
  {0, 3}, {0, 2}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 2}
};

unsigned int text[TEXT_SIZE] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	516, 516, 1020, 516, 516, // I
	0,
	56, 68, 132, 260, 520, 520, 260, 132, 68, 56, // <3
	0,
	252, 256, 512, 512, 256, 252, // U
};

unsigned int text_2[TEXT_SIZE_2] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	516, 516, 1020, 516, 516, // I
	0,
	56, 68, 132, 260, 520, 520, 260, 132, 68, 56, // <3
	0,
	252, 256, 512, 512, 256, 252, // U
	1024, 512, // ,
	0,
	504, 516, 516, 516, // C
	0,
	504, 516, 516, 516, 504, // O
	0,
	512, 508, 4, 4, 1020, // Ë
	0,
	1020, 32, 32, 1020, // Í
	0,
	1020, 576, 576, 384, 0, 1020, // Û
	0,
	1020, 512, 1020, 512, 1020, // Ø
	0,
	1020, 96, 144, 264, 516, // Ê
	0,
	504, 516, 516, 516, 504, // Î
	0,
	764
};

void null_states() {
  for (uint8_t i = 0; i < COLUMNS_N; i++) {
    states[i] = 0;
  }
}

void delay(uint16_t ms) {
	if (ms){
		for(uint16_t t = 0; t < ms; t++) {
			_delay_ms(1);
		}
	}
}

void shift(unsigned int x[], int size, int pos, int time) {
	while (pos--) {
		unsigned int tmp = x[0];

		for(int i = 0; i < size - 1; i++) {
			x[i] = x[i + 1];
		}

		x[size - 1] = tmp;

		delay(time);
	}
}

unsigned int y_to_matrix(unsigned int y) {
	return 1 << (REVERSE ? (COLUMNS_N - 1 - y) : y);
}


void draw_heart(int time) {
  for (uint8_t i = 0; i < HEART_SIZE; i++) {
    states[heart[i].x] += y_to_matrix(heart[i].y);
  }

  delay(time);
}

void blink_heart(int time) {
  draw_heart(time);
  null_states();
  delay(time);
}

void one_way_running_heart(int time, uint8_t direction) {
  for (uint8_t i = 0; i < HEART_SIZE; i++) {
    if (direction == DIRECT) {
      states[heart[i].x] = y_to_matrix(heart[i].y);
      delay(time);
      states[heart[i].x] = 0;
    }
    else if (direction == REVERSE) {
      states[heart[HEART_SIZE - 1 - i].x] = y_to_matrix(heart[HEART_SIZE - 1 - i].y);
      delay(time);
      states[heart[HEART_SIZE - 1 - i].x] = 0;
    }
  }
}

void one_way_running_heart_blink(int time, uint8_t direction) {
  for (uint8_t i = 0; i < HEART_SIZE; i++) {
    if (direction == DIRECT) {
      states[heart[i].x] = y_to_matrix(heart[i].y);
      delay(time >> 1);
      states[heart[i].x] = 0;
      delay(time >> 1);
    }
    else if (direction == REVERSE) {
      states[heart[HEART_SIZE - 1 - i].x] = y_to_matrix(heart[HEART_SIZE - 1 - i].y);
      delay(time >> 1);
      states[heart[HEART_SIZE - 1 - i].x] = 0;
      delay(time >> 1);
    }
  }
}

void one_way_chain(int time, uint8_t direction) {
  for (uint8_t i = 0; i < HEART_SIZE; i++) {
    if (direction == DIRECT) {
      states[heart[i].x] += y_to_matrix(heart[i].y);
      delay(time);
    }
    else if (direction == REVERSE) {
      states[heart[HEART_SIZE - 1 - i].x] += y_to_matrix(heart[HEART_SIZE - 1 - i].y);
      delay(time);
    }
  }
}

void two_ways_chain_blink(int time, uint8_t direction) {
  int count = HEART_SIZE >> 1;

  while (count) {
    for (uint8_t i = 0; i < count; i++) {
      if (direction == DIRECT) {
        states[heart[i].x] += y_to_matrix(heart[i].y);
        states[heart[HEART_SIZE - 1 - i].x] += y_to_matrix(heart[HEART_SIZE - 1 - i].y);
        delay(time / 2);
        states[heart[i].x] -= y_to_matrix(heart[i].y);
        states[heart[HEART_SIZE - 1 - i].x] -= y_to_matrix(heart[HEART_SIZE - 1 - i].y);
        delay(time / 2);
      }
      else if (direction == REVERSE) {
        states[heart[HEART_SIZE / 2 - i].x] += y_to_matrix(heart[HEART_SIZE / 2 - i].y);
        states[heart[HEART_SIZE / 2 + i].x] += y_to_matrix(heart[HEART_SIZE / 2 + i].y);
        delay(time / 2);
        states[heart[HEART_SIZE / 2 - i].x] -= y_to_matrix(heart[HEART_SIZE / 2 - i].y);
        states[heart[HEART_SIZE / 2 + i].x] -= y_to_matrix(heart[HEART_SIZE / 2 + i].y);
        delay(time / 2);
      }
    }

    if (direction == DIRECT) {
      states[heart[count - 1].x] += y_to_matrix(heart[count - 1].y);
      states[heart[HEART_SIZE - count].x] += y_to_matrix(heart[HEART_SIZE - count].y);
    }
    else if (direction == REVERSE) {
      states[heart[HEART_SIZE / 2 - (count - 1)].x] += y_to_matrix(heart[HEART_SIZE / 2 - (count - 1)].y);
      states[heart[HEART_SIZE / 2 + (count - 1)].x] += y_to_matrix(heart[HEART_SIZE + (count - 1)].y);
    }

    count--;
  }
}

void two_ways_running_heart(int time, uint8_t direction) {
  for (uint8_t i = 0; i < HEART_SIZE / 2; i++) {
    if (direction == REVERSE) {
      states[heart[HEART_SIZE / 2 - 1 - i].x] += y_to_matrix(heart[HEART_SIZE / 2 - 1 - i].y);
      states[heart[HEART_SIZE / 2 + i].x] += y_to_matrix(heart[HEART_SIZE / 2 + i].y);
    }
    else if (direction == DIRECT) {
      states[heart[i].x] += y_to_matrix(heart[i].y);
      states[heart[HEART_SIZE - 1 - i].x] += y_to_matrix(heart[HEART_SIZE - 1 - i].y);
    }

    delay(time);
  }

  for (uint8_t i = 0; i < HEART_SIZE / 2; i++) {
    if (direction == REVERSE) {
      states[heart[HEART_SIZE / 2 - 1 - i].x] -= y_to_matrix(heart[HEART_SIZE / 2 - 1 - i].y);
      states[heart[HEART_SIZE / 2 + i].x] -= y_to_matrix(heart[HEART_SIZE / 2 + i].y);
    }
    else if (direction == DIRECT) {
      states[heart[i].x] -= y_to_matrix(heart[i].y);
      states[heart[HEART_SIZE - 1 - i].x] -= y_to_matrix(heart[HEART_SIZE - 1 - i].y);
    }

    delay(time);
  }
}

void stereo_level_heart(int time, uint8_t direction) {
  for (uint8_t i = 0; i < HEART_SIZE / 2; i++) {
    if (direction == DIRECT) {
      states[heart[HEART_SIZE / 2 - 1 - i].x] += y_to_matrix(heart[HEART_SIZE / 2 - 1 - i].y);
      states[heart[HEART_SIZE / 2 + i].x] += y_to_matrix(heart[HEART_SIZE / 2 + i].y);
    }
    else if (direction == REVERSE) {
      states[heart[i].x] += y_to_matrix(heart[i].y);
      states[heart[HEART_SIZE - 1 - i].x] += y_to_matrix(heart[HEART_SIZE - 1 - i].y);
    }

    delay(time);
  }

  for (uint8_t i = 0; i < HEART_SIZE / 2; i++) {
    if (direction == DIRECT) {
      states[heart[i].x] -= y_to_matrix(heart[i].y);
      states[heart[HEART_SIZE - 1 - i].x] -= y_to_matrix(heart[HEART_SIZE - 1 - i].y);
    }
    else if (direction == REVERSE) {
      states[heart[HEART_SIZE / 2 - 1 - i].x] -= y_to_matrix(heart[HEART_SIZE / 2 - 1 - i].y);
      states[heart[HEART_SIZE / 2 + i].x] -= y_to_matrix(heart[HEART_SIZE / 2 + i].y);
    }

    delay(time);
  }
}

void cool_heart(int time, uint8_t reverse) {
  uint8_t count = HEART_SIZE / 2;

  while (count) {
    for (uint8_t i = HEART_SIZE / 2 - count; i < HEART_SIZE / 2; i++) {
      states[heart[HEART_SIZE / 2 - 1 - i].x] += y_to_matrix(heart[HEART_SIZE / 2 - 1 - i].y);
      states[heart[HEART_SIZE / 2 + i].x] += y_to_matrix(heart[HEART_SIZE / 2 + i].y);
      delay(time);
    }

    count--;

    for (uint8_t i = 0; i < count; i++) {
      states[heart[i].x] -= y_to_matrix(heart[i].y);
      states[heart[HEART_SIZE - 1 - i].x] -= y_to_matrix(heart[HEART_SIZE - 1 - i].y);
      delay(time);
    }
  }
}

void draw_heart_two_ways(int time, uint8_t direction) {
  for (uint8_t i = 0; i < HEART_SIZE / 2; i++) {
    if (direction == REVERSE) {
      states[heart[HEART_SIZE / 2 - 1 - i].x] += y_to_matrix(heart[HEART_SIZE / 2 - 1 - i].y);
      states[heart[HEART_SIZE / 2 + i].x] += y_to_matrix(heart[HEART_SIZE / 2 + i].y);
    }
    else if (direction == DIRECT) {
      states[heart[i].x] += y_to_matrix(heart[i].y);
      states[heart[HEART_SIZE - 1 - i].x] += y_to_matrix(heart[HEART_SIZE - 1 - i].y);
    }

    delay(time);
  }
}

void half_heart(int time) {
	for(uint8_t i = 0; i < HEART_SIZE; i++) {
		states[heart[i].x] += y_to_matrix(heart[i].y);
	    states[heart[HEART_SIZE - 1 - i].x] = 0;

	    if (i + 1 == HEART_SIZE / 2 || i + 1 == HEART_SIZE) {
	    	delay(time);
	    }
	}
}

void beat_heart(int time) {
  for (uint8_t i = 0; i < SMALL_HEART_SIZE; i++) {
    states[small_heart[i].x] += y_to_matrix(small_heart[i].y);
  }

  delay(time);

  null_states();

  draw_heart(time);
}

void heart_effects(int effect, int time, int counts, int direction) {
  for (uint8_t count = 0; count < counts; count++) {
    null_states();

    switch (effect) {
      case DRAW_HEART:
        draw_heart(time);
        break;

      case BLINK_HEART:
        blink_heart(time);
        break;

      case DRAW_HEART_TWO_WAYS:
        draw_heart_two_ways(time, direction);
        break;

      case ONE_WAY_RUNNING_HEART:
        one_way_running_heart(time, direction);
        break;

      case ONE_WAY_CHAIN:
        one_way_chain(time, direction);
        break;

      case ONE_WAY_RUNNING_HEART_BLINK:
        one_way_running_heart_blink(time, direction);
        break;

      case TWO_WAYS_CHAIN_BLINK:
        two_ways_chain_blink(time, direction);
        break;

      case TWO_WAYS_RUNNING_HEART:
        two_ways_running_heart(time, direction);
        break;

      case STEREO_LEVEL_HEART:
        stereo_level_heart(time, direction);
        break;

      case COOL_HEART:
        cool_heart(time, direction);
        break;

      case HALF_HEART:
        half_heart(time);
        break;

      case BEAT_HEART:
       beat_heart(time);
       break;
    }
  }
}
