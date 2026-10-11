/*
 * adcMesure.c
 *
 *  Created on: Mar 12, 2026
 *      Author: Navumchyk
 */


#include "adcMeasure.h"

uint16_t readADC(uint8_t channel)
{
    ADC1->CHSELR = (1 << channel);
    ADC1->CR |= ADC_CR_ADSTART;
    while(!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}

uint32_t getVDD()
{
	uint16_t adc_vref = readADC(VREFINT_CHANNEL); // 13- канал VREFINT
	uint16_t vref_cal = *VREFINT_CAL_ADDR;
	uint32_t vdda = 3000UL * vref_cal / adc_vref;
	return vdda; // mV
}

uint32_t readVoltage(uint8_t channel)
{
    uint32_t vdda = getVDD();
    uint16_t adc = readADC(channel);
    uint32_t voltage = adc * vdda / 4095;
    return voltage; // mV
}


