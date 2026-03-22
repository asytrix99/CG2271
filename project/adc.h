/*
 * adc.h
 *
 *  Created on: 22 Mar 2026
 *      Author: andre
 */

#ifndef ADC_H_
#define ADC_H_

#include "globals.h"
void ADC_Init();
void ADC_Start(uint8_t channel);
void ADC0_IRQHandler();

#endif /* ADC_H_ */
