/*
 * OBD2msg.h
 *
 *  Created on: May 20, 2025
 *      Author: Melona
 */

#ifndef INC_OBD2MSG_H_
#define INC_OBD2MSG_H_

#include <stdint.h>

extern uint8_t OBD2_DATA_LENGTH;
extern uint8_t OBD2_NUMBYTES_POS;
extern uint8_t OBD2_MODE_POS;
extern uint8_t OBD2_PID_POS;
extern uint8_t OBD2_DATA_POS;
extern uint8_t OBD2_DATA_SIZE;

void readOBD2(uint8_t* msg);
void writeOBD2(uint8_t* msg, uint8_t len, uint8_t mode, uint8_t pid, uint8_t* data);
void loadOBD2(uint8_t* TxReg, uint8_t* msg);


// uint8_t testmsg[] = { 0x03, 0x41, 0x0D, 0x32, 0xAA, 0xAA, 0xAA, 0xAA };

#endif /* INC_OBD2MSG_H_ */
