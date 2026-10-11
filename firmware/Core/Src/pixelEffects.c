/*
 * pixelEffects.c
 *
 *  Created on: Mar 11, 2026
 *      Author: Pavel
 */

#include "pixelEffects.h"

void static_color(uint16_t hue)
{
	RGB_Color color = hsv_to_rgb(hue, 100, 100);
    for (uint16_t i = 0; i < PIXEL_NUM; i++) {
    	setPixel(color.r, color.g, color.b, i);
    }
    updatePixel();
}

void breathing_effect_sinusoidal(uint16_t hue,uint8_t min_brightness, uint8_t max_brightness, uint8_t reset)
//void breathing_effect_sinusoidal(uint16_t hue,uint8_t min_brightness, uint8_t max_brightness, uint8_t speed, uint8_t reset)
{
    static uint16_t breath_index = 64;
    static uint8_t first_run = 1;

    if (reset || first_run)
    {
        breath_index = 128;
        first_run = 0;
    }

    // Преобразование скорости дыхания 1-10 → 1-5
    //uint8_t actual_breath_speed = (speed + 1) / 2;

    // Дыхание
    uint8_t phase = breath_index & 0xFF;
    uint8_t lookup_index = (phase < 128) ? phase : (255 - phase);
    uint8_t table_val = breath_half_sine_table[lookup_index];

    uint8_t brightness = min_brightness + (table_val * (max_brightness - min_brightness)) / 255;

    RGB_Color color = hsv_to_rgb(hue, 100, brightness);

    for (uint16_t i = 0; i < PIXEL_NUM; i++) {
    	setPixel(color.r, color.g, color.b, i);
    }
    updatePixel();
    breath_index += 1;
    //breath_index += actual_breath_speed;
}

//void smooth_color_transition(uint16_t hue, uint8_t reset)
//{
//    static uint16_t nhue = 0;
//    static uint16_t frac = 0;
//    const uint16_t STEP = 51; // Шаг в fixed-point (примерно 0.066 градуса за кадр)
//    // 17/256 ≈ 0.066, полный круг 360° займет ~5400 кадров при 60fps = 90 секунд
//
//    if(reset)
//    {
//        nhue = hue;
//        frac = 0;
//    }
//
//    // Добавляем шаг к дробной части
//    frac += STEP;
//
//    // Переносим целую часть при переполнении
//    if (frac >= 256) {
//        nhue += frac >> 8;  // Добавляем целую часть от frac
//        frac &= 0xFF;       // Оставляем только дробную часть
//
//        if (nhue >= 360) {
//            nhue %= 360;
//        }
//    }
//
//    RGB_Color color = hsv_to_rgb(nhue, 100, 100);
//
//    for (uint16_t i = 0; i < PIXEL_NUM; i++) {
//        setPixel(color.r, color.g, color.b, i);
//    }
//    updatePixel();
//}

void smooth_color_transition(uint16_t hue, uint8_t reset)
{
    static float nhue = 0;

    if(reset)
    {
        nhue = (float)hue;
    }

    nhue += 0.2f; // Медленное изменение
    if (nhue >= 360.0f) {
        nhue -= 360.0f;
    }

    // Получаем два соседних цвета для интерполяции
    uint16_t hue_int = (uint16_t)nhue;
    float fraction = nhue - (float)hue_int; // Дробная часть (0-0.999)

    uint16_t hue_next = (hue_int + 1) % 360;

    // Получаем два цвета
    RGB_Color color1 = hsv_to_rgb(hue_int, 100, 100);
    RGB_Color color2 = hsv_to_rgb(hue_next, 100, 100);

    // Интерполируем между ними
    RGB_Color color;
    color.r = (uint8_t)((float)color1.r * (1.0f - fraction) + (float)color2.r * fraction);
    color.g = (uint8_t)((float)color1.g * (1.0f - fraction) + (float)color2.g * fraction);
    color.b = (uint8_t)((float)color1.b * (1.0f - fraction) + (float)color2.b * fraction);

    for (uint16_t i = 0; i < PIXEL_NUM; i++) {
        setPixel(color.r, color.g, color.b, i);
    }
    updatePixel();
}
