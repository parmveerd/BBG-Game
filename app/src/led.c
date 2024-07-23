#include "led.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define BRIGHTNESS0 "/sys/class/leds/beaglebone:green:usr0/brightness"
#define BRIGHTNESS1 "/sys/class/leds/beaglebone:green:usr1/brightness"
#define BRIGHTNESS2 "/sys/class/leds/beaglebone:green:usr2/brightness"
#define BRIGHTNESS3 "/sys/class/leds/beaglebone:green:usr3/brightness"

#define TRIGGER0 "/sys/class/leds/beaglebone:green:usr0/trigger"
#define TRIGGER1 "/sys/class/leds/beaglebone:green:usr1/trigger"
#define TRIGGER2 "/sys/class/leds/beaglebone:green:usr2/trigger"
#define TRIGGER3 "/sys/class/leds/beaglebone:green:usr3/trigger"


// Function to initialize all four led's
void init_led() {
    // First, change the triggers to none for all leds
    FILE *fp = fopen(TRIGGER0, "w");
    if (fp == NULL) {
        perror("Error with opening the trigger file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "none");
    fclose(fp);

    fp = fopen(TRIGGER1, "w");
    if (fp == NULL) {
        perror("Error with opening the trigger file!.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "none");
    fclose(fp);

    fp = fopen(TRIGGER2, "w");
    if (fp == NULL) {
        perror("Error with opening the trigger file!.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "none"); 
    fclose(fp);

    fp = fopen(TRIGGER3, "w");
    if (fp == NULL) {
        perror("Error with opening the trigger file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "none"); 
    fclose(fp);

    // Next, turn off all leds by changing the brightness to 0
    fp = fopen(BRIGHTNESS0, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);

    fp = fopen(BRIGHTNESS1, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);

    fp = fopen(BRIGHTNESS2, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);

    fp = fopen(BRIGHTNESS3, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);
}


// Function that prints the get ready message and also changes the leds to the get ready state
void get_ready() {
    printf("Get ready...\n");

    // Turn off leds 0 & 3, and turn on leds 1 & 2
    FILE *fp = fopen(BRIGHTNESS0, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);

    fp = fopen(BRIGHTNESS1, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "1");
    fclose(fp);

    fp = fopen(BRIGHTNESS2, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "1");
    fclose(fp);

    fp = fopen(BRIGHTNESS3, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);
}


// Function that will turn on the correct led depending on what the direction is (UP or DOWN)
void up_down_led(int dir) {
    // 0 = UP and 1 = DOWN so only open one of these files based on this because both are currently off
    if (dir == 0) {
        FILE *led = fopen(BRIGHTNESS0, "w");
        if (led == NULL) {
            perror("Error with opening the brightness file!\n");
            exit(EXIT_FAILURE);
        }
        fprintf(led, "1");
        fclose(led);
    } else {
        FILE *led = fopen(BRIGHTNESS3, "w");
        if (led == NULL) {
            perror("Error with opening the brightness file!\n");
            exit(EXIT_FAILURE);
        }
        fprintf(led, "1");
        fclose(led);
    }

    // Open the files for both led1 and led2, so we can turn them both off
    FILE *led1 = fopen(BRIGHTNESS1, "w");
    if (led1 == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(led1, "0");
    fclose(led1);

    FILE *led2 = fopen(BRIGHTNESS2, "w");
    if (led2 == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(led2, "0");
    fclose(led2);
}


// Function that will flash leds depending on if the user got the answer correct
void flash_led(int flashes, int on, int off) {
    // Create a sleep time for how long the leds will stay on and off for
    struct timespec req_on = {0, on};
    struct timespec req_off = {0, off};
    
    // Create four file variables since we will need to control all four leds at the same time
    FILE* led0;
    FILE* led1;
    FILE* led2;
    FILE* led3;

    // Run our loop for how many flashes we want to do
    for (int i = 0; i < flashes; i++) {
        // Open all four files so we can turn every led on
        led0 = fopen(BRIGHTNESS0, "w");
        led1 = fopen(BRIGHTNESS1, "w");
        led2 = fopen(BRIGHTNESS2, "w");
        led3 = fopen(BRIGHTNESS3, "w");

        if (led0 == NULL || led1 == NULL || led2 == NULL || led3 == NULL) {
            perror("Error with openeing up the brightness files!\n");
            exit(EXIT_FAILURE);
        }
         
        // Turn on every single led, close the files, and put our program to sleep for the specified amount of time
        fprintf(led0, "1");
        fprintf(led1, "1");
        fprintf(led2, "1");
        fprintf(led3, "1");

        fclose(led0);
        fclose(led1);
        fclose(led2);
        fclose(led3);

        nanosleep(&req_on, (struct timespec *) NULL);

        // Re-open all the files in order to turn all the leds off
        led0 = fopen(BRIGHTNESS0, "w");
        led1 = fopen(BRIGHTNESS1, "w");
        led2 = fopen(BRIGHTNESS2, "w");
        led3 = fopen(BRIGHTNESS3, "w");

        if (led0 == NULL || led1 == NULL || led2 == NULL || led3 == NULL) {
            perror("Error with openeing up the brightness files!\n");
            exit(EXIT_FAILURE);
        } 

        // Turn off all the leds, close the files, and put program to sleep for the specified amount of time
        fprintf(led0, "0");
        fprintf(led1, "0");
        fprintf(led2, "0");
        fprintf(led3, "0");

        fclose(led0);
        fclose(led1);
        fclose(led2);
        fclose(led3);

        nanosleep(&req_off, (struct timespec *) NULL);
    }
}


// Function to reset the led lights
void reset_led() {
    // First turn off all the led lights by making brightness 0
    FILE* fp = fopen(BRIGHTNESS0, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);

    fp = fopen(BRIGHTNESS1, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);

    fp = fopen(BRIGHTNESS2, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);

    fp = fopen(BRIGHTNESS3, "w");
    if (fp == NULL) {
        perror("Error with opening the brightness file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "0");
    fclose(fp);

    // Next, change the trigger back to the default settings
    fp = fopen(TRIGGER0, "w");
    if (fp == NULL) {
        perror("Error with opening the trigger file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "heartbeat");
    fclose(fp);

    fp = fopen(TRIGGER1, "w");
    if (fp == NULL) {
        perror("Error with opening the trigger file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "mmc0");
    fclose(fp);
    
    fp = fopen(TRIGGER2, "w");
    if (fp == NULL) {
        perror("Error with opening the trigger file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "cpu0");
    fclose(fp);

    fp = fopen(TRIGGER3, "w");
    if (fp == NULL) {
        perror("Error with opening the trigger file!\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "mmc0");
    fclose(fp);
}

