/*
 * HSV_to_RGB.h
 *
 *  Created on: Aug 22, 2025
 *      Author: Pavel
 */

#ifndef INC_HSV_TO_RGB_H_
#define INC_HSV_TO_RGB_H_

#include <stdint.h>

// Диапазоны входных значений (можно менять под свои нужды)
#define H_MAX 360    // Оттенок: 0-360 градусов
#define S_MAX 100    // Насыщенность: 0-100%
#define V_MAX 100    // Яркость: 0-100%

// Диапазон выходных значений
#define RGB_MAX 255     // RGB: 0-255

// Структура для хранения RGB цвета
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB_Color;

RGB_Color hsv_to_rgb(uint16_t h, uint8_t s_percent, uint8_t v_percent);



#endif /* INC_HSV_TO_RGB_H_ */
