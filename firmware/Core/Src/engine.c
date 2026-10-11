/*
 * engine.c
 *
 *  Created on: Mar 29, 2026
 *      Author: Pavel
 */
#include "engine.h"



BUTTON button_1;
EFFEXTS_CTX effects_ctx;
SETTINGS_CTX settings_ctx;
BATTERY_CTX bat_ctx;
void Engine_Init()
{
	Power_InitAfterStandby();
	button_init(&button_1, GPIOB, 5);
	initEEPROM();
	effects_ctx.state = ST_STATIC;
	effects_ctx.hue = eeRead(HUE_ID);
	effects_ctx.renew = TRUE;
	effects_ctx.enableSettings = FALSE;
	effects_ctx.enableStandBy = TRUE;

	settings_ctx.state = ST_IDLE;
	settings_ctx.enableSettingHue = FALSE;



	bat_ctx.checkCounter = 0;
	bat_ctx.enableMeasure = TRUE;
	bat_ctx.enableIndication = FALSE;
	bat_ctx.checkPeriod = CHECK_BAT_PERIOD_AT_START;
	checkPowerLogic(START);
	bat_ctx.status = getBatStatus();
	if(bat_ctx.status == DISCHARGED)
	{
		indicateDischarge();
		goStandBy();
	}
	clearPixel();

}

