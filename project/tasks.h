/*
 * tasks.h
 *
 *  Created on: 22 Mar 2026
 *      Author: andre
 */

#ifndef TASKS_H_
#define TASKS_H_

#include "globals.h"
#include "adc.h"

void soilMoisturePollingTask(void *pvParams);
void alertTask(void* pvParams);

#endif /* TASKS_H_ */
