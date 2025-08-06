/*
 * BMSmsg.c
 *
 *  Created on: Jul 8, 2025
 *      Author: Melona
 */

#include "BMSmsg.h"

/*
 * Mode
 */
uint8_t OBD2_BMS_MODE	=	0x22;	// OBD2 Mode for BMS

/*
 * PID
 */
uint8_t BMS_PID_PSOC[]		=	{0xF0, 0x0F};	// Pack State of Charge
uint8_t BMS_PID_PVOL[]		=	{0xF0, 0x0D};	// Pack Voltage
uint8_t BMS_PID_AVGT[]		=	{0xF0, 0x2A};	// Average Pack Temperature

/*
 * LEN for BMS PID for requesting
 */
uint8_t BMS_LEN_PSOC	= 	0x04; 		// 1 byte for Mode, 2 Byte for PID, 1 byte for padding
uint8_t BMS_LEN_PVOL	= 	0x04;		// Pack Voltage
uint8_t BMS_LEN_AVGT	=	0x04;		// Average Pack Temperature
