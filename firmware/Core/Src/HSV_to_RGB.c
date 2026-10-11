/*
 * HSV_to_RGB.c
 *
 *  Created on: Aug 22, 2025
 *      Author: Pavel
 */

#include "HSV_to_RGB.h"

RGB_Color hsv_to_rgb(uint16_t h, uint8_t s_percent, uint8_t v_percent)

{
    RGB_Color rgb = { 0, 0, 0 };

    uint8_t s = (s_percent * 255) / 100;
    uint8_t v = (v_percent * 255) / 100;

    //Проверка граничных случаев
    if (s == 0)
    {
        // Нет насыщенности - оттенки серого
        rgb.r = rgb.g = rgb.b = v;
        return rgb;
    }

    // Нет яркости - черный цвет
    if (v == 0) {
        rgb.r = rgb.g = rgb.b = 0;
        return rgb;
    }

    // Нормализуем H к диапазону 0-359
    h = h % 360;

    // Определяем сектор и дробную част
    uint8_t sector = h / 60; // 0-5
    uint16_t fractional = (h % 60) * 256 / 60; // 0-255 для точности

    // Промежуточные значения (используем 8-битные операции)
    uint16_t p = (v * (256 - s)) >> 8; // Эквивалентно (v * (255 - s)) / 255

        // Вычисляем q и t с сохранением точности
        uint16_t q = (v * (256 - ((s * fractional) >> 8))) >> 8;
        uint16_t t = (v * (256 - ((s * (256 - fractional)) >> 8))) >> 8;

    // Приводим к uint8_t
    uint8_t p8 = (uint8_t)p;
    uint8_t q8 = (uint8_t)q;
    uint8_t t8 = (uint8_t)t;

    switch (sector) {
    case 0: rgb.r = v; rgb.g = t8; rgb.b = p8; break;
    case 1: rgb.r = q8; rgb.g = v; rgb.b = p8; break;
    case 2: rgb.r = p8; rgb.g = v; rgb.b = t8; break;
    case 3: rgb.r = p8; rgb.g = q8; rgb.b = v; break;
    case 4: rgb.r = t8; rgb.g = p8; rgb.b = v; break;
    case 5: rgb.r = v; rgb.g = p8; rgb.b = q8; break;
    }

    return rgb;
}
