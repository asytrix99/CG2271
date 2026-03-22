/*
 * adc.c
 *
 *  Created on: 22 Mar 2026
 *      Author: andre
 */

#include "adc.h"
// ADC for the soil moisture sensor readings
void ADC_Init()
{
    NVIC_DisableIRQ(ADC0_IRQn);

    // Enable ADC + port
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

    // Set pins to analog mode
    PORTE->PCR[ADC_SE0_PIN] = PORT_PCR_MUX(0);

    // Enable interrupt
    ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;

    // 12-bit resolution
    ADC0->CFG1 |= ADC_CFG1_MODE(1);

    // Software trigger
    ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;

    // Disable averaging & continuous mode
    ADC0->SC3 &= ~ADC_SC3_AVGE_MASK;
    ADC0->SC3 &= ~ADC_SC3_ADCO_MASK;

    NVIC_SetPriority(ADC0_IRQn, 1);
    NVIC_EnableIRQ(ADC0_IRQn);
}

void ADC_Start(uint8_t channel) {
	ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK;
	ADC0->SC1[0] |= ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(channel);
}

//ISR - saves result, posts to queue
void ADC0_IRQHandler()
{
	//clear pending IRQ
	NVIC_ClearPendingIRQ(ADC0_IRQn);

	// if conversion complete, result[turn] = result register
    if (ADC0->SC1[0] & ADC_SC1_COCO_MASK)
    {
    	int moistureVal = ADC0->R[0];
    	BaseType_t hpw = pdFALSE;
    	xQueueSendFromISR(sensorQueue, &moistureVal, &hpw);
    	portYIELD_FROM_ISR(hpw);
    }
}
