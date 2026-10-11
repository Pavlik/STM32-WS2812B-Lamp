/*
 * powerLogic.c
 *
 *  Created on: Mar 15, 2026
 *      Author: Pavel
 */

#include "powerLogic.h"

static BatStatus currentStatus;
//BatStatus previousStatus;

uint16_t measureBattery()
{
	enableBatMeasure();
	uint16_t current_bat_voltage = readVoltage(BAT_ADC_CHANNEL) * RES_DIVIDER_COEF;
//	setBatStatus(current_bat_voltage);
	disableBatMeasure(0);
	return current_bat_voltage;
}

void measureBattery_atStart(uint16_t voltage_atStart)
{
	setBatStatus(voltage_atStart - DROP);
}

void measureBattery_whileRun(uint16_t voltage_whileRun)
{
	setBatStatus(voltage_whileRun);
}

//void measureBattery()
//{
//	enableBatMeasure();
//	uint16_t current_bat_voltage = readVoltage(BAT_ADC_CHANNEL) * RES_DIVIDER_COEF;
//	setBatStatus(current_bat_voltage);
//	disableBatMeasure(0);
//}

void setBatStatus(uint16_t voltage)
{
    switch(currentStatus)
    {
    case NORMAL:
        if(voltage <= LOW_BATTERY_THRESHOLD)  // Сначала проверяем самый низкий уровень
        {
            currentStatus = DISCHARGED;
        }
        else if(voltage <= NORMAL_THRESHOLD)  // Затем средний
        {
            currentStatus = LOW_BATTERY;
        }
        break;

    case LOW_BATTERY:
        if (voltage >= (NORMAL_THRESHOLD + HYSTERESIS))
        {
            currentStatus = NORMAL;
        }
        else if (voltage <= LOW_BATTERY_THRESHOLD)
        {
            currentStatus = DISCHARGED;
        }
        break;

    case DISCHARGED:
        if (voltage >= (LOW_BATTERY_THRESHOLD + HYSTERESIS))
        {
            currentStatus = LOW_BATTERY;
        }
        break;
    }
}

BatStatus getBatStatus()
{
	return currentStatus;
}

void enableBatMeasure()
{
	GPIO_SET_PIN(EN_BAT_MEASURE_PORT, EN_BAT_MEASURE_PIN);
}
void disableBatMeasure()
{
	GPIO_RESET_PIN(EN_BAT_MEASURE_PORT, EN_BAT_MEASURE_PIN);
}



