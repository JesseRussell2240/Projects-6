/*
 * CAN.c
 *
 *  Created on: Jul 29, 2025
 *      Author: hayde
 */

#include "CAN.h"
#include "main.h"
#include "stdint.h"

uint8_t OBD2_NUMBYTES_POS = 0;
uint8_t OBD2_MODE_POS     = 1;
uint8_t OBD2_PID_POS      = 2;
uint8_t OBD2_DATA_POS     = 3;
uint8_t OBD2_DATA_SIZE    = 4;
uint8_t OBD2_DATA_LENGTH  = 8;

extern CAN_HandleTypeDef hcan;
extern CAN_TxHeaderTypeDef TxHeader;
extern uint32_t TxMailbox;
extern uint8_t TxData[8];
extern uint8_t msgBuffer[8];

void writeOBD2(uint8_t* msg, uint8_t len, uint8_t mode, uint8_t pid, uint8_t* data)
{
	msg[OBD2_NUMBYTES_POS] = len;
	msg[OBD2_MODE_POS]     = mode;
	msg[OBD2_PID_POS]      = pid;

	for (int i = 0; i < OBD2_DATA_SIZE; i++)
		msg[OBD2_DATA_POS + i] = data[i];
}

void loadOBD2(uint8_t* dest, const uint8_t* msg)
{
	for (int i = 0; i < OBD2_DATA_LENGTH; i++)
		dest[i] = msg[i];
}

void writeOBD2Temp(float temperature, uint8_t pid)
{
    uint8_t intPart = (uint8_t)temperature;
    uint8_t decPart = (uint8_t)((temperature - intPart) * 100.0f);

    uint8_t data[4] = {0, 0, intPart, decPart};

    writeOBD2(msgBuffer, 6, 0x01, pid, data);  // Mode 0x01 for current data
    loadOBD2(TxData, msgBuffer);

    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
}

