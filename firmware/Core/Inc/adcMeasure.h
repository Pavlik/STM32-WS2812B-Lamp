/*
 * adcMesure.h
 *
 *  Created on: Mar 12, 2026
 *      Author: Navumchyk
 */

#ifndef SRC_ADCMESURE_H_
#define SRC_ADCMESURE_H_

#include <stdio.h>
#include "stm32g030xx.h"

#define VREFINT_CAL_ADDR  ((uint16_t*)0x1FFF75AA)
#define VREFINT_CHANNEL 13
#define RES_DIVIDER_COEF 2

uint16_t readADC(uint8_t channel);
uint32_t getVDD();
uint32_t readVoltage(uint8_t channel);


#endif /* SRC_ADCMESURE_H_ */
