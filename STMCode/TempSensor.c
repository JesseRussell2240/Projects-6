#include "TempSensor.h"
#include "stm32f3xx_hal.h"
#include "stm32f3xx_ll_usart.h"

extern UART_HandleTypeDef huart1;

#define SENSOR_COUNT 3

static uint8_t DS18B20_ReadBit(void);
static uint8_t DS18B20_ReadByte(void);
static void DS18B20_WriteByte(uint8_t data);
static uint8_t sensorROMs[SENSOR_COUNT][8];  // ROM codes for 3 sensors


uint8_t DS18B20_Init(void)
{
    uint8_t ResetByte = 0xF0, PresenceByte;
    LL_USART_SetBaudRate(huart1.Instance, HAL_RCC_GetPCLK2Freq(), LL_USART_OVERSAMPLING_16, 9600);
    HAL_UART_Transmit(&huart1, &ResetByte, 1, 1);
    HAL_UART_Receive(&huart1, &PresenceByte, 1, 1);
    LL_USART_SetBaudRate(huart1.Instance, HAL_RCC_GetPCLK2Freq(), LL_USART_OVERSAMPLING_16, 115200);

    return (PresenceByte != ResetByte);
}

void DS18B20_SampleTemp(void)
{
    DS18B20_Init();
    DS18B20_WriteByte(0xCC);  // Skip ROM
    DS18B20_WriteByte(0x44);  // Convert T
}

float DS18B20_ReadTemp(void)
{
    uint8_t Temp_LSB, Temp_MSB;
    uint16_t Temp;

    DS18B20_Init();
    DS18B20_WriteByte(0xCC);  // Skip ROM
    DS18B20_WriteByte(0xBE);  // Read Scratchpad

    Temp_LSB = DS18B20_ReadByte();
    Temp_MSB = DS18B20_ReadByte();
    Temp = ((Temp_MSB << 8) | Temp_LSB);

    return (float)Temp / 16.0;
}

static uint8_t DS18B20_ReadBit(void)
{
    uint8_t ReadBitCMD = 0xFF;
    uint8_t RxBit;

    HAL_UART_Transmit(&huart1, &ReadBitCMD, 1, 1);
    HAL_UART_Receive(&huart1, &RxBit, 1, 1);

    return (RxBit & 0x01);
}

static uint8_t DS18B20_ReadByte(void)
{
    uint8_t RxByte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        RxByte >>= 1;
        if (DS18B20_ReadBit())
        {
            RxByte |= 0x80;
        }
    }
    return RxByte;
}

static void DS18B20_WriteByte(uint8_t data)
{
    uint8_t TxBuffer[8];
    for (int i = 0; i < 8; i++)
    {
        TxBuffer[i] = (data & (1 << i)) ? 0xFF : 0x00;
    }
    HAL_UART_Transmit(&huart1, TxBuffer, 8, 10);
}

void DS18B20_ReadROM(uint8_t *rom)
{
    DS18B20_Init();
    DS18B20_WriteByte(0x33);  // Read ROM command

    for (int i = 0; i < 8; i++) {
        rom[i] = DS18B20_ReadByte();
    }
}


void DS18B20_SetROM(uint8_t index, uint8_t *romCode)
{
    if (index < SENSOR_COUNT)
    {
        for (int i = 0; i < 8; i++)
            sensorROMs[index][i] = romCode[i];
    }
}

static void DS18B20_MatchROM(uint8_t *romCode)
{
    DS18B20_WriteByte(0x55);  // Match ROM
    for (int i = 0; i < 8; i++)
        DS18B20_WriteByte(romCode[i]);
}

void DS18B20_ReadTempIndexed(uint8_t index, float *temperature)
{
    if (index >= SENSOR_COUNT) return;

    DS18B20_Init();
    DS18B20_MatchROM(sensorROMs[index]);
    DS18B20_WriteByte(0x44);  // Convert T
    //HAL_Delay(750);

    DS18B20_Init();
    DS18B20_MatchROM(sensorROMs[index]);
    DS18B20_WriteByte(0xBE);  // Read Scratchpad

    uint8_t lsb = DS18B20_ReadByte();
    uint8_t msb = DS18B20_ReadByte();
    uint16_t raw = (msb << 8) | lsb;

    *temperature = (float)raw / 16.0;
}


