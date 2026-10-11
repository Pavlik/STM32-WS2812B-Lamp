/*
 * engine.h
 *
 *  Created on: Mar 29, 2026
 *      Author: Pavel
 */

#ifndef INC_ENGINE_H_
#define INC_ENGINE_H_

#include "stm32g0xx_hal.h"
#include "stm32g030xx.h"
#include "myPixel.h"
#include "pixelEffects.h"
#include "my_button.h"
#include "eeprom.h"
#include "powerLogic.h"
#include "bq24070.h"


#define GPIO_SET_PIN(port, pin)   ((port)->BSRR = (1UL << (pin)))
#define GPIO_RESET_PIN(port, pin) ((port)->BSRR = (1UL << ((pin) + 16)))
#define GPIO_GET_PIN_STATE(port, pin) ((port->IDR >> pin) & 1)

static inline void GPIO_TogglePin(GPIO_TypeDef *port, uint8_t pin)
{
    if(port->IDR & (1UL << pin))
        port->BSRR = (1UL << (pin + 16));
    else
        port->BSRR = (1UL << pin);
}
//static inline void gpio_reset(GPIO_TypeDef *port, uint32_t pin)
//{
//    port->BSRR = (1UL << (pin + 16));
//}

#define BRIGHTNESS 255

#define LED_PORT GPIOC
#define LED_PIN 14
#define BUTTON_PORT GPIOB
#define BUTTON_PIN 5

// Signal LED
#define SL1_PORT GPIOA
#define SL1_PIN 11
#define SL2_PORT GPIOA
#define SL2_PIN 10

// Power LED
#define PL1_PORT GPIOB
#define PL1_PIN 8
#define PL2_PORT GPIOB
#define PL2_PIN 7
#define INDICATION_PERIOD 50 // uSec
#define INDICATION_TIME 10 //sec

#define EN_5V_PORT GPIOB
#define EN_5V_PIN 9



//menu parameters
#define MENU_ITEMS    2
#define SETTING_ITEMS 1
#define MIN_BREATH_BR 0 	// %
#define MAX_BREATH_BR 100 	// %

// setting parameters
#define HUE_SETTING_TIM_DIVIDER 200 // ~200ms since TIM16 makes ~59Hz than ~200ms is 13 periods
#define HUE_CHANGE_STEP 2

typedef enum
{
	FALSE = 0,
	TRUE
}bool_logic;

typedef enum
{
	HUE_ID = 0,
	BAT_STATE_ID
}EE_ID;

typedef enum
{
	EV_NONE = 0,
	EV_SHORT,
	EV_LONG2,
	EV_LONG5
}BUTTON_EV;

#define MAX_EFFECTS 3
typedef enum
{
	ST_STATIC = 0,
	ST_BREATH,
	ST_SIN_WAVE,
}effects_state;

typedef struct
{
	effects_state state;
	uint16_t hue;
	bool_logic renew;
	bool_logic enableSettings;
	bool_logic enableStandBy;
}EFFEXTS_CTX;

typedef enum
{
	ST_IDLE = 0,
	ST_SET_HUE,
	ST_EXIT
}SETTINGS_STATE;

typedef struct
{
	SETTINGS_STATE state;
	SETTINGS_STATE prev_state;
	uint32_t timeCheck;
	bool_logic enableSettingHue;
}SETTINGS_CTX;

typedef enum
{
	STATIC_COLOR = 0,
	BREATH,
	SIN_WAVE
}menu_list;

typedef enum
{
	SETTING_PARAMETER = 0,
	EXIT
}settings_list;

typedef struct
{
	uint16_t checkPeriod;
	uint16_t checkCounter;
	BatStatus status;
	bool_logic enableMeasure;
	bool_logic enableIndication;
	uint16_t indicationCounter;
	bool_logic isCharging;
}BATTERY_CTX;

typedef enum
{
	START = 0,
	RUN
}when_measure;

void enable5V();
void disable5V();


void Engine_Init();
void Engine_Config();
void Engine_Main();

void stateEffects(EFFEXTS_CTX *effects_ctx, BUTTON_EV event);
void setHue(SETTINGS_CTX *settings_ctx, EFFEXTS_CTX *effects_ctx, BUTTON_EV event);

void goStandBy();
void Power_InitAfterStandby();

void indicateDischarge();
void checkPowerLogic(when_measure part_of_program);
void indicatePowerLvl(BatStatus status);
void indicatePowerLvl_PeriodCheck();
void indicateCharger();



#endif /* INC_ENGINE_H_ */