void Engine_Config()
{

	RCC->AHBENR |=    RCC_AHBENR_DMA1EN  	// enable DMA1
					| RCC_AHBENR_CRCEN;		// enable CRC

	RCC->APBENR1 |=   RCC_APBENR1_PWREN 	// enable PWR
					| RCC_APBENR1_TIM3EN; 	// enable TIM3

	RCC->APBENR2 |=   RCC_APBENR2_TIM14EN 	// enable TIM14
					| RCC_APBENR2_TIM16EN	// enable TIM16
					| RCC_APBENR2_ADCEN;
	//enable clock for ports
	RCC->IOPENR |=    RCC_IOPENR_GPIOAEN	// enable GPIO A
					| RCC_IOPENR_GPIOBEN	// enable GPIO B
					| RCC_IOPENR_GPIOCEN;	// enable GPIO C
	RCC->AHBENR |= RCC_AHBENR_CRCEN;



	// pin PC14 as output
	GPIOC->MODER &= ~GPIO_MODER_MODE14;
	GPIOC->MODER |= GPIO_MODER_MODE14_0; // 01 - OUTPUT
	GPIO_SET_PIN(GPIOC, LED_PIN);

	//pin PB9 as enable output for 5 volts
	GPIOB->MODER &= ~GPIO_MODER_MODE9;
	GPIOB->MODER |= GPIO_MODER_MODE9_0; // 01 - OUTPUT

	disable5V(); // disable at start

	// config wake-up pins. PULL-UP
	GPIOA->MODER &= ~GPIO_MODER_MODE0;

	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5; 	// reset PUPDR
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD5_0;

	// pin PB5 as input PULL-UP for button
	GPIOB->MODER &= ~GPIO_MODER_MODE5;  // 00 - input




	/* Signal LEDs configuration START*/

	//PA11 as SL1
	GPIOA->MODER &= ~GPIO_MODER_MODE11;  // reset
	GPIOA->MODER |= GPIO_MODER_MODE11_0; // PA11 as output 01
	GPIO_SET_PIN(SL1_PORT, SL1_PIN); // off at start
	//PA10 as SL2
	GPIOA->MODER &= ~GPIO_MODER_MODE10;  // reset
	GPIOA->MODER |= GPIO_MODER_MODE10_0; // PA10 as output 01
	GPIO_SET_PIN(SL2_PORT, SL2_PIN); // off at start

	/* Signal LEDs configuration END*/

	/*START power LED configuration*/

	//PB8 as PL1
	GPIOB->MODER &= ~GPIO_MODER_MODE8; // reset MODER
	GPIOB->MODER |= GPIO_MODER_MODE8_0; // PB8 as output 01
	GPIO_SET_PIN(PL1_PORT, PL1_PIN); // off at start
	//PB8 as PL2
	GPIOB->MODER &= ~GPIO_MODER_MODE7; // reset MODER
	GPIOB->MODER |= GPIO_MODER_MODE7_0; // PB7 as output 01
	GPIO_SET_PIN(PL2_PORT, PL2_PIN); // off at start

	/*END power LED configuration*/

	// pin PA6 as PWM output TIM3 CH1
	GPIOA->MODER &= ~GPIO_MODER_MODE6; 		// Reset bits
	GPIOA->MODER |= GPIO_MODER_MODE6_1;   	// 10 = AF
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL6; 	// Reset bits
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_0;   // AF1 for PA6
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED6;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED6; // very high speed

	//TIM3 set-up
	TIM3->PSC = 0;
	TIM3->ARR = 30 - 1; // Получается 800 кГц, 1,25 мкс
	TIM3->CCR1 = 0;
	TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // PWM mode
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE; // preload enable
	TIM3->CCER |= TIM_CCER_CC1E; // enable CH1 output
	TIM3->DIER &= ~TIM_DIER_CC1DE;
	TIM3->DIER |= TIM_DIER_UDE;
	TIM3->EGR |= TIM_EGR_UG; // force update
	//DMA+DMAMUX set-up
	DMA1_Channel1->CCR = 0; // выключить все перед настройкой
	DMA1_Channel1->CPAR = (uint32_t)&TIM3->CCR1;  // where
	DMA1_Channel1->CCR =   DMA_CCR_PL
						 | DMA_CCR_DIR 	// memory to peripheral
			  	  	     | DMA_CCR_MINC 	// increment of memory
						 | DMA_CCR_PSIZE_0 	// 16 bit
						 | DMA_CCR_MSIZE_0; 	// 16 bit
	DMA1_Channel1->CCR |= DMA_CCR_TCIE;
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	DMAMUX1_Channel0->CCR = 0;      // сбросить старые настройки
	DMAMUX1_Channel0->CCR |= 37U;   // выставить нужный DMAREQ_ID

	//TIM14 for button process
	TIM14->PSC = 24000 - 1; 	// f = 1kHz -> T = 1 ms
	TIM14->ARR = 10 - 1; 	// T = 10 ms или f = 100 Hz
	TIM14->EGR |= TIM_EGR_UG; // update now
	TIM14->CR1 |= TIM_CR1_URS;
	TIM14->SR &= ~TIM_SR_UIF;
	TIM14->CR1 |= TIM_CR1_CEN;

	//TIM16 for effects
	TIM16->PSC = 4000-1;
	TIM16->ARR = 100-1;
	TIM16->SR = 0;
	TIM16->EGR |= TIM_EGR_UG;
	TIM16->DIER |= TIM_DIER_UIE;
	TIM16->CR1 |= TIM_CR1_CEN;

	/*START ADC pin PA7 configuration*/

	GPIOA->MODER &= ~GPIO_MODER_MODE7; // reset MODER
	GPIOA->MODER |= GPIO_MODER_MODE7; // analog mode 11

	/*END ADC pin PA7 configuration end*/

	/*START ADC configuration*/

	ADC1->CR &= ~ADC_CR_ADEN; // ADC off
	while (ADC1->CR & ADC_CR_ADEN); // wait for adc off

	ADC1->CR |= ADC_CR_ADVREGEN; // enable voltage regulator
	HAL_Delay(1); // delay for stab

	ADC1->CR |= ADC_CR_ADCAL; // calibrate ADC
	while (ADC1->CR & ADC_CR_ADCAL); // wait for adc calibration

	// sampling time
	ADC1->SMPR |= ADC_SMPR_SMP1;      // 160.5 cycles = 111
	ADC1->SMPR &= ~ADC_SMPR_SMPSEL13;  // channel 13
	ADC1->SMPR &= ~ADC_SMPR_SMPSEL7;  // channel 7

	ADC1->CFGR1 = 0; // reset configuration for CFGR1

	ADC->CCR |= ADC_CCR_VREFEN; // enable vrefint
	HAL_Delay(1); // delay for stab

	ADC1->ISR |= ADC_ISR_EOC | ADC_ISR_EOS; // clear flags

	ADC1->CR |= ADC_CR_ADEN; // enable ADC
	while (!(ADC1->ISR & ADC_ISR_ADRDY));  // wait

	// pin PA3 as output for enable/disable measure bat voltage
	GPIOA->MODER &= ~GPIO_MODER_MODE3; // reset
	GPIOA->MODER |= GPIO_MODER_MODE3_0; // PA3 as output 01
	GPIO_RESET_PIN(GPIOA, EN_BAT_MEASURE_PIN); // disable at start

	/*END ADC configuration*/
	configBQ24070();

}

