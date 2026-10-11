/*
 * myPixel.c
 *
 *  Created on: Feb 24, 2026
 *      Author: Navumchyk
 */
#include "myPixel.h"

static 	uint16_t buff_pixel_array[BUF_LEN] = {0};
static uint8_t brightness = 255;
const uint8_t gamma28[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};



uint8_t correctGamma(uint8_t color)
{
    return gamma28[color];
}
void setBrightness(uint8_t br)
{
	brightness = br;
}


uint8_t getScaledValue(uint8_t value)
{
	value = (value * (uint16_t)brightness+127) / 255;
	return value;
}

void setPixel(uint8_t pixel_R, uint8_t pixel_G, uint8_t pixel_B, uint8_t pixel_position)
{

#if USE_GAMMA
	pixel_R = correctGamma(pixel_R);
	pixel_G = correctGamma(pixel_G);
	pixel_B = correctGamma(pixel_B);
#endif

	pixel_R = getScaledValue(pixel_R);
	pixel_G = getScaledValue(pixel_G);
	pixel_B = getScaledValue(pixel_B);

	for(int i = 0; i < 8; i++)
	{
		if(pixel_G & (1 << (7-i)))
		//if(pixel_is_set(pixel_G, 7-i) == 1)
			buff_pixel_array[PIXEL_DEL_LEN + pixel_position * 24 + i] = PIXEL_HIGH;
		else
			buff_pixel_array[PIXEL_DEL_LEN + pixel_position * 24 + i] = PIXEL_LOW;
		if(pixel_R & (1 << (7-i)))
			buff_pixel_array[PIXEL_DEL_LEN + pixel_position * 24 + i+8] = PIXEL_HIGH;
		else
			buff_pixel_array[PIXEL_DEL_LEN + pixel_position * 24 + i+8] = PIXEL_LOW;
		if(pixel_B & (1 << (7-i)))
			buff_pixel_array[PIXEL_DEL_LEN + pixel_position * 24 + i+16] = PIXEL_HIGH;
		else
			buff_pixel_array[PIXEL_DEL_LEN + pixel_position * 24 + i+16] = PIXEL_LOW;
	}

}


void startDMA(const uint16_t *pData, uint16_t length)
{
	DMA1_Channel1->CCR &= ~DMA_CCR_EN;
	DMA1_Channel1->CMAR = (uint32_t)pData;
	DMA1_Channel1->CNDTR = length;
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	TIM3->CR1 |= TIM_CR1_CEN;
}

void updatePixel()
{
	startDMA((uint16_t*)&buff_pixel_array, BUF_LEN);
}

void clearPixel()
{
	for(uint8_t i = 0; i < PIXEL_NUM; i++)
	{
		setPixel(0, 0, 0, i);
	}
	updatePixel();
}



