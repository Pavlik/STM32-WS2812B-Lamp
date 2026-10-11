/*
 * powerLogic.h
 *
 *  Created on: Mar 15, 2026
 *      Author: Pavel
 */

#ifndef INC_POWERLOGIC_H_
#define INC_POWERLOGIC_H_

#include "stm32g030xx.h"
#include "adcMeasure.h"

#define EN_BAT_MEASURE_PORT GPIOA
#define EN_BAT_MEASURE_PIN 3

#define CHECK_BAT_PERIOD_AT_START 		0
#define CHECK_BAT_PERIOD_NORMAL 		6000 // 6000 600
#define CHECK_BAT_PERIOD_LOW_BATTERY 	600
#define BAT_ADC_CHANNEL 				7
#define RES_DIVIDER_COEF 				2

/*All in mVolts*/
#define NORMAL_THRESHOLD 		3400
#define LOW_BATTERY_THRESHOLD 	3200
#define HYSTERESIS  			50

#define DROP 250 // mV

#define GPIO_SET_PIN(port, pin)   ((port)->BSRR = (1UL << (pin)))
#define GPIO_RESET_PIN(port, pin) ((port)->BSRR = (1UL << ((pin) + 16)))

typedef enum
{
    NORMAL,
    LOW_BATTERY,
    DISCHARGED
}BatStatus;

uint16_t measureBattery();
void measureBattery_atStart(uint16_t voltage_atStart);
void measureBattery_whileRun(uint16_t voltage_whileRun);
//void measureBattery();
void setBatStatus(uint16_t voltage);
BatStatus getBatStatus();
void enableBatMeasure();
void disableBatMeasure();

#endif /* INC_POWERLOGIC_H_ */