void DMA1_Channel1_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF1)   // проверка флага окончания передачи
    {
        DMA1->IFCR |= DMA_IFCR_CTCIF1; // сброс флага
        TIM3->CR1 &= ~TIM_CR1_CEN;         // остановить таймер
    }
}



void Engine_Main()
{

	BUTTON_EV event = EV_NONE;


	if (TIM14->SR & TIM_SR_UIF)
	{
	    TIM14->SR &= ~TIM_SR_UIF;
	    button_process(&button_1);
	    switch( get_button_status(&button_1))
	    {
	    case BTN_EVENT_SHORT:
	    	event = EV_SHORT;
	    	break;
	    case BTN_EVENT_LONG2:
	    	event = EV_LONG2;
	    	break;
	    case BTN_EVENT_LONG5:
	    	event = EV_LONG5;
	    	break;
	    default:
	    	break;
	    }
	    if((PWR->SR1 & PWR_SR1_WUF6))
	    	indicatePowerLvl_PeriodCheck();
	    checkPowerLogic(RUN);

	    indicateCharger();

	}
	if(!(PWR->SR1 & PWR_SR1_WUF6) && !(PWR->SR1 & PWR_SR1_WUF1))
	{
		if(RCC->CSR & RCC_CSR_SFTRSTF || RCC->CSR & RCC_CSR_PINRSTF || RCC->CSR &  RCC_CSR_PWRRSTF)
		{
			enable5V();
			RCC->CSR |= RCC_CSR_RMVF;
		}
		stateEffects(&effects_ctx, event);
		setHue(&settings_ctx, &effects_ctx, event);
	}
	else
	{
		if((PWR->SR1 & PWR_SR1_WUF6))
		{
			if(event == EV_LONG2)
			{
				enable5V();
				GPIO_SET_PIN(PL1_PORT, PL1_PIN);
				PWR->SCR |= PWR_SCR_CWUF6;
			}
			else//if(event != EV_LONG2)
				indicatePowerLvl(getBatStatus());
		}
		if((PWR->SR1 & PWR_SR1_WUF1))
		{
			if(event == EV_SHORT)
			{
				enable5V();
				PWR->SCR |= PWR_SCR_CWUF1;
			}
			if(getPGstate())
			{
				goStandBy();
			}
		}

	}

}

void stateEffects(EFFEXTS_CTX *effects_ctx, BUTTON_EV event)
{
	if(event == EV_SHORT && effects_ctx->enableSettings == FALSE)
	{
		effects_ctx->state++;
		if(effects_ctx->state > (MAX_EFFECTS - 1))
			effects_ctx->state = ST_STATIC;
		effects_ctx->renew = TRUE;
	}
	if(event == EV_LONG2 && effects_ctx->enableSettings == FALSE)
	{
		GPIO_TogglePin(LED_PORT, LED_PIN);
		if(effects_ctx->state == ST_STATIC && effects_ctx->enableSettings == FALSE)
		{
			effects_ctx->enableSettings = TRUE;
			effects_ctx->enableStandBy = FALSE;
			GPIO_RESET_PIN(SL1_PORT, SL1_PIN); // on indicate settings regime
			GPIO_RESET_PIN(SL2_PORT, SL2_PIN); // on indicate set_hue enable parameter
		}
	}
	if(event == EV_LONG5 && effects_ctx->enableStandBy == TRUE)
	{
		goStandBy();
	}

	if(TIM16->SR & TIM_SR_UIF)
	{
		TIM16->SR &= ~TIM_SR_UIF;
		switch(effects_ctx->state)
		{
		case ST_STATIC:
			static_color(effects_ctx->hue);
			break;
		case ST_BREATH:
			breathing_effect_sinusoidal(effects_ctx->hue, MIN_BREATH_BR, MAX_BREATH_BR, effects_ctx->renew);
			break;
		case ST_SIN_WAVE:
			smooth_color_transition(effects_ctx->hue, effects_ctx->renew);
			break;
		default:
			break;
		}
		effects_ctx->renew = FALSE;
	}

}

