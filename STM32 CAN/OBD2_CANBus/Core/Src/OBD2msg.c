/*
 * OBD2msg.c
 *
 *  Created on: May 20, 2025
 *      Author: Melona
 */

#include "OBD2msg.h"
#include "stdio.h"
#include "stdint.h"

uint8_t OBD2_DATA_LENGTH = 8;
uint8_t OBD2_NUMBYTES_POS = 0;
uint8_t OBD2_MODE_POS = 1;
uint8_t OBD2_PID_POS = 2;
uint8_t OBD2_DATA_POS = 3;
uint8_t OBD2_DATA_SIZE = 4;

/*
 * void readOBD2()
 * Purpose: read the obd2 message stored in 'msg'
 */
void readOBD2(uint8_t* msg)
{
	// Copy the register into a temporary buffer
	uint8_t dataBuffer[8];
	for (int i = 0; i < OBD2_DATA_LENGTH; i++)
		dataBuffer[i] = msg[i];

	printf("|--- Reading OBD2 Message... ---|\n");
	printf("Length:\t0x%02X\n", dataBuffer[OBD2_NUMBYTES_POS]);
	printf("Mode:\t0x%02X\n", dataBuffer[OBD2_MODE_POS]);
	printf("PID:\t0x%02X\n", dataBuffer[OBD2_PID_POS]);
	printf("Data:\t");
	for (int i = 0; i < OBD2_DATA_SIZE; i++)
		printf("0x%02X ", dataBuffer[OBD2_DATA_POS + i]);
	printf("\n");
	printf("|---           DONE          ---|\n\n");
}

/*
 * void writeOBD2()
 * Purpose: a function to build a an OBD2 message and store it in 'msg'
 */
void writeOBD2(uint8_t* msg, uint8_t len, uint8_t mode, uint8_t pid, uint8_t* data)
{
	msg[OBD2_NUMBYTES_POS] = len;
	msg[OBD2_MODE_POS] = mode;
	msg[OBD2_PID_POS] = pid;
	for(int i = 0; i < OBD2_DATA_SIZE; i++)
		msg[OBD2_DATA_POS + i] = data[i];
}

/*
 * void loadOBD2()
 * Purpose: copy the message from the register to a buffer
 */
void loadOBD2(uint8_t* TxReg, uint8_t* msg)
{
	for(int x = 0; x < OBD2_DATA_LENGTH; x++)
		TxReg[x] = msg[x];
}


