/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SDCard.h"
#include "LCD.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

CAN_RxHeaderTypeDef		RxHeader;
uint8_t					RxData[8];

// Global flags
volatile uint8_t spi_xmit_flag = 0;
volatile uint8_t spi_recv_flag = 0;
volatile uint8_t lcdUpdatePending = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_CAN_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) ITM_SendChar(*ptr++);
    return len;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */




  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);


  LCD_Init();

  LCD_FillRGB(0, 0, 0); // black background

   FATFS FatFs;
   FIL imageFile;
   FRESULT fr = f_mount(&FatFs, "", 1);
   printf("Mount result: %d\r\n", fr);

   if (fr == FR_OK) {
       printf("SD card mounted.\r\n");

       // Show static symbols
       LCD_Draw_RLE_Image_From_File("Battery.rle", 240, 440, 32, 32);     // Battery icon
      // LCD_Draw_BND_Char('5', 240, 400);     // Battery icon
      // LCD_Draw_BND_Char('8', 240, 370);     // Battery icon
       LCD_Draw_RLE_Image_From_File("r.rle", 280, 440, 32, 32);    // R
       LCD_Draw_RLE_Image_From_File("p.rle", 280, 410, 32, 32);    // P
       LCD_Draw_RLE_Image_From_File("m.rle", 280, 380, 32, 32);    // M
       LCD_Draw_RLE_Image_From_File("k.rle", 140, 100, 32, 32);    // K
       LCD_Draw_RLE_Image_From_File("m.rle", 140, 70, 32, 32);    // M
       LCD_Draw_RLE_Image_From_File("Slash.rle", 140, 40, 32, 32);    // /
       LCD_Draw_RLE_Image_From_File("h.rle", 140, 10, 32, 32);    // H
//       LCD_Draw_BND_Char(':', 280, 350);    // 1
     //  LCD_Draw_BND_Char('1', 280, 330);    // 0
     //  LCD_Draw_BND_Char('0', 280, 300);    // 0
     //  LCD_Draw_BND_Char('0', 280, 270);    // 0
     //  LCD_Draw_BND_Char('0', 280, 240);    // 0

   //    LCD_Draw_BND_Char('K', 80, 310);    // 0
   //    LCD_Draw_BND_Char('M', 80, 240);    // 0
   //    LCD_Draw_BND_Char('X', 80, 170);    // 0
   //    LCD_Draw_BND_Char('H', 80, 100);    // 0



    //   uint16_t rpm = 0, speed = 0, battery_percent = 0;
//
  //        while (1) {
  //            LCD_Draw_Battery_RLE_Region(battery_percent, 240, 400);  // Centered region
  //      	  LCD_Draw_RPM_RLE_Region(rpm, 280, 330);
   //           LCD_Draw_Speed_RLE_Region(speed, 100, 280);  // Adjust row/col as needed


            //  HAL_Delay(100);  // Visual update delay

     //         battery_percent = (battery_percent + 1) % 101;  // Cycle 0 → 100
     //         rpm = (rpm + 100) % 9100;
     //         speed = (speed + 5) % 105;
    //     }
   } else {
       printf("Failed to mount SD card\r\n");
       while (1);
  }







 //  LCD_FillRGB(255, 0, 0);   // Red
 //  HAL_Delay(1000);
 //  LCD_FillRGB(0, 255, 0);   // Green
 // HAL_Delay(1000);
 //  LCD_FillRGB(0, 0, 255);   // Blue
 //  HAL_Delay(1000);



  // LCD_DrawImage(0, 0, 320, 480, josh_image);



   //Touch_Init();
   //uint8_t wasTouched = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // Receive
	  if (lcdUpdatePending) {
	  	lcdUpdatePending = 0;

	  	getLCD(RxHeader.StdId, RxData);

	  	memset(RxData, 0x00, sizeof(RxData));
	  	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	  }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN;
  hcan.Init.Prescaler = 8;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_4TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  // Set Up CAN_Rx Filter

  CAN_FilterTypeDef filter;		// This is one of the 13 filters - can create more filters - this one will be number 0

  // Configure filter 0 to accept CAN IDs from 0x000 to 0x7D0
  filter.FilterBank = 0;                              // Filter 0
  filter.FilterMode = CAN_FILTERMODE_IDMASK;          // Use ID-mask mode
  filter.FilterScale = CAN_FILTERSCALE_32BIT;         // Use 32-bit filters

  filter.FilterIdHigh = 0x000 << 5;   // Match from ID 0
  filter.FilterIdLow = 0x0000;

  filter.FilterMaskIdHigh = 0x0000;   // Wildcard: don't care about any bits
  filter.FilterMaskIdLow = 0x0000;

  filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;     // Route matches to FIFO0
  filter.FilterActivation = ENABLE;                   // Enable filter
  filter.SlaveStartFilterBank = 14;                   // Only needed on dual CAN controllers (safe default)


  if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK)	// Set the above value for filter 0
  {
   Error_Handler();
  }

  // Start the CAN Peripheral
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
   Error_Handler();
  }

  // Activate CAN_Rx notification interrupt
  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
   Error_Handler();
  }


  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_DC_Pin|LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DC_Pin SD_CS_Pin LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin|SD_CS_Pin|LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_RST_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// This is called when SPI transmit is done
void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef * hspi)
{
  // Set CS pin to high and raise flag
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  spi_xmit_flag = 1;
}

// This is called when SPI receive is done
void HAL_SPI_RxCpltCallback (SPI_HandleTypeDef * hspi)
{
  // Set CS pin to high and raise flag
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  spi_recv_flag = 1;
}

// Override the HAL_CAN_RxFifo0MsgPendingCallback function
// This is called when the interrupt for FIFO0 is triggered.
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // Get the CAN message
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
        Error_Handler();
    }

    // Disable further CAN RX interrupts
    HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

    // Set flag to update LCD in main loop
    lcdUpdatePending = 1;
}


// Override the HAL_GPIO Callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// Set the BUTTON Flag to indicate which button was pressed
//	if (GPIO_Pin == GPIO_PIN_13)
//	{
//		BUTTON = BLUE_BUTTON_PRESSED;
//	}



}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
