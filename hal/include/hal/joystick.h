

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

enum direction{NONE, UP, RIGHT, DOWN, LEFT};

long long getTimeInMs(void);
void export_gpio(int);
int check_joy(long long);


#endif