/*
 * SDCard.c
 *
 *  Created on: Jul 4, 2025
 *      Author: hayde
 */
#include "SDCard.h"



#include "fatfs.h"  // Required for USERFatFS and USERPath


void SD_Card_Test(void)
{
  FATFS FatFs;
  FIL Fil;
  FRESULT FR_Status;
  FATFS *FS_Ptr;
  UINT RWC, WWC; // Read/Write Word Counter
  DWORD FreeClusters;
  uint32_t TotalSize, FreeSpace;
  char RW_Buffer[200];
  do
  {
    //------------------[ Mount The SD Card ]--------------------
    FR_Status = f_mount(&FatFs, "", 1);
    if (FR_Status != FR_OK)
    {
      printf("Error! While Mounting SD Card, Error Code: (%i)\r\n", FR_Status);
      break;
    }
    printf("SD Card Mounted Successfully! \r\n\n");

    //------------------[ Get & Print The SD Card Size & Free Space ]--------------------
    f_getfree("", &FreeClusters, &FS_Ptr);
    TotalSize = (uint32_t)((FS_Ptr->n_fatent - 2) * FS_Ptr->csize * 0.5);
    FreeSpace = (uint32_t)(FreeClusters * FS_Ptr->csize * 0.5);
    printf("Total SD Card Size: %lu Bytes\r\n", TotalSize);
    printf("Free SD Card Space: %lu Bytes\r\n\n", FreeSpace);

    //------------------[ Open A Text File For Write & Write Data ]--------------------
    FR_Status = f_open(&Fil, "test.txt", FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
    if(FR_Status != FR_OK)
    {
      printf("Error! While Creating/Opening A New Text File, Error Code: (%i)\r\n", FR_Status);
      break;
    }
    printf("Text File Created & Opened! Writing Data To The Text File..\r\n\n");

    f_puts("Hello! From STM32 To SD Card Over SPI, Using f_puts()\n", &Fil);
    strcpy(RW_Buffer, "Hello! From STM32 To SD Card Over SPI, Using f_write()\r\n");
    f_write(&Fil, RW_Buffer, strlen(RW_Buffer), &WWC);
    f_close(&Fil);

    //------------------[ Open A Text File For Read & Read Its Data ]--------------------
    FR_Status = f_open(&Fil, "test.txt", FA_READ);
    if(FR_Status != FR_OK)
    {
      printf("Error! While Opening (test.txt) File For Read.. \r\n");
      break;
    }

    f_gets(RW_Buffer, sizeof(RW_Buffer), &Fil);
    printf("Data Read From (test.txt) Using f_gets():%s\n", RW_Buffer);

    f_read(&Fil, RW_Buffer, f_size(&Fil), &RWC);
    printf("Data Read From (test.txt) Using f_read():%s\n", RW_Buffer);

    f_close(&Fil);
    printf("File Closed! \r\n\n");

    //------------------[ Open An Existing Text File, Update Its Content, Read It Back ]--------------------
    FR_Status = f_open(&Fil, "test.txt", FA_OPEN_EXISTING | FA_WRITE);
    FR_Status = f_lseek(&Fil, f_size(&Fil));
    if(FR_Status != FR_OK)
    {
      printf("Error! While Opening (test.txt) File For Update.. \r\n");
      break;
    }

    f_puts("This New Line Was Added During Update!\r\n", &Fil);
    f_close(&Fil);
    memset(RW_Buffer,'\0',sizeof(RW_Buffer));

    FR_Status = f_open(&Fil, "test.txt", FA_READ);
    f_read(&Fil, RW_Buffer, f_size(&Fil), &RWC);
    printf("Data Read From (test.txt) After Update:%s\n", RW_Buffer);
    f_close(&Fil);

    //------------------[ Delete The Text File ]--------------------
    /*
    FR_Status = f_unlink(TextFileWrite.txt);
    if (FR_Status != FR_OK){
        printf("Error! While Deleting The (TextFileWrite.txt) File.. \r\n");
    }
    */
  } while(0);

  //------------------[ Test Complete! Unmount The SD Card ]--------------------
  FR_Status = f_mount(NULL, "", 0);
  if (FR_Status != FR_OK)
  {
      printf("Error! While Un-mounting SD Card, Error Code: (%i)\r\n", FR_Status);
  } else{
      printf("SD Card Un-mounted Successfully! \r\n");
  }
}

