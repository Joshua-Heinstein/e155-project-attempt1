// STM32L432KC_RCC.c
// Source code for RCC functions

#include "STM32L432KC_RCC.h"

void configurePLL() {
    // Set clock to 80 MHz
    // Output freq = (src_clk) * (N/M) / R
    // (4 MHz) * (80/1) / 4 = 80 MHz
    // M: 1, N: 80, R: 4
    // Use MSI as PLLSRC
    // Turn off PLL

    RCC->CR &= ~(1 << 24);
    
    // Wait till PLL is unlocked (e.g., off)
    while ((RCC->CR >> 25 & 1) != 0);

    // Load configuration
    // Set PLL SRC to MSI
    RCC->PLLCFGR |= (1 << 0);
    RCC->PLLCFGR &= ~(1 << 1);

    // Set PLLN
    RCC->PLLCFGR &= ~(0b11111111 << 8); // Clear all bits of PLLN
    RCC->PLLCFGR |= (0b101000 << 8); // |= 40
    
    // Set PLLM
    RCC->PLLCFGR &= ~(0b111 << 4);  // Clear all bits
    
    // Set PLLR to /2 (bits 00)
RCC->PLLCFGR &= ~((1 << 26) | (1 << 25));
    
    // Enable PLLR output
    RCC->PLLCFGR |= (1 << 24);

    // Enable PLL
    RCC->CR |= (1 << 24);
    
    // Wait until PLL is locked
    while ((RCC->CR >> 25 & 1) != 1);
}

void configureClock(){
    // Configure and turn on PLL
    configurePLL();

    // Select PLL as clock source
    RCC->CFGR &= ~(0b11 << 0);
    RCC->CFGR |= (0b11 << 0);
    while(!((RCC->CFGR >> 2) & 0b11));

    // Set AHB prescaler to /1
    RCC->CFGR &= ~(0b1111 << 4);
    
    // Set APB2prescaler to /1
    RCC->CFGR &= ~(0b111 << 11);

    // Enable AHB clock
    RCC->AHB2ENR |= (0b1 << 1);   //Enable GPIOB
    RCC->AHB2ENR |= (0b1 << 0);     // Enable GPIOA clock (for PA6)

    // Enable APB clock
    RCC->APB2ENR |= (0b1 << 16);  //Enable TIM15
    RCC->APB2ENR |= (0b1 << 17);  //Enable TIM16
}