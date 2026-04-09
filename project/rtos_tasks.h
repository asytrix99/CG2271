/*
 * tasks.h
 *
 *  Created on: 22 Mar 2026
 *      Author: andre
 */

#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include "globals.h"
#include "adc.h"

void soilMoisturePollingTask(void *pvParams);
void alertTask(void *pvParams);
extern SemaphoreHandle_t buttonSemaphore;
void manualWaterTask(void *pvParams);
#endif /* TASKS_H_ */
