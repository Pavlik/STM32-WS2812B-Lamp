/*
 * myPixel.h
 *
 *  Created on: Feb 24, 2026
 *      Author: Navumchyk
 */

#ifndef INC_MYPIXEL_H_
#define INC_MYPIXEL_H_


#include "stm32g030xx.h"

#define BR 				255
#define USE_GAMMA       0

#define PIXEL_DEL_LEN	50
#define PIXEL_NUM 		4
#define BUF_LEN 		(PIXEL_DEL_LEN + PIXEL_NUM * 24)

#define PIXEL_LOW 	9 	//~378 ns
#define PIXEL_HIGH 	22 	//~924 ns

#define pixel_is_set(reg, bit) (reg & (1 << (bit)))

void setBrightness(uint8_t br);
uint8_t correctGamma(uint8_t color);
uint8_t getScaledValue(uint8_t value);
void setPixel(uint8_t pixel_R, uint8_t pixel_G, uint8_t pixel_B, uint8_t pixel_position);
void startDMA(const uint16_t *pData, uint16_t length);
void updatePixel();
void clearPixel();




#endif /* INC_MYPIXEL_H_ */