void setHue(SETTINGS_CTX *settings_ctx, EFFEXTS_CTX *effects_ctx, BUTTON_EV event)
{
	static bool_logic button_pin_state;
	button_pin_state = !GPIO_GET_PIN_STATE(BUTTON_PORT, BUTTON_PIN);
	if(effects_ctx->enableSettings == TRUE) // вход в режим настройки цвета
	{

		if(event == EV_SHORT) // листаем параметры либо ST_SET_HUE либо EXIT
		{
			settings_ctx->state++;
			if(settings_ctx->state > ST_EXIT)
				settings_ctx->state = ST_SET_HUE;

			settings_ctx->enableSettingHue = FALSE;
		}

		switch(settings_ctx->state)
		{
		case ST_IDLE:
			if(button_pin_state == FALSE)
			{
				GPIO_TogglePin(LED_PORT, LED_PIN);
				settings_ctx->state = ST_SET_HUE;
			}
			if(button_pin_state == TRUE && event == EV_LONG5)
				goStandBy();
			break;
		case ST_SET_HUE:
			if(event == EV_LONG2 && settings_ctx->enableSettingHue == FALSE)
				settings_ctx->enableSettingHue = TRUE;

			if(settings_ctx->enableSettingHue == TRUE && button_pin_state == TRUE)
			{
				if( HAL_GetTick() - settings_ctx->timeCheck >= HUE_SETTING_TIM_DIVIDER)
				{
					settings_ctx->timeCheck = HAL_GetTick();
					GPIO_TogglePin(SL2_PORT, SL2_PIN);
					effects_ctx->hue += HUE_CHANGE_STEP;
					if(effects_ctx->hue > 360)
						effects_ctx->hue = 0;
				}
			}

			if(settings_ctx->enableSettingHue == TRUE && button_pin_state == FALSE)
			{
				settings_ctx->enableSettingHue = FALSE;
				GPIO_RESET_PIN(SL2_PORT, SL2_PIN); // on indicate set_hue enable parameter
				settings_ctx->timeCheck = 0;
			}

			break;
		case ST_EXIT:
			if(event == EV_LONG2)
			{
				eeWrite(effects_ctx->hue, HUE_ID);

				settings_ctx->state = ST_IDLE;
				settings_ctx->timeCheck = 0;
				settings_ctx->enableSettingHue = FALSE;
				effects_ctx->enableSettings = FALSE;
				effects_ctx->enableStandBy = TRUE;
		        GPIO_SET_PIN(SL1_PORT, SL1_PIN);
		        GPIO_SET_PIN(SL2_PORT, SL2_PIN);
			}
			break;
		default:
			break;
		}
		if(settings_ctx->state != settings_ctx->prev_state)
		{
		    switch(settings_ctx->state)
		    {
		    case ST_SET_HUE:
		        GPIO_RESET_PIN(SL2_PORT, SL2_PIN);
		        break;

		    case ST_EXIT:
		        GPIO_RESET_PIN(SL1_PORT, SL1_PIN);
		        GPIO_SET_PIN(SL2_PORT, SL2_PIN);
		        break;
		    default:
		    	break;
		    }

		    settings_ctx->prev_state = settings_ctx->state;
		}
	}

}


void goStandBy()
{
	clearPixel();
	disable5V();
	GPIO_SET_PIN(SL1_PORT, SL1_PIN);
	GPIO_SET_PIN(SL2_PORT, SL2_PIN);
	while(!GPIO_GET_PIN_STATE(BUTTON_PORT, BUTTON_PIN));
	HAL_Delay(100);

	RCC->APBENR1 |= RCC_APBENR1_PWREN;
	PWR->SCR |= PWR_SCR_CWUF; // clear flags

	PWR->CR3 |= PWR_CR3_APC; 		// APC = Apply Pull Configuration

	// for wake-up 1 PA0
	PWR->PUCRA |= PWR_PUCRA_PU0;	// pull-up  PA0
	PWR->PDCRA &= ~PWR_PDCRA_PD0;	// disable pull-down
//	PWR->PUCRA &= ~PWR_PUCRA_PU0;	// pull-up  PA0
//	PWR->PDCRA |= PWR_PDCRA_PD0;	// disable pull-down
	// for wake-up 6 PB5
	PWR->PUCRB |= PWR_PUCRB_PU5;	// pull-up  PB5
	PWR->PDCRB &= ~PWR_PDCRB_PD5;	// disable pull-down
	HAL_Delay(1);

//	PWR->CR4 &= ~PWR_CR4_WP1; // polarity for WKUP1 HIGH
//	PWR->CR4 &= ~PWR_CR4_WP6; // polarity for WKUP6 HIGH

	PWR->CR4 |= PWR_CR4_WP1; // polarity for WKUP1 LOW
	PWR->CR4 |= PWR_CR4_WP6; // polarity for WKUP6 LOW

	PWR->CR3 |= PWR_CR3_EWUP1; // enable WKUP1
	PWR->CR3 |= PWR_CR3_EWUP6; // enable WKUP6

	// config low power regime
	PWR->CR1 &= ~PWR_CR1_LPMS;
	PWR->CR1 |= PWR_CR1_LPMS_0 | PWR_CR1_LPMS_1;

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // enable standby
	__WFI(); // enter into standby
}



