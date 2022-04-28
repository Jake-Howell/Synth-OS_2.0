#include "HW_Timer.h"
#define TIMER_MODULE TIM17
#define FLAG_PORT GPIOD
#define FLAG_PIN    6
///*


void init_HWTimer_ISR(int psc, int arr){    //initalise a hardware timer and provide link it's ISR
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;    //enable timer 17 clock

    TIMER_MODULE->DIER |= TIM_DIER_UIE;		//timer update interrupt enabled
    TIMER_MODULE->PSC = psc;                //set pre scalar (devide clock source)
    TIMER_MODULE->ARR = arr;                //set auto reload register

    TIMER_MODULE->CR1 |= TIM_CR1_CEN;       //Enable Timer

    NVIC_EnableIRQ(TIM17_IRQn);             //Enable Timer Interupt
    NVIC_SetVector(TIM17_IRQn, (uint32_t)&TIM_IRQHandler); //point to ISR

    FLAG_PORT->MODER &= ~(3u<<(FLAG_PIN*2));  //reset GPIOD Pin 14
    FLAG_PORT->MODER |=  (1u<<(FLAG_PIN*2));  //set GPIOD Pin 14 to output mode, to test interrupt timing
    FLAG_PORT->PUPDR |= PullDown;
}

void TIM_IRQHandler(){
    TIMER_MODULE->SR &= ~TIM_SR_UIF;    //clear interupt flag

    //pulse PD_14 high and low for timing measurements
    //FLAG_PORT->ODR = (0u<<FLAG_PIN);              
    FLAG_PORT->BSRR |= (1u<<FLAG_PIN);
    FLAG_PORT->BSRR |= (1u<<(FLAG_PIN+16));

}

//*/
