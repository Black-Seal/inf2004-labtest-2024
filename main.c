#include "functions.h"

int main(void)
{
    stdio_init_all();
    printf("Morse Code Capture and Playback\n");

    // Initialize system components
    init_gpio();
    init_pwm();

    
    for (;;)
    {
    }

    return 0;
}
