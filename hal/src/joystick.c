#include "hal/joystick.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define JOYSTICK_UP "/sys/class/gpio/gpio26/value"
#define JOYSTICK_RIGHT "/sys/class/gpio/gpio47/value"
#define JOYSTICK_DOWN "/sys/class/gpio/gpio46/value"
#define JOYSTICK_LEFT "/sys/class/gpio/gpio65/value"

// Function to get time in ms (provided in the assignment pdf)
long long getTimeInMs(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    // Extract seconds and nanoseconds from the timespec structure
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;

    // Convert the time to milliseconds
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;

    // Return the time in milliseconds
    return milliSeconds;
}


// Function to make sure all directions of the joystick are configured
void export_gpio(int num) {
    char buffer[50];
    sprintf(buffer, "/sys/class/gpio/gpio%d", num);

    // Check if the pin is already exported (if it is, we just skip the rest of this function)
    struct stat st;
    if (stat(buffer, &st) == -1) {
        // Pin has not been exported, so we must do that now
        FILE *fp = fopen("/sys/class/gpio/export", "w");
        
        // Cannot open file so end the program here because it will seg fault eventually if we do not 
        if (fp == NULL) {
            perror("Error with opening export files.\n");
            exit(EXIT_FAILURE);
        }

        // Put the number of the direction into our export file to export it, then close the file
        fprintf(fp, "%d", num);
        fclose(fp);
    }
}


// Function to check if the joystick is being pressed and in which direction
int check_joy(long long final_time) {
    // The time we will use to check how long this function has been running for
    long long start_time = getTimeInMs();
    long long current_time;

    // Sleep so we can check our joysticks every 100000000 ns
    struct timespec nap = {0, 100000000};

    // Create buffers for each direction of size 3, since we are expecting only 0 or 1 from the values
    int len = 3;
    char up_buffer[len];
    char down_buffer[len];
    char left_buffer[len];
    char right_buffer[len];

    // Declare the names of the variables we will be using to open our direction files
    FILE *fp_up;
    FILE *fp_down;
    FILE *fp_right;
    FILE *fp_left;
    
    // This loop will continue to run until the joystick has been pressed or the time is up
    while(true) {
        // Check if the elapsed time exceeds the final time (which can be the timeout or the randomized time from 0.5-3s)
        current_time = getTimeInMs();
        if ((current_time - start_time) > final_time) {
            if (final_time == 5000) {
                printf("No input within 5000ms; quitting!\n");
            }
            return NONE;
        }

        // Now open our files in here so we can re-open them at every iteration
        fp_up = fopen(JOYSTICK_UP, "r");
        fp_down = fopen(JOYSTICK_DOWN, "r");
        fp_right = fopen(JOYSTICK_RIGHT, "r");
        fp_left = fopen(JOYSTICK_LEFT, "r");

        // Get the value from all 4 directions so see if any of them are being pressed
        fgets(up_buffer, len, fp_up);
        fgets(down_buffer, len, fp_down);
        fgets(right_buffer, len, fp_right);
        fgets(left_buffer, len, fp_left);

        // If any of the directions are being pressed then the value will be 0, so we can close all files and return that direction
        if (strcmp(up_buffer, "0\n") == 0) {
            fclose(fp_up);
            fclose(fp_down);
            fclose(fp_right);
            fclose(fp_left);
            return UP;
        } else if (strcmp(down_buffer, "0\n") == 0) {
            fclose(fp_up);
            fclose(fp_down);
            fclose(fp_right);
            fclose(fp_left);
            return DOWN;
        } else if (strcmp(right_buffer, "0\n") == 0) {
            fclose(fp_up);
            fclose(fp_down);
            fclose(fp_right);
            fclose(fp_left);
            return RIGHT;
        } else if (strcmp(left_buffer, "0\n") == 0) {
            fclose(fp_up);
            fclose(fp_down);
            fclose(fp_right);
            fclose(fp_left);
            return LEFT;
        }

        // Close all files if nothing has been pressed during this iteration
        fclose(fp_up);
        fclose(fp_down);
        fclose(fp_right);
        fclose(fp_left);

        // Put to sleep before the next iteration
        nanosleep(&nap, (struct timespec *) NULL);
    }
    // Return none if (on the very slight chance) we make it out of the loop
    return NONE;
}