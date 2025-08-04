/*
 * TempSensor.h
 *
 *  Created on: Jun 4, 2025
 *      Author: hayden
 */

#ifndef INC_TEMPSENSOR_H_
#define INC_TEMPSENSOR_H_

#include <stdint.h>

uint8_t DS18B20_Init(void);
void DS18B20_SampleTemp(void);
float DS18B20_ReadTemp(void);
void DS18B20_ReadROM(uint8_t *rom);
void DS18B20_SetROM(uint8_t index, uint8_t *romCode);
void DS18B20_ReadTempIndexed(uint8_t index, float *temperature);


#endif /* INC_TEMPSENSOR_H_ */
