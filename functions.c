#include "functions.h"

#define DOT_BUTTON_PIN 22
#define DASH_BUTTON_PIN 21
#define PLAYBACK_BUTTON_PIN 20

#define MAX_CODES 12
#define LED_START_PIN 2     // Last LED
#define LED_END_PIN 13      // First LED

// Global Variables
static struct repeating_timer timer;
static uint8_t morse_code[MAX_CODES];
static uint8_t code_index = 0;
static bool playback_in_progress = false;

/*
    Initialize all buttons for the dot, dash and playback.
*/
void init_gpio()
{
    /* Dot Button with interrupt */
    gpio_init(DOT_BUTTON_PIN);
    gpio_set_dir(DOT_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(DOT_BUTTON_PIN);

    /* Dash Button with interrupt */
    gpio_init(DASH_BUTTON_PIN);
    gpio_set_dir(DASH_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(DASH_BUTTON_PIN);

    /* Playback Button with interrupt */
    gpio_init(PLAYBACK_BUTTON_PIN);
    gpio_set_dir(PLAYBACK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(PLAYBACK_BUTTON_PIN);

    // Single callback function for all button interrupts
    gpio_set_irq_enabled_with_callback(DOT_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, button_callback);
    gpio_set_irq_enabled(DASH_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(PLAYBACK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Initialize LEDs on Pico W (GP2 to GP13)
    for (uint gpio = LED_START_PIN; gpio <= LED_END_PIN; gpio++)
    {
        gpio_init(gpio);
        gpio_set_dir(gpio, GPIO_OUT);
        gpio_put(gpio, 0);  // Turn off LED
    }
}

void init_pwm()
{
    for (uint gpio = LED_START_PIN; gpio <= LED_END_PIN; gpio++)
    {
        gpio_set_function(gpio, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(gpio);
        pwm_set_wrap(slice_num, 255);  // 8-bit resolution (0-255)
        pwm_set_enabled(slice_num, true);
    }
    printf("PWM initialized on LEDs\n");
}


void button_callback(uint gpio, uint32_t events)
{
    if (playback_in_progress)
        return;

    if (gpio == DOT_BUTTON_PIN)
    {
        if (code_index < MAX_CODES)
        {
            add_dotdash(0);
            printf("Dot added: [%d]\n", code_index - 1);
        }
        else
        {
            printf("Max Capacity Reached\n");
        }
    }
    else if (gpio == DASH_BUTTON_PIN)
    {
        if (code_index < MAX_CODES)
        {
            add_dotdash(1);
            printf("Dash added: [%d]\n", code_index - 1);
        }
        else
        {
            printf("Max Capacity Reached\n");
        }
    }
    else if (gpio == PLAYBACK_BUTTON_PIN)
    {
        playback_in_progress = true;
        printf(">>>Code Playback Start\n");

        // Turn off all LEDs before playback
        for (uint led_gpio = LED_START_PIN; led_gpio <= LED_END_PIN; led_gpio++)
        {
            setup_pwm(led_gpio, 0);
        }

        // Start playback timer
        add_repeating_timer_ms(1000, timer_callback, NULL, &timer);
    }
}

/*
    In the add_dotdash function,
    0 represents a dot.
    1 represents a dash.
*/
uint add_dotdash(uint dotdash)
{
    morse_code[code_index++] = dotdash;
    return code_index;
}

bool timer_callback(struct repeating_timer *rt)
{
    static uint playback_index = 0;

    if (playback_index < code_index)
    {
        playback_update_led(playback_index, morse_code[playback_index]);
        playback_index++;
        return true;
    }
    else
    {
        printf(">>>Code Playback End\n");
        playback_in_progress = false;
        playback_index = 0;

        // Turn off all LEDs after playback
        for (uint led_gpio = LED_START_PIN; led_gpio <= LED_END_PIN; led_gpio++)
        {
            setup_pwm(led_gpio, 0);
        }

        cancel_repeating_timer(rt);
        return false;
    }
}

void playback_update_led(uint led_id, uint led_state)
{
    uint gpio = LED_END_PIN - led_id;  // Start from GP13 onwards

    // Check for invalid GPIO pin
    if (gpio < LED_START_PIN || gpio > LED_END_PIN)
        return;

    if (led_state == 0)
    {
        setup_pwm(gpio, 255);
        printf("Dot at [%d]\n", led_id);
    }
    else
    {
        setup_pwm(gpio, 25);
        printf("Dash at [%d]\n", led_id);
    }
}

void setup_pwm(uint gpio, uint dutycycle)
{
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);

    pwm_set_chan_level(slice_num, channel, dutycycle);
}

void main_fn()
{
}
