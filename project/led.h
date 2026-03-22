/*
 * led.h
 *
 *  Created on: 22 Mar 2026
 *      Author: andre
 */

#ifndef LED_H_
#define LED_H_

#include "globals.h"

void GPIO_Init(void);
void LED_On(int pin);
void LED_Off(int pin);
void ledControlTask(void *pvParams);

#endif /* LED_H_ */
