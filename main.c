#include "functions.h"

int main(void)
{
    stdio_init_all();
    printf("Morse Code Capture and Playback\n");

    // Initialize system components
    init_gpio();
    init_pwm();

    // Main loop can be left empty as we're using interrupts
    while (1)
    {
        tight_loop_contents();  // Placeholder for main loop
    }

    return 0;
}
