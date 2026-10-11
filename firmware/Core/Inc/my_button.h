/*
 * my_button.h
 *
 *  Created on: Feb 20, 2026
 *      Author: Pavel
 */

#ifndef INC_MY_BUTTON_H_
#define INC_MY_BUTTON_H_

/*VALID FOR SYSTICKS AT 24 Mhz. TIM PSC = 24000, ARR = 50 */
#define DEBOUNCE_TICKS   5     // 50 мс
#define LONG2_TICKS      200    // 2 сек
#define LONG5_TICKS      500   // 5 сек

#include "stm32g030xx.h"

typedef enum
{
    BTN_EVENT_NONE   = 0,
    BTN_EVENT_SHORT,
    BTN_EVENT_LONG2,
    BTN_EVENT_LONG5
} btn_event_t;

typedef enum
{
	BTN_RELEASED,
    BTN_DEBOUNCE_PRESS,
    BTN_PRESSED,
    BTN_DEBOUNCE_RELEASE
} btn_fsm_state;

typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;

	btn_fsm_state state;

	uint16_t press_ticks;
	uint8_t debounce_ticks;
	uint8_t button_lvl;

	uint8_t events;

}BUTTON;

void button_init(BUTTON *btn, GPIO_TypeDef *port_init, uint16_t pin_init);
void button_process(BUTTON *btn);
uint8_t get_button_status(BUTTON *btn);


#endif /* INC_MY_BUTTON_H_ */
