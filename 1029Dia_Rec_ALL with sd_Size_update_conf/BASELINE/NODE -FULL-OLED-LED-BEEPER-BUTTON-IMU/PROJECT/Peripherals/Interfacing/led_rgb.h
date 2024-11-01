/**
 * @file led_rgb.h
 * @author SHUAIWEN CUI (SHUAIWEN001 AT e DOT ntu DOT edu DOT sg)
 * @brief This file is for RGB LED control function declaration.
 * @version 1.0
 * @date 2024-09-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _LED_RGB_H_
#define _LED_RGB_H_

#include "stm32h7xx_hal.h" // HAL library file declaration, replace it with the corresponding file according to the actual situation
#include "main.h" // IO definition and initialization function are in the main.c file, must be referenced

void LED_RGB(int LED_R, int LED_G, int LED_B);
void LED_RGB_Toggle(void); 

#endif /* _LED_RGB_H_ */
