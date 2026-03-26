/*
 * uart.h
 *
 *  Created on: 22 Mar 2026
 *      Author: andre
 */

#ifndef UART_H_
#define UART_H_

#include "globals.h"

void initUART2(uint32_t baud_rate);
void UART2_FLEXIO_IRQHandler(void);
void uartTxTask(void *pvParams);
void uartRxTask(void *pvParams);

#endif /* UART_H_ */
