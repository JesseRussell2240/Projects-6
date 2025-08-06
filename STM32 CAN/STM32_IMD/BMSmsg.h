/*
 * BMSmsg.h
 *
 *  Created on: Jul 8, 2025
 *      Author: Melona
 */

#ifndef INC_BMSMSG_H_
#define INC_BMSMSG_H_

#include <stdint.h>

/*
 * CAN ID to set for talking to OBD2
 */
#define BMS_ID 0x7E3

/*
 * Mode for BMS
 */
extern uint8_t OBD2_BMS_MODE;

/*
 * PID for BMS
 * BMS uses 4 Hex instead of 2 hex for PID
 */
extern uint8_t BMS_PID_PSOC[2];		// Pack State of Charge
extern uint8_t BMS_PID_PVOL[2];		// Pack Voltage
extern uint8_t BMS_PID_AVGT[2];		// Average Pack Temperature

/*
 * LEN for BMS PID
 */
extern uint8_t BMS_LEN_PSOC;		// Pack State of Charge
extern uint8_t BMS_LEN_PVOL;		// Pack Voltage
extern uint8_t BMS_LEN_AVGT;		// Average Pack Temperature

#endif /* INC_BMSMSG_H_ */
