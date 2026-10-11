/*
 * bq24070.h
 *
 *  Created on: Mar 15, 2026
 *      Author: Pavel
 */

#ifndef INC_BQ24070_H_
#define INC_BQ24070_H_

#include "stm32g030xx.h"

#define STAT1_PORT GPIOA
#define STAT1_PIN 1
#define STAT2_PORT GPIOA
#define STAT2_PIN 2

#define CE_PORT GPIOA
#define CE_PIN 4

#define PG_PORT GPIOA
#define PG_PIN 5



typedef enum
{
	PRECHARGE = 1,
	CHARGE,
	CHARGE_DONE,
	CHARGE_SUSPEND,
	CHARGE_OFF
}charge_state;

void configBQ24070();
void process_BQ24070();
void chargeEnable();
void chargeDisable();
uint8_t getPGstate();
charge_state getState_BQ27070();



#endif /* INC_BQ24070_H_ */
