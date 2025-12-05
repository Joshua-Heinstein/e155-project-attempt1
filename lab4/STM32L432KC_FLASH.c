// Joshua Heinstein
// joshuaheinstein@gmail.com
// 10/6/2025
// STM32L432KC_FLASH.c
// Source code for FLASH functions

#include "STM32L432KC_FLASH.h"

void configureFlash() {
    FLASH->ACR |= (0b100); // Set to 4 waitstates
    FLASH->ACR |= (1 << 8); // Turn on the ART
}