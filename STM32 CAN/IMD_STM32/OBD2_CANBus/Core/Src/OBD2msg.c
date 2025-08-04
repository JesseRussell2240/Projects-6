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
uint8_t OBD2_DATA_BMS_POS = 4;

uint8_t OBD2_DATA_SIZE = 4;
uint8_t OBD2_PID_BMS_SIZE = 2;

uint8_t IMD_DATA_LEN 	= 3;		// fixed, set manually
uint8_t IMD_MODE 		= 0x01; 	// set manually
uint8_t IMD_PID 		= 0x01;		// set manually
uint8_t IMD_FREQ_POS 	= 3;
uint8_t IMD_DUTY_POS 	= 4;
uint8_t IMD_STAT_POS 	= 5;

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
 * void readOBD2BMS()
 * Purpose: read the obd2 message, specifically for BMS, stored in 'msg'
 */
void readOBD2BMS(uint8_t* msg)
{
	// Copy the register into a temporary buffer
	uint8_t dataBuffer[8];
	for (int i = 0; i < OBD2_DATA_LENGTH; i++)
		dataBuffer[i] = msg[i];

	printf("|--- Reading OBD2 BMS Message... ---|\n");

	printf("Length:\t0x%02X\n", dataBuffer[OBD2_NUMBYTES_POS]);

	printf("Mode:\t0x%02X\n", dataBuffer[OBD2_MODE_POS]);

	printf("PID:\t");
		for (int j = 0; j < OBD2_PID_BMS_SIZE; j++)
			printf("0x%02X ", dataBuffer[OBD2_PID_POS + j]);
		printf("\n");

	printf("Data:\t");
	for (int i = 0; i < OBD2_DATA_SIZE; i++)
		printf("0x%02X ", dataBuffer[OBD2_DATA_BMS_POS + i]);
	printf("\n");

	printf("|---           DONE          ---|\n\n");
}

/*
 * void writeOBD2BMS()
 * Purpose: a function to build a an OBD2 message, specifically for the BMS,
 * and store it in 'msg'
 */
void writeOBD2BMS(uint8_t* msg, uint8_t len, uint8_t mode, uint8_t* pid, uint8_t* data)
{
	msg[OBD2_NUMBYTES_POS] = len;

	msg[OBD2_MODE_POS] = mode;

	for(int j = 0; j < OBD2_PID_BMS_SIZE; j++)
		msg[OBD2_PID_POS + j] = pid[j];

	for(int i = 0; i < OBD2_DATA_SIZE; i++)
		msg[OBD2_DATA_BMS_POS + i] = data[i];
}

/*
 * void writeOBD2IMD()
 * Purpose: a function to build a an OBD2 message, specifically for the writing IMD Info
 * and storing it in 'msg'
 */
void writeIMD(uint8_t* msg, uint8_t frequency, uint8_t dutyCycle, uint8_t status)
{
	msg[OBD2_NUMBYTES_POS] = IMD_DATA_LEN;
	msg[OBD2_MODE_POS] = IMD_MODE;
	msg[OBD2_PID_POS] = IMD_PID;
	msg[IMD_FREQ_POS] = frequency;	// msg[3]
	msg[IMD_DUTY_POS] = dutyCycle;	// msg[4]
	msg[IMD_STAT_POS] = status;		// msg[5]
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


