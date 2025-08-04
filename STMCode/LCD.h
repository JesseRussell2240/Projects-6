/*
 * LCD.h
 *
 *  Created on: Jun 6, 2025
 *      Author: hayde
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"
#include "ff.h"
#include "SDCard.h"
#include "stm32f3xx_hal.h"
#include <stdbool.h>
#include <string.h>


// Pin definitions
#define LCD_CS_PORT    GPIOB
#define LCD_CS_PIN     GPIO_PIN_6

#define LCD_DC_PORT    GPIOB
#define LCD_DC_PIN     GPIO_PIN_12

#define LCD_RST_PORT   GPIOA
#define LCD_RST_PIN    GPIO_PIN_11

// === Initialization & Communication ===
void LCD_Reset(void);
void LCD_Init(void);
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint8_t *data, uint16_t size);

// === Basic Drawing ===
void LCD_FillRGB(uint8_t r, uint8_t g, uint8_t b);
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b);

// === RLE Image Rendering ===
void LCD_DrawRLEImage(uint8_t *data, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h);
void LCD_Draw_RLE_Image_From_File(const char *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

// === Character Drawing ===
static void LCD_Draw_RLE_Char(char c, uint16_t x, uint16_t y, bool big);

// === Utility ===
uint16_t LCD_CenterDigits(uint16_t region_width, uint8_t num_digits, uint8_t digit_width);

// === Value Regions ===
void LCD_Draw_RPM_RLE_Region(uint16_t rpm, uint16_t row, uint16_t col_right_edge);
void LCD_Draw_Battery_RLE_Region(uint8_t battery_percent, uint16_t row, uint16_t col_right_edge);
void LCD_Draw_Speed_RLE_Region(uint16_t speed, uint16_t row, uint16_t col_right_edge);

void getLCD(uint32_t id, uint8_t* msg);

#endif /* INC_LCD_H_ */
