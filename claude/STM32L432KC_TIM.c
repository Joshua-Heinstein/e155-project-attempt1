// STM32L432KC_TIM.c
// TIM15 PWM
// TIM16 delay
#include "STM32L432KC_TIM.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"

void configureTIM15(void) {
    
    GPIOB->MODER &= ~(0b11 << 28);  // Clear bits for PB14
    GPIOB->MODER |= (0b10 << 28);   // Set to AF mode
    
    GPIOB->AFRH &= ~(0xF << 24);    // Clear AF bits for PB14
    GPIOB->AFRH |= (0xE << 24);     // Set AF14 

    // Enable TIM15 clock
    RCC->APB2ENR |= (1 << 16);

    // Configure TIM15 for pwm
    TIM15->TIM15_PSC = 79;          // Prescaler: 80MHz / 80 = 1MHz timer clock
    TIM15->TIM15_ARR = 1000;        // Default ARR value (will be updated for each note)
    TIM15->TIM15_CCR1 = 500;        // 50% duty cycle

    // Configure PWM mode 1 on channel 1
    TIM15->TIM15_CCMR1 &= ~(0b111 << 4); // Clear OC1M bits
    TIM15->TIM15_CCMR1 |= (0b110 << 4);  // PWM mode 1
    TIM15->TIM15_CCMR1 |= (1 << 3);      // Preload enable

    // Enable output on channel 1
    TIM15->TIM15_CCER |= (1 << 0);       // Enable channel 1 output
    
    // Enable main output 
    TIM15->TIM15_BDTR |= (1 << 15);      // Main output enable
    
    // Enable auto reload preload
    TIM15->TIM15_CR1 |= (1 << 7);        // Auto-reload preload enable
    
    // Load registers
    TIM15->TIM15_EGR |= (1 << 0);        // UG: Update generation
}

void configureTIM16(void) {
    // Enable TIM16 clock
    RCC->APB2ENR |= (1 << 17);

    // Configure TIM16
    TIM16->TIM16_PSC = 79;               // 80MHz / 80 = 1MHz (1us per tick)
    TIM16->TIM16_ARR = 999;              // Count to 1000 for 1ms
    
    // Enable auto reload preload
    TIM16->TIM16_CR1 |= (1 << 7);        // Auto-reload preload enable
    
    // Load prescaler
    TIM16->TIM16_EGR |= (1 << 0);        // UG: Update generation
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
        TIM15->TIM15_ARR = arr_value;
        TIM15->TIM15_CCR1 = arr_value / 2;  // 50% duty cycle
        
        // Generate update event to load new values
        TIM15->TIM15_EGR |= (1 << 0);
        
        // Start PWM
        TIM15->TIM15_CR1 |= (1 << 0);       // CEN: Counter enable
    }
  
    // Wait for the specified duration
    delay_ms(duration_ms);
}

void stopNote(void) {
    // Stop TIM15 counter
    TIM15->TIM15_CR1 &= ~(1 << 0);       // CEN: Disable counter
    TIM15->TIM15_CNT = 0;                // Reset counter
}


// Delay function using TIM16
void delay_ms(uint32_t ms) {

    // Setup
    TIM16->TIM16_ARR = 999;              // 1ms per overflow (1MHz / 1000)
    TIM16->TIM16_CNT = 0;                // Reset counter
    TIM16->TIM16_SR = 0;                 // Clear all flags
    
    // Start timer
    TIM16->TIM16_CR1 |= (1 << 0);        // CEN: Enable counter
    
    // Wait for the specified number of milliseconds
    for (uint32_t i = 0; i < ms; i++) {
        // Wait until update interrupt flag
        while (!(TIM16->TIM16_SR & (1 << 0)));
        TIM16->TIM16_SR &= ~(1 << 0);    // Clear UIF flag
    }
    
    // Stop timer
    TIM16->TIM16_CR1 &= ~(1 << 0);      // CEN: Disable counter
}