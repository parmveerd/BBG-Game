

#ifndef _LED_H_
#define _LED_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void init_led();
void get_ready();
void up_down_led(int);
void flash_led(int, int, int);
void reset_led();

#endif