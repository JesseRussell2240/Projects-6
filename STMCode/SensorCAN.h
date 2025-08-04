/*
 * CAN.h
 *
 *  Created on: Jul 29, 2025
 *      Author: hayde
 */

#ifndef INC_OBD2MSG_H_
#define INC_OBD2MSG_H_

#include <stdint.h>

// OBD2 field positions
extern uint8_t OBD2_DATA_LENGTH;
extern uint8_t OBD2_NUMBYTES_POS;
extern uint8_t OBD2_MODE_POS;
extern uint8_t OBD2_PID_POS;
extern uint8_t OBD2_DATA_POS;
extern uint8_t OBD2_DATA_SIZE;

// Function to populate an OBD2-style CAN message
void writeOBD2(uint8_t* msg, uint8_t len, uint8_t mode, uint8_t pid, uint8_t* data);
void loadOBD2(uint8_t* dest, const uint8_t* msg);
void writeOBD2Temp(float temperature, uint8_t pid);

#endif /* INC_OBD2MSG_H_ */

