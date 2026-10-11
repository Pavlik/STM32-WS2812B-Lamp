/*
 * bq24070.c
 *
 *  Created on: Mar 15, 2026
 *      Author: Pavel
 */
#include "bq24070.h"
static charge_state state;
static uint8_t power_good, stat1, stat2;

void configBQ24070()
{
	/* CHARGER configuration START */

	//gpio port A clock is active else enable gpio clock

	//PA5  as input PULL UP
	//POWER GOOD (/PG) open drain
	GPIOA->MODER &= ~GPIO_MODER_MODE5; // 00 - input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;  // reset pupdr
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD5_0; // pull_up

	//PA1 as input PULL UP
	//STAT1 open drain
	GPIOA->MODER &= ~GPIO_MODER_MODE1; // 00 - input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;  // reset pupdr
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_0; // pull_up

	//PA2 as input PULL UP
	//STAT2 open drain
	GPIOA->MODER &= ~GPIO_MODER_MODE2; // 00 - input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;  // reset pupdr
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0; // pull_up

	// PIN PA4 as output for enable/disable charging
	GPIOA->MODER &= ~GPIO_MODER_MODE4; // reset MODER for PA4
	GPIOA->MODER |= GPIO_MODER_MODE4_0; // PA4 as output 01
	CE_PORT->BSRR = 1 << CE_PIN; // enable at start

	/* CHARGER configuration END */
}

void process_BQ24070()
{

	power_good = (PG_PORT->IDR >> PG_PIN) & 1;
	stat1 = (STAT1_PORT->IDR >> STAT1_PIN) & 1;
	stat2 = (STAT2_PORT->IDR >> STAT2_PIN) & 1;

	if(!power_good)
	{
		if(!stat1 && !stat2)
			state = PRECHARGE;
		else if(!stat1 && stat2)
			state = CHARGE;
		else if(stat1 && !stat2)
			state = CHARGE_DONE;
		else
			state = CHARGE_SUSPEND;
	}
	else
		state = CHARGE_OFF;
}

void chargeEnable()
{
	CE_PORT->BSRR = 1 << CE_PIN;
}

void chargeDisable()
{
	CE_PORT->BSRR = 1 << (CE_PIN + 16);
}

uint8_t getPGstate()
{
	return (PG_PORT->IDR >> PG_PIN) & 1;
}

charge_state getState_BQ27070()
{
	return state;
}

