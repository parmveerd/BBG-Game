
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "hal/joystick.h"
#include "led.h"


// Function to run commands (provided in the assignment pdf)
static void runCommand(char* command){
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) {
            break;
        }
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}


int main() {
    // Call our export function for all four direction to make sure they are all exported
    int gp[] = {26, 47, 46, 65};
    for (int i = 0; i < 4; i++) {
        export_gpio(gp[i]);
    }

    printf("Hello embedded world, from Parmveer!\n\n");
    printf("When the LEDs light up, press the joystick in that direction!\n");
    printf("(Press left or right to exit)\n");

    srand(time(NULL));
    int random; // variable that will randomly select up or down
    int direction = NONE; // initialize our direction as none 

    // Initialize the leds so we can control them
    init_led();

    // Create a slight sleep in between each run command call to make sure our pins are configured correctly
    struct timespec delay = {0, 100000000};
    runCommand("config-pin p8.14 gpio");
    nanosleep(&delay, (struct timespec *) NULL);
    runCommand("config-pin p8.15 gpio");
    nanosleep(&delay, (struct timespec *) NULL);
    runCommand("config-pin p8.16 gpio");
    nanosleep(&delay, (struct timespec *) NULL);
    runCommand("config-pin p8.18 gpio");
    nanosleep(&delay, (struct timespec *) NULL);

    long long best_time = 5000; // variable that will keep track of the users best score
    int msg_checker = 0; // variable to check if the joystick is being held
    long long time_random; // variable that will store the random time we generate from 0.5 to 3s
    int joy_value; // variable that will store what joystick direction we got from calling our joystick function
    
    // The main loop that will keep our program running
    while (true) {
        // Function to let the user know to get ready with the leds and a message
        get_ready();

        // Loop to check if the joystick is being held
        while (true) {
            // 10ms is more than enough to check if joystick is being held
            if (check_joy(10) != NONE) {
                // If statement so the same message does not keep getting printed during each loop, one time is enough
                if (msg_checker == 0) {
                    printf("Please let go of the joystick...\n");
                    msg_checker = 1;
                }
            } else {
                // The joystick is not being held, so we can break out of this loop
                break;
            }
            // Put it to a small sleep to avoid any seg faults from accessing gpio too fast
            nanosleep(&delay, (struct timespec *) NULL);
        }

        // reset our checker
        msg_checker = 0;
        
        // Select a random time from 0.5s to 3s
        time_random = (rand() % 2501) + 500;
        joy_value = check_joy(time_random);
        struct timespec nap = {0, 300000000};

        // If joystick is pressed too soon then go back to step 1, exit if it is R or L, else continue to the next step
        if (joy_value == UP || joy_value == DOWN) {
            printf("Too soon!\n");
            nanosleep(&nap, (struct timespec *) NULL); // Do a very small nap before going back to step 1, to ensure we give user some time to let go of joystick
            continue;
        } else if (joy_value == RIGHT || joy_value == LEFT) {
            printf("User selected to quit.\n");
            reset_led();
            return 0;
        }

        // Randomly select up or down and let the led's know the select direction (0 = UP and 1 = DOWN)
        random = rand() % 2;
        if (random == 0) {
            printf("Press UP now!\n");
        } else {
            printf("Press DOWN now!\n");
        }
        up_down_led(random);

        // Calculate the time it takes for our joystick function to finish
        long long start_time = getTimeInMs();
        direction = check_joy(5000); // this function will automatically stop after 5 secs, so don't need to worry about stopping it from here
        long long end_time = getTimeInMs();
        
        // No input (stopped after 5s), so end the program
        if (direction == NONE) {
            reset_led();
            return 0;
        }

        // Check to see if the inputted direction is the same as the one randomly selected by our program
        if ((random == 0 && direction == UP) || (random == 1 && direction == DOWN)){
            printf("Correct!\n");
            if ((end_time - start_time) < best_time) { // Update our best time if this time is better, and let the user know about it
                best_time = end_time - start_time;
                printf("New best time!\n");
            }
            printf("Your reaction time was %lldms; best so far in game is %lldms.\n", end_time - start_time, best_time);
            flash_led(2, 125000000, 125000000); // Flash all leds at 4hz for 0.5s, since they were correct
        } else if (direction == RIGHT || direction == LEFT) { // User wanted to quit so reset our leds and end the program
            printf("User selected to quit.\n");
            reset_led();
            return 0;
        } else {
            printf("Incorrect.\n");
            flash_led(10, 50000000, 50000000); // Flash all leds at 10hz for 1s since they were incorrect
        }
    }

    reset_led();

    return 0;
}
