// STM32L432KC_GPIO.c
// Source code for GPIO functions

#include "STM32L432KC_GPIO.h"

void pinMode(int pin, int function) {
    switch(function) {
        case GPIO_INPUT:
            GPIO->MODER &= ~(0b11 << 2*pin);
            break;
        case GPIO_OUTPUT:
            GPIO->MODER |= (0b1 << 2*pin);
            GPIO->MODER &= ~(0b1 << (2*pin+1));
            break;
        case GPIO_ALT:
            GPIO->MODER &= ~(0b1 << 2*pin);
            GPIO->MODER |= (0b1 << (2*pin+1));
            break;
        case GPIO_ANALOG:
            GPIO->MODER |= (0b11 << 2*pin);
            break;
    }
}

int digitalRead(int pin) {
    return ((GPIO->IDR) >> pin) & 1;
}

void digitalWrite(int pin, int val) {
    GPIO->ODR |= (1 << pin);
}

void togglePin(int pin) {
    // Use XOR to toggle
    GPIO->ODR ^= (1 << pin);
}

void PA6Output(void) {
    // Set PA6 as output for pwm    
    // AF1 is the PWM output
    pinMode(6, GPIO_ALT);

    // Choose the alternate function
    GPIO->AFRL &= (~(0b1111<<24));   
    GPIO->AFRL |= (0b1110<<24);

    // Set pin 6 as push-pull
    GPIO->OTYPER &= (~(0b1<<6));

    // Set low speed
    GPIO->OSPEEDR &= (~(0b11<<12));

    // No pull-up, pull-down
    GPIO->PURPDR &= (~(0b11<<12));
}