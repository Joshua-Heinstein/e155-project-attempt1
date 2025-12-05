// main.c
// Joshua Heinstein
// joshuaheinstein@gmail.com
// 10/8/25

#include "main.h"
#include <stdio.h>

int main(void) {

    // Initialize system
    configureClock(); // Set system clock to 80MHz
    configureFlash(); // Set flash wait-states for high speed clock

    // Enable encoder pins as inputs
    gpioEnable(GPIO_PORT_A);
    pinMode(quadencA, GPIO_INPUT);
    pinMode(quadencB, GPIO_INPUT);

    // Set pull-down resistors on encoder pins
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD6, 0b10); // Set PA6 as pull-down
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD8, 0b10); // Set PA8 as pull-down

    // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable clock for TIM2
    initTIM(DELAY_TIM); // Initialize the timer

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable clock for SYSCFG

    // Configure EXTI for encoderA (PA6)
    // For PA6 and PA8
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR2_EXTI6, 0b000); // Map PA6 to EXTI6
    SYSCFG->EXTICR[2] |= _VAL2FLD(SYSCFG_EXTICR2_EXTI8, 0b000); // Map PA8 to EXTI8

    // Enable interrupts globally
    __enable_irq();

    // 1. Configure mask bit
    EXTI->IMR1 |= (1 << gpioPinOffset(encoderA));
    EXTI->IMR1 |= (1 << gpioPinOffset(encoderB));

    //Enable rising and falling edge triggers
    EXTI->RTSR1 |= (1 << gpioPinOffset(encoderA)); // Rising edge for encoder A
    EXTI->FTSR1 |= (1 << gpioPinOffset(encoderA)); // Falling edge for encoder A

    EXTI->RTSR1 |= (1 << gpioPinOffset(encoderB)); // Rising edge for encoder B
    EXTI->FTSR1 |= (1 << gpioPinOffset(encoderB)); // Falling edge for encoder B

    NVIC->ISER[0] |= (1 << EXTI9_5_IRQn); // Enable EXTI line 5-9 interrupts

    // Run indefinitely
    while(1){
        delay_millis(DELAY_TIM, 1000); // 1000ms = 1hz update

        // Read and reset pulses
        __disable_irq(); // Disable interrupts to prevent race condition
        int32_t total_counts = pulse_count;
        pulse_count = 0; // Reset the counter for the next interval
        __enable_irq();

        // Calculate revolutions
        float PPR = 408; // Pulses per revolution for the encoder
        float revolutions = (float)total_counts / PPR; // Calculate revolutions

        // Print the result
        printf("Revolutions in last second: %.2f\n", revolutions);

    }


}

void EXTI9_5_IRQHandler(void) {
    // Check if encoderA triggered the interrupt
    if (EXTI->PR1 & (1 << gpioPinOffset(encoderA))) {
        // Clear interrupt flag by writing 1 
        EXTI->PR1 |= (1 << gpioPinOffset(encoderA));

        //Read the other pin to determine direction
        if (digitalRead(encoderB) == 1) {
            pulse_count++; // Clockwise
        } else {
            pulse_count--; // Counterclockwise
        }
    }

    // Check if encoderB triggered the interrupt
    if (EXTI->PR1 & (1 << gpioPinOffset(encoderB))) {
        // Clear interrupt flag by writing 1
        EXTI->PR1 |= (1 << gpioPinOffset(encoderB));

        //Read the other pin to determine direction
        if (digitalRead(encoderA) == 1) {
            pulse_count--; // Counterclockwise
        } else {
            pulse_count++; // Clockwise
        }
    }
}