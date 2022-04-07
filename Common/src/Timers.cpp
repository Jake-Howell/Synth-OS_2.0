#include "stm32h7xx.h"                  // Device header

#include "../../Common/inc/Timers.hpp"



void Init_Timer1(unsigned int PSC_val, unsigned int ARR_val, bool ISR_Enable){
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//ENABLE tim1 to run on APB2 (Advanced Perphieral Bus 2's clock tick) - 90MHz in this case

	TIM1->DIER |= TIM_DIER_UIE;					//timer update interrupt enabled
	
	TIM1->PSC = PSC_val -1;							//setting pre-scaler (APB1 clock devider) 
	TIM1->ARR = ARR_val -1;							//counter reload value (Auto Reload Register - TIM2 ARR is 32 bit)	
	TIM1->CNT = 0xFFFFFFFE;							//set initial value to counter
	if (ISR_Enable == 1){
		NVIC->ISER[0]|=(1u<<28);					//timer 2 global interrupt enabled
	}
	TIM1->CR1|= TIM_CR1_CEN;						//sets first bit of controle register to 1 (this enables it)
}

void Init_Timer2(unsigned int PSC_val, unsigned int ARR_val, bool ISR_Enable){
	//RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	//ENABLE tim1 to run on APB2 (Advanced Perphieral Bus 2's clock tick) - 90MHz in this case

	TIM2->DIER |= TIM_DIER_UIE;					//timer update interrupt enabled
	
	TIM2->PSC = PSC_val -1;							//setting pre-scaler (APB1 clock devider) 
	TIM2->ARR = ARR_val -1;							//counter reload value (Auto Reload Register - TIM2 ARR is 32 bit)	
	TIM2->CNT = 0xFFFFFFFE;							//set initial value to counter
	if (ISR_Enable == 1){
		NVIC->ISER[0]|=(1u<<28);					//timer 2 global interrupt enabled
	}
	TIM2->CR1|= TIM_CR1_CEN;						//sets first bit of controle register to 1 (this enables it)
}

void MX_TIM2_Init(unsigned int PSC_val, unsigned int ARR_val, bool ISR_Enable)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  TIM_InitStruct.Prescaler = PSC_val;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = ARR_val;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
}

void MX_TIM3_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  /* TIM3 interrupt Init */
  NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM3_IRQn);

  TIM_InitStruct.Prescaler = 240;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM3);
  LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);
}


unsigned int TIM2_Elapsed_ms(unsigned int startTime){
	
	unsigned int currentTime = TIM2->CNT;
	unsigned int timeElapsed_ms = (((currentTime - startTime) & 0xFFFFFFFF)/2);
	return timeElapsed_ms;
}

unsigned int TIM3_Elapsed_us(unsigned int startTime){
	
	unsigned int currentTime = TIM3->CNT;
	unsigned int timeElapsed_us = (((currentTime - startTime) & 0xFFFF));
	return timeElapsed_us;
}


void Wait3_us(int delay_us){
	
	int start = TIM3->CNT;								//mark the stating point
	int current_CNT = start;
	
	while(((current_CNT - start) & (0xFFFF)) < delay_us){
		current_CNT = TIM3->CNT;
		
	}//calculate diffrence in time and compare to delay_cycles
	
}

void Wait3_ms(int delay_ms){
	int count = delay_ms;
	while(count > 0){
		Wait3_us(1000);
		count --;
	}
}


//class TIMER{
//	public:
//			TIMER(TIM_TypeDef * TIMER, unsigned int PSC, unsigned int ARR){
//				this->mTIMER = TIMER;
//				this->mPSC = PSC;
//				this->mARR = ARR;
//				this->mCNT = 0;
//				this->mWRAP = 0xFFFF;
//				init();
//			}
//			void init(){
//				RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
//				TIM2->DIER |= TIM_DIER_UIE;
//				TIM2->PSC = mPSC -1;
//				TIM2->ARR = mARR -1;
//				TIM2->CNT = mCNT;
//				TIM2->CR1|= TIM_CR1_CEN;
//			}
//			void delay_us(unsigned int us){
//				int start = TIM2->CNT;
//				int current_CNT = start;
//				while(((current_CNT - start) & (mWRAP)) < us){
//					current_CNT = TIM2->CNT;
//				}
//			}
//			void delay_ms(unsigned int ms){
//				for(int i = 0; i<ms; i++){
//					delay_us(1000);
//				}
//			}
//			
//			unsigned int elapsed_us(unsigned int startTime){
//				unsigned int currentTime = mTIMER->CNT;
//				unsigned int timeElapsed_us = (((currentTime - startTime) & mWRAP));
//				return timeElapsed_us;
//			}
//			IRQn_Type getIRQ(){
//				return mIRQ;
//			}
//			void enable_ISR(IRQn_Type IRQ, void*ISR){ //pass in IRQ type, and pointer to user defined ISR function
//				this->mIRQ = IRQ;
//				NVIC_EnableIRQ(IRQ);
//			}
//			void disable_ISR(){
//				NVIC_DisableIRQ(mIRQ);
//			}
//			void set_timer_32_bit(){
//				mWRAP = 0xFFFFFFFF;
//			}
//			void set_timer_16_bit(){
//				mWRAP = 0xFFFF;
//			}
//			
//			void mTIMER_IRQHandler(){
//				mISR();										//redirect to user defined ISR
//			}
//			
//	private:
//			unsigned int mPSC;
//			unsigned int mARR;
//			unsigned int mCNT;
//			bool mISR_ENABLE;
//			TIM_TypeDef * mTIMER;
//			unsigned int mWRAP;
//			IRQn_Type mIRQ;
//			void *mISR();
//};

