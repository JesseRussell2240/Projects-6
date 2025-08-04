/*
 * LCD.c
 *
 *  Created on: Jun 6, 2025
 *      Author: hayde
 */
/*
 * LCD.c
 *
 *  Created on: Jun 6, 2025
 *      Author: hayde
 */

#include "LCD.h"


#define MAX_IMAGE_SIZE 9800  // Max size for 70x70 images
static uint8_t rle_buffer[MAX_IMAGE_SIZE];  // Shared buffer for RLE decoding

extern SPI_HandleTypeDef hspi1;

// === Low-level LCD communication ===

static void LCD_SelectCmdMode(void) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

static void LCD_SelectDataMode(void) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

static void LCD_Deselect(void) {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

void LCD_WriteCommand(uint8_t cmd) {
    LCD_SelectCmdMode();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    LCD_Deselect();
}

void LCD_WriteData(uint8_t *data, uint16_t size) {
    LCD_SelectDataMode();
    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
    LCD_Deselect();
}

void LCD_Reset(void) {
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(150);
}

void LCD_Init(void) {
    LCD_Reset();
    LCD_WriteCommand(0x01); HAL_Delay(150); // Software reset
    LCD_WriteCommand(0x11); HAL_Delay(150); // Sleep out

    uint8_t pixel_format = 0x55; // 16-bit
    LCD_WriteCommand(0x3A); LCD_WriteData(&pixel_format, 1);

    uint8_t mem_access = 0x48;
    LCD_WriteCommand(0x36); LCD_WriteData(&mem_access, 1);

    LCD_WriteCommand(0x29); HAL_Delay(10); // Display ON
}

// === Drawing Helpers ===

static void LCD_SetAddressWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint8_t col[] = {x >> 8, x & 0xFF, (x + w - 1) >> 8, (x + w - 1) & 0xFF};
    uint8_t row[] = {y >> 8, y & 0xFF, (y + h - 1) >> 8, (y + h - 1) & 0xFF};

    LCD_WriteCommand(0x2A); LCD_WriteData(col, 4);
    LCD_WriteCommand(0x2B); LCD_WriteData(row, 4);
    LCD_WriteCommand(0x2C);
}

void LCD_FillRGB(uint8_t r, uint8_t g, uint8_t b) {
    LCD_SetAddressWindow(0, 0, 320, 480);
    uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    uint8_t pixel[2] = {color >> 8, color & 0xFF};

    for (int i = 0; i < 320 * 480; i++) LCD_WriteData(pixel, 2);
}

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b) {
    LCD_SetAddressWindow(x, y, w, h);
    uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    uint8_t pixel[2] = {color >> 8, color & 0xFF};

    uint16_t total_pixels = w * h;
    uint16_t max_chunk = 256;
    uint8_t buffer[512];

    for (int i = 0; i < max_chunk; i++) {
        buffer[i * 2] = pixel[0];
        buffer[i * 2 + 1] = pixel[1];
    }

    while (total_pixels > 0) {
        uint16_t chunk = (total_pixels > max_chunk) ? max_chunk : total_pixels;
        LCD_WriteData(buffer, chunk * 2);
        total_pixels -= chunk;
    }
}


void LCD_DrawRLEImage(uint8_t *data, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h) {
    uint8_t *p = data;
    for (int y = 0; y < h; y++) {
        LCD_SetAddressWindow(x0, y0 + y, w, 1);
        int pixels_written = 0;
        while (pixels_written < w) {
            uint8_t count = *p++;
            uint8_t hi = *p++;
            uint8_t lo = *p++;
            for (int i = 0; i < count; i++) {
                uint8_t color[2] = {hi, lo};
                LCD_WriteData(color, 2);
            }
            pixels_written += count;
        }
    }
}

void LCD_Draw_RLE_Image_From_File(const char *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    FIL file;
    UINT bytesRead;
    if (f_open(&file, filename, FA_READ) != FR_OK) return;
    if (f_read(&file, rle_buffer, sizeof(rle_buffer), &bytesRead) != FR_OK) {
        f_close(&file);
        return;
    }
    f_close(&file);
    LCD_DrawRLEImage(rle_buffer, x, y, width, height);
}

static void LCD_Draw_RLE_Char(char c, uint16_t x, uint16_t y, bool big) {
    const char *filename = NULL;

    if (c >= '0' && c <= '9') {
        static const char *small[10] = {"0.rle", "1.rle", "2.rle", "3.rle", "4.rle", "5.rle", "6.rle", "7.rle", "8.rle", "9.rle"};
        static const char *large[10] = {"Big0.rle", "Big1.rle", "Big2.rle", "Big3.rle", "Big4.rle", "Big5.rle", "Big6.rle", "Big7.rle", "Big8.rle", "Big9.rle"};
        filename = big ? large[c - '0'] : small[c - '0'];
    } else {
        switch (c) {
            case ':': filename = "Colon.rle"; break;
            case '%': filename = "Percent.rle"; break;
            default: return;
        }
    }

    if (!filename) return;

    uint16_t w = big ? 70 : 32;
    uint16_t h = w;
    LCD_Draw_RLE_Image_From_File(filename, x, y, w, h);
}

uint16_t LCD_CenterDigits(uint16_t region_width, uint8_t num_digits, uint8_t digit_width) {
    if (num_digits == 0 || digit_width == 0) return 0;

    uint16_t total_width = num_digits * digit_width;
    if (total_width > region_width) total_width = region_width;

    return (region_width - total_width) / 2;
}

