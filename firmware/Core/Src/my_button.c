/*
 * my_button.c
 *
 *  Created on: Feb 21, 2026
 *      Author: Pavel
 */


#include "my_button.h"

void button_init(BUTTON *btn, GPIO_TypeDef *port_init, uint16_t pin_init)
{

	btn->port = port_init;
	btn->pin = 1 << pin_init;
	btn->state = BTN_RELEASED;
	btn->press_ticks = 0;
	btn->debounce_ticks = 0;
	btn->button_lvl = 0;
	btn->events = BTN_EVENT_NONE;
}

void button_process(BUTTON *btn)
{

  		btn->button_lvl = !(btn->port->IDR & btn->pin);
  		switch(btn->state)
  		{
  		case BTN_RELEASED:
            if(btn->button_lvl)
            {
            	btn->debounce_ticks = 0;
            	btn->state = BTN_DEBOUNCE_PRESS;
            }
  			break;
  		case BTN_DEBOUNCE_PRESS:
  		  if(btn->button_lvl)
  		  {
  			  if(++btn->debounce_ticks == DEBOUNCE_TICKS)
  			  {
  				btn->press_ticks = 0;
  				btn->state = BTN_PRESSED;
  			  }
  		  }
  		  else
  		  {
  			btn->state = BTN_RELEASED;
  		  }
  			break;
  		case BTN_PRESSED:
  		  if(btn->button_lvl)
  		  {
  			btn->press_ticks++;
  			  if(btn->press_ticks == LONG2_TICKS)
  			  {
  				  btn->events = BTN_EVENT_LONG2;
  			  }
  			  if(btn->press_ticks == LONG5_TICKS)
  			  {
  				btn->events = BTN_EVENT_LONG5;
  			  }
  		  }
  		  else
  		  {
  			  btn->debounce_ticks = 0;
  			  btn->state = BTN_DEBOUNCE_RELEASE;
  		  }
  			break;
  		case BTN_DEBOUNCE_RELEASE:
  		  if(!btn->button_lvl)
  		  {
  			  if(++btn->debounce_ticks == DEBOUNCE_TICKS)
  			  {
  			      if(btn->press_ticks < LONG2_TICKS)
  			      {
  			    	btn->events = BTN_EVENT_SHORT;
  			      }
  	  			btn->state = BTN_RELEASED;
  			  }
  		  }
  		  else
  		  {
  			btn->press_ticks = 0;
    		btn->state = BTN_PRESSED;
  		  }
  			break;
  		default:
  			break;
  		}
}

uint8_t get_button_status(BUTTON *btn)
{
	  if(btn->events == BTN_EVENT_SHORT)
	  {
		btn->events = BTN_EVENT_NONE;
		return BTN_EVENT_SHORT;
	  }
	  if(btn->events == BTN_EVENT_LONG2)
	  {
		  btn->events = BTN_EVENT_NONE;
		  return BTN_EVENT_LONG2;
	  }
	  if(btn->events == BTN_EVENT_LONG5)
	  {
		  btn->events = BTN_EVENT_NONE;
		  return BTN_EVENT_LONG5;
	  }
	  return BTN_EVENT_NONE;
}

