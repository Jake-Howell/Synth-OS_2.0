#include "HW_Timer.h"

///*

void init_HWTimer_ISR(int psc, int arr){    //initalise a hardware timer and provide link it's ISR
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM_TypeDef* TIMER_MODULE = TIM17; //define which timer to use

    TIMER_MODULE->DIER |= TIM_DIER_UIE;					//timer update interrupt enabled
    TIMER_MODULE->PSC = psc;          //
    TIMER_MODULE->ARR = arr;

    TIMER_MODULE->CR1 |= TIM_CR1_CEN;   //Enable Timer

    NVIC_EnableIRQ(TIM17_IRQn);             //Enable Interupt
    NVIC_SetVector(TIM17_IRQn, (uint32_t)&TIM_IRQHandler); //point to ISR
}

void TIM_IRQHandler(){
    
}

//*/
