/*
 * pixel_effects.h
 *
 *  Created on: Mar 11, 2026
 *      Author: Pavel
 */

#ifndef INC_PIXELEFFECTS_H_
#define INC_PIXELEFFECTS_H_

#include <stdio.h>
#include "myPixel.h"
#include "HSV_to_RGB.h"

// Таблица только для 0-180 градусов (128 значений)
static const uint8_t breath_half_sine_table[128] = {
    0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
    32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62,
    64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
    96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126,
    128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158,
    160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
    192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222,
    224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254
};
void static_color(uint16_t hue);
void breathing_effect_sinusoidal(uint16_t hue,uint8_t min_brightness, uint8_t max_brightness, uint8_t reset);
void smooth_color_transition(uint16_t hue, uint8_t reset);

#endif /* INC_PIXELEFFECTS_H_ */
