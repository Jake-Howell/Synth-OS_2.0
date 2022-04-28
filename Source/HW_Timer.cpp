#include "HW_Timer.h"
#include "global_defs.hpp"
///*
extern bool SAMPLE_FLAG;

void init_HWTimer_ISR(int psc, int arr){    //initalise a hardware timer and provide link it's ISR
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;    //enable timer 17 clock

    HW_TIMER_MODULE->DIER |= TIM_DIER_UIE;		//timer update interrupt enabled
    HW_TIMER_MODULE->PSC = psc;                //set pre scalar (devide clock source)
    HW_TIMER_MODULE->ARR = arr;                //set auto reload register

    HW_TIMER_MODULE->CR1 |= TIM_CR1_CEN;       //Enable Timer

    NVIC_EnableIRQ(TIM17_IRQn);             //Enable Timer Interupt
    NVIC_SetVector(TIM17_IRQn, (uint32_t)&TIM_IRQHandler); //point to ISR

    GLOBAL_FLAG_PORT->MODER &= ~(3u<<(GLOBAL_FLAG_PIN*2));  //reset GPIOD Pin 14
    GLOBAL_FLAG_PORT->MODER |=  (1u<<(GLOBAL_FLAG_PIN*2));  //set GPIOD Pin 14 to output mode, to test interrupt timing
    GLOBAL_FLAG_PORT->PUPDR |= PullDown;
}

void TIM_IRQHandler(){
    HW_TIMER_MODULE->SR &= ~TIM_SR_UIF;    //clear interupt flag

    //pulse PD_14 high and low for timing measurements
    //FLAG_PORT->ODR = (0u<<FLAG_PIN);              
    PULSE_GLOBAL_FLAG();
    SAMPLE_FLAG = 1;

}



//*/