void LCD_Draw_RPM_RLE_Region(uint16_t rpm, uint16_t row, uint16_t col_right_edge) {
    static uint16_t last_rpm = 65535;
    static char last_str[5] = "----";  // Max 4 digits

    if (rpm > 9999) rpm = 9999;
    if (rpm == last_rpm) return;

    char str[5];
    sprintf(str, "%u", rpm);
    int num_digits = strlen(str);

    const uint16_t region_width = 128;
    const uint8_t digit_width = 32;

    int old_num_digits = strlen(last_str);
    uint16_t old_offset = LCD_CenterDigits(region_width, old_num_digits, digit_width);
    uint16_t new_offset = LCD_CenterDigits(region_width, num_digits, digit_width);

    bool full_redraw = (num_digits != old_num_digits) || (old_offset != new_offset);

    if (full_redraw) {
        for (int i = 0; i < old_num_digits; i++) {
            uint16_t digit_col = col_right_edge - (old_offset + i * digit_width);
            LCD_FillRect(row, digit_col, digit_width, digit_width, 0, 0, 0);
        }
        for (int i = 0; i < num_digits; i++) {
            uint16_t digit_col = col_right_edge - (new_offset + i * digit_width);
            LCD_Draw_RLE_Char(str[i], row, digit_col, false);
        }
    } else {
        for (int i = 0; i < num_digits; i++) {
            if (str[i] != last_str[i]) {
                uint16_t digit_col = col_right_edge - (new_offset + i * digit_width);
                LCD_FillRect(row, digit_col, digit_width, digit_width, 0, 0, 0);
                LCD_Draw_RLE_Char(str[i], row, digit_col, false);
            }
        }
    }

    strcpy(last_str, str);
    last_rpm = rpm;
}

void LCD_Draw_Battery_RLE_Region(uint8_t battery_percent, uint16_t row, uint16_t col_right_edge) {
    static uint8_t last_percent = 255;
    static char last_str[4] = "---";  // Up to 3 digits + null terminator

    if (battery_percent > 100) battery_percent = 100;
    if (battery_percent == last_percent) return;

    char str[4];
    sprintf(str, "%u", battery_percent);
    int num_digits = strlen(str);

    const uint16_t region_width = 96;
    const uint8_t digit_width = 32;

    int old_num_digits = strlen(last_str);
    uint16_t old_offset = LCD_CenterDigits(region_width, old_num_digits, digit_width);
    uint16_t new_offset = LCD_CenterDigits(region_width, num_digits, digit_width);

    bool full_redraw = (num_digits != old_num_digits) || (old_offset != new_offset);

    if (full_redraw) {
        for (int i = 0; i < old_num_digits; i++) {
            uint16_t digit_col = col_right_edge - (old_offset + i * digit_width);
            LCD_FillRect(row, digit_col, digit_width, digit_width, 0, 0, 0);
        }
        for (int i = 0; i < num_digits; i++) {
            uint16_t digit_col = col_right_edge - (new_offset + i * digit_width);
            LCD_Draw_RLE_Char(str[i], row, digit_col, false);
        }
    } else {
        for (int i = 0; i < num_digits; i++) {
            if (str[i] != last_str[i]) {
                uint16_t digit_col = col_right_edge - (new_offset + i * digit_width);
                LCD_FillRect(row, digit_col, digit_width, digit_width, 0, 0, 0);
                LCD_Draw_RLE_Char(str[i], row, digit_col, false);
            }
        }
    }

    strcpy(last_str, str);
    last_percent = battery_percent;
}

void LCD_Draw_Speed_RLE_Region(uint16_t speed, uint16_t row, uint16_t col_right_edge) {
    static uint16_t last_speed = 65535;
    static char last_str[4] = "---";  // Up to 3 digits + null terminator

    if (speed > 999) speed = 999;
    if (speed == last_speed) return;

    char str[4];
    sprintf(str, "%u", speed);
    int num_digits = strlen(str);

    const uint16_t region_width = 210;
    const uint8_t digit_width = 70;

    int old_num_digits = strlen(last_str);
    uint16_t old_offset = LCD_CenterDigits(region_width, old_num_digits, digit_width);
    uint16_t new_offset = LCD_CenterDigits(region_width, num_digits, digit_width);

    bool full_redraw = (num_digits != old_num_digits) || (old_offset != new_offset);

    if (full_redraw) {
        for (int i = 0; i < old_num_digits; i++) {
            uint16_t digit_col = col_right_edge - (old_offset + i * digit_width);
            LCD_FillRect(row, digit_col, digit_width, digit_width, 0, 0, 0);
        }
        for (int i = 0; i < num_digits; i++) {
            uint16_t digit_col = col_right_edge - (new_offset + i * digit_width);
            LCD_Draw_RLE_Char(str[i], row, digit_col, true);
        }
    } else {
        for (int i = 0; i < num_digits; i++) {
            if (str[i] != last_str[i]) {
                uint16_t digit_col = col_right_edge - (new_offset + i * digit_width);
                LCD_FillRect(row, digit_col, digit_width, digit_width, 0, 0, 0);
                LCD_Draw_RLE_Char(str[i], row, digit_col, true);
            }
        }
    }

    strcpy(last_str, str);
    last_speed = speed;
}


void getLCD(uint32_t id, uint8_t* msg)
{
	//if (id == 0x6D0)
	//{
	//	int charge = msg[0] / 2;
	//	printf("SOC: %d\n", charge);
	//	LCD_Draw_Battery_RLE_Region(charge, 240, 400);
	//}
	if (id == 0x181 && msg[0] == 0x30)
	{
		 uint16_t rpm_raw = msg[2] << 8 | msg[1];
		 int rpm = (rpm_raw * 6600) / 32767;
		 printf("RPM: %d\n", rpm);
		 LCD_Draw_RPM_RLE_Region(rpm, 280, 330);
	}
}



