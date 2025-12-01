// STM32L432KC_TIM.h    



#ifndef STM32L4_TIM_H
#define STM32L4_TIM_H

#include <stdint.h> 

#define __IO volatile

#define TIM15_BASE (0x40014000UL)
#define TIM16_BASE (0x40014400UL)

typedef struct {
    volatile uint32_t TIM15_CR1; 
    volatile uint32_t TIM15_CR2;  
    volatile uint32_t TIM15_SMCR; 
    volatile uint32_t TIM15_DIER;
    volatile uint32_t TIM15_SR;
    volatile uint32_t TIM15_EGR;
    volatile uint32_t TIM15_CCMR1;
    volatile uint32_t RESERVED;
    volatile uint32_t TIM15_CCER;
    volatile uint32_t TIM15_CNT;
    volatile uint32_t TIM15_PSC;
    volatile uint32_t TIM15_ARR;
    volatile uint32_t TIM15_RCR;
    volatile uint32_t TIM15_CCR1;
    volatile uint32_t TIM15_CCR2;
    volatile uint32_t RESERVED0;
    volatile uint32_t RESERVED1;
    volatile uint32_t TIM15_BDTR;
    volatile uint32_t TIM15_DCR;
    volatile uint32_t TIM15_DMAR;
    volatile uint32_t TIM15_OR1;
    volatile uint32_t RESERVED2;
    volatile uint32_t RESERVED3;
    volatile uint32_t RESERVED4;
    volatile uint32_t TIM15_OR2;
} TIM15_TypeDef;

#define TIM15 ((TIM15_TypeDef *) TIM15_BASE)

typedef struct {
    volatile uint32_t TIM16_CR1; 
    volatile uint32_t TIM16_CR2;  
    volatile uint32_t RESERVED;
    volatile uint32_t TIM16_DIER;
    volatile uint32_t TIM16_SR;
    volatile uint32_t TIM16_EGR;
    volatile uint32_t TIM16_CCMR1;
    volatile uint32_t RESERVED0;
    volatile uint32_t TIM16_CCER;
    volatile uint32_t TIM16_CNT;
    volatile uint32_t TIM16_PSC;
    volatile uint32_t TIM16_ARR;
    volatile uint32_t TIM16_RCR;
    volatile uint32_t TIM16_CCR1;
    volatile uint32_t RESERVED1;
    volatile uint32_t RESERVED2;
    volatile uint32_t RESERVED3;
    volatile uint32_t TIM16_BDTR;
    volatile uint32_t TIM16_DCR;
    volatile uint32_t TIM16_DMAR;
    volatile uint32_t TIM16_OR1;
    volatile uint32_t RESERVED4;
    volatile uint32_t RESERVED5;
    volatile uint32_t RESERVED6;
    volatile uint32_t TIM16_OR2;
} TIM16_TypeDef;

#define TIM16 ((TIM16_TypeDef *) TIM16_BASE)

void configureTIM15(void);
void configureTIM16(void);

void playNote(uint32_t frequency, uint32_t duration_ms);
void delay_ms(uint32_t ms);
void stopNote(void);

#endif