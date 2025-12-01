// STM32L432KC_TIM.c
// TIM15 PWM
// TIM16 delay
#include "STM32L432KC_TIM.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"

void configureTIM15(void) {
    // Turn off counter
    TIM15->TIM15_CR1 &= ~(0b1);

    // Turn off auto preload register
    TIM15->TIM15_CR1  &= ~(0b1 << 7);

    // Turn off Slave Master Select to use internal RCC clock
    TIM15->TIM15_SMCR &= ~(0b111);
    TIM15->TIM15_SMCR &= ~(0b1 << 16);

    // Turn off interrupts
    TIM15->TIM15_DIER &= ~(0b1);

    // Set Prescaler (divide by 500 (PSC + 1)) with PSC clock  at 10kHz
    // Sets delay to 100µS 
    TIM15->TIM15_PSC = 499;

    // Clear auto reload 
    TIM15->TIM15_ARR = 0;

    // Set counter to 9 (which is 10cycles) to make a 1ms delay
    TIM15->TIM15_ARR = 0b0000000000010011;    \
                
    // Update and Enable counter
    TIM15->TIM15_EGR   |= (0b1); // Set (UG) bit to 1
    TIM15->TIM15_CR1   |= (0b1);
}

void configureTIM16(void) {
    // Turn off counter
    TIM16->TIM16_CR1 &= ~1;

    // Set PWM mode 1 on CH1 
    TIM16->TIM16_CCMR1 &= ~(0x7 << 4);
    TIM16->TIM16_CCMR1 |=  (0x6 << 4);

    // Enable CCR1, ARR preload
    TIM16->TIM16_CCMR1 |= (1 << 3);
    TIM16->TIM16_CR1 |= (1 << 7);   // Auto-reload preload enable

    // Set active high polarity
    TIM16->TIM16_CCER &= ~(1 << 1);

    // Set divider to 1
    TIM16->TIM16_PSC = 0;

    // Enable output
    TIM16->TIM16_BDTR |= (1 << 15); 
    TIM16->TIM16_CCER |= (1 << 0); 

    // Update the registers
    TIM16->TIM16_EGR |= 1;  // UG: Update generation
}


// Play a note at specified frequency for specified duration 
//making pwm wave from tim16
void playNote(uint32_t frequency, uint32_t duration_ms) {
    if (frequency == 0) {
        // Rest (silence)
        stopNote();
    } else {
        // Calculate ARR value for desired frequency
        // PWM frequency = Timer_clock / (ARR + 1)
        // ARR = (Timer_clock / frequency) - 1
        // Timer clock = 1MHz (80MHz / 80 prescaler)
        uint32_t arr_value = (1000000 / frequency) - 1;
        
        // Update ARR and CCR1 for 50% duty cycle
        TIM16->TIM16_ARR = arr_value;
        TIM16->TIM16_CCR1 = arr_value / 2;  // 50% duty cycle

        // Generate update event to load new values
        TIM16->TIM16_EGR |= (1 << 0);
        
        // Start PWM
        TIM16->TIM16_CR1 |= (1 << 0);       // CEN: Counter enable
    }
  
    // Wait for the specified duration
    delay_ms(duration_ms);
}

void stopNote(void) {
    // Stop TIM16 counter
    TIM16->TIM16_CR1 &= ~(1 << 0);       // CEN: Disable counter
    TIM16->TIM16_CNT = 0;                // Reset counter
}


// Delay function using TIM15
void delay_ms(uint32_t ms) {

    // Setup
    TIM15->TIM15_ARR = 999;              // 1ms per overflow (1MHz / 1000)
    TIM15->TIM15_CNT = 0;                // Reset counter
    TIM15->TIM15_SR = 0;                 // Clear all flags

    // Start timer
    TIM15->TIM15_CR1 |= (1 << 0);        // CEN: Enable counter
    
    // Wait for the specified number of milliseconds
    for (uint32_t i = 0; i < ms; i++) {
        // Wait until update interrupt flag
        while (!(TIM15->TIM15_SR & (1 << 0)));
        TIM15->TIM15_SR &= ~(1 << 0);    // Clear UIF flag
    }
    
    // Stop timer
    TIM15->TIM15_CR1 &= ~(1 << 0);      // CEN: Disable counter
}