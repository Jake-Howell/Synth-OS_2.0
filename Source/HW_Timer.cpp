#include "HW_Timer.h"
#include "global_defs.hpp"
#include <cstdint>

extern Thread outputStreamThread;
/*
void init_HWTimer_ISR(){    //initalise a hardware timer and provide link it's ISR
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;    //enable timer 17 clock

    HW_TIMER_MODULE->DIER |= TIM_DIER_UIE;		//timer update interrupt enabled
    HW_TIMER_MODULE->PSC = 168;                //set pre scalar (devide clock source)
    HW_TIMER_MODULE->ARR = 10466;                //set auto reload register

    HW_TIMER_MODULE->CR1 |= TIM_CR1_CEN;       //Enable Timer

    NVIC_EnableIRQ(TIM17_IRQn);             //Enable Timer Interupt
    NVIC_SetVector(TIM17_IRQn, (uint32_t)&TIM_IRQHandler); //point to ISR
}
//*/
//*
void init_HWTimer_ISR(int frq){    //initalise a hardware timer and provide link it's ISR
    int psc = ((SystemCoreClock/1000000)/2) - 1; //psc = clock frequency by 1,000,000 to get 1us ticks
    int arr = 1000000/frq; //
    //psc = (168/2)-1;
    //arr = 10000;

    RCC->HW_TIMER_APB |= HW_TIMER_APB_EN;    //enable timer 17 clock

    HW_TIMER_MODULE->DIER |= TIM_DIER_UIE;		//timer update interrupt enabled
    HW_TIMER_MODULE->PSC = psc;                //set pre scalar (devide clock source)
    HW_TIMER_MODULE->ARR = arr;                //set auto reload register

    HW_TIMER_MODULE->CR1 |= TIM_CR1_CEN;       //Enable Timer

    NVIC_EnableIRQ(HW_TIMER_IRQn);             //Enable Timer Interupt
    //NVIC_SetVector(HW_TIMER_IRQn, (uint32_t)&TIM_IRQHandler); //point to ISR
}//*/

void TIM_IRQHandler(){
    HW_TIMER_MODULE->SR &= ~TIM_SR_UIF;    //clear interupt flag


    TOGGLE_GLOBAL_FLAG();
    //outputStreamThread.flags_set(SAMPLE_FLAG);
/*    while(1){
        printf("In Timer IRQ");
        __NOP();
    }//*/
}




//*/