void Power_InitAfterStandby()
{
    RCC->APBENR1 |= RCC_APBENR1_PWREN;
    PWR->CR3 &= ~(PWR_CR3_EWUP1 | PWR_CR3_EWUP6);
    //PWR->SCR |= PWR_SCR_CWUF1 | PWR_SCR_CWUF6;
}

void indicateDischarge()
{
	bool_logic counter = 0;
	while(counter < 10)
	{
		GPIO_TogglePin(PL2_PORT, PL2_PIN);
		HAL_Delay(100);
		counter++;
	}
}

void checkPowerLogic(when_measure part_of_program)
{
	bat_ctx.checkCounter++;
	if(bat_ctx.checkCounter > bat_ctx.checkPeriod)
	{
		if(part_of_program == START)
			measureBattery_atStart(measureBattery());
		else
			measureBattery_whileRun(measureBattery());
		bat_ctx.checkCounter = 0;

		if(getBatStatus() == LOW_BATTERY)
		{
			bat_ctx.checkPeriod = CHECK_BAT_PERIOD_LOW_BATTERY; 		//checking period to 10 sec
			setBrightness(127);
		}
		else if(getBatStatus() == DISCHARGED)
		{
			clearPixel();
			indicateDischarge();
			goStandBy();
		}
		else
			bat_ctx.checkPeriod = CHECK_BAT_PERIOD_NORMAL;
	}


}

void indicatePowerLvl(BatStatus status)
{
	if(bat_ctx.enableIndication == TRUE)
	{
		switch(status)
		{
		case NORMAL:
			GPIO_RESET_PIN(PL1_PORT, PL1_PIN);
			//GPIO_TogglePin(PL1_PORT, PL1_PIN);
			break;
		case LOW_BATTERY:
			//GPIO_RESET_PIN(PL1_PORT, PL1_PIN);
			GPIO_TogglePin(PL1_PORT, PL1_PIN);
			break;
		default:
			break;
		}
	}
	bat_ctx.enableIndication = FALSE;
}

void indicatePowerLvl_PeriodCheck()
{
	static uint8_t timeIndication;
	bat_ctx.indicationCounter++;
	if(bat_ctx.indicationCounter > INDICATION_PERIOD)
	{
		bat_ctx.enableIndication = TRUE;
		timeIndication++;
		bat_ctx.indicationCounter = 0;
	}
	if(timeIndication > INDICATION_TIME)
		goStandBy();
}


void indicateCharger()
{
	// T = 10 ms
	static uint16_t counter = 0;

	static uint8_t current_pgState = 1, prev_pgState = 1;
	current_pgState = getPGstate();
	if(current_pgState == FALSE)
	{
		prev_pgState = current_pgState;
		setBrightness(BRIGHTNESS);
		process_BQ24070();
		counter++;
		switch(getState_BQ27070())
		{
		case PRECHARGE:
		case CHARGE:
			if(counter > 100)
			{
				counter = 0;
				GPIO_TogglePin(PL2_PORT, PL2_PIN);
			}
			break;
		case CHARGE_DONE:
			GPIO_RESET_PIN(PL1_PORT, PL1_PIN);
			counter = 0;
			break;
		case CHARGE_SUSPEND:
			if(counter > 20)
			{
					//chargeEnable();
				counter = 0;
				GPIO_TogglePin(PL1_PORT, PL1_PIN);
			}
			break;
		case CHARGE_OFF:
			GPIO_SET_PIN(PL1_PORT, PL1_PIN);
			GPIO_SET_PIN(PL2_PORT, PL2_PIN);
			break;
		default:
			break;
		}
	}
	else
	{
		if(current_pgState != prev_pgState)
		{
			prev_pgState = current_pgState;
			counter = 0;
			GPIO_SET_PIN(PL1_PORT, PL1_PIN);
			GPIO_SET_PIN(PL2_PORT, PL2_PIN);
		}
	}
}

void enable5V()
{
	GPIO_SET_PIN(EN_5V_PORT, EN_5V_PIN);
}
void disable5V()
{
	GPIO_RESET_PIN(EN_5V_PORT, EN_5V_PIN);
}
