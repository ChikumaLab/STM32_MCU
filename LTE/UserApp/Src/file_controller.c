/*
 * file_controller.c
 *
 *  Created on: 2022/09/19
 *      Author: User
 */
#include "file_controller.h"
#include "fatfs.h"

extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

FATFS fs;
FIL MyFile;
FRESULT fresult;

void mount_usb(void)
{
	fresult = f_mount(&USBHFatFS,USBHPath,1);
	fresult = f_open(&USBHFile, "aaaa.txt", FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
	fresult = f_close(&USBHFile);

	if(fresult != FR_OK)
	{

	}
	else
	{

	}
}

uint8_t fs_file_open(char *filename)
{
	fresult = f_open(&USBHFile, filename, FA_OPEN_ALWAYS|FA_WRITE);
	fresult = f_lseek(&USBHFile, f_size(&USBHFile));

	return fresult;
}

uint8_t fs_file_write(char *data,uint32_t len)
{
	unsigned int length;
	fresult = f_write (&USBHFile, data, len,&length);
	return fresult;
}

uint8_t fs_file_close()
{
	fresult = f_close (&USBHFile);
	return fresult;
}




void unmount_usb(void)
{
	fresult = f_mount(NULL,USBHPath,1);
	if(fresult != FR_OK)
	{

	}
	else
	{

	}

}

uint32_t get_RTC_time (void)
{
	uint32_t timeData;
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	timeData = ((sDate.Year + 20) << 25) |
			   (sDate.Month << 21) |
			   (sDate.Date <<16) |
			   (sTime.Hours <<11) |
			   ((sTime.Minutes << 5)) |
			   ((sTime.Seconds >> 1) & 0x0000001f);

	return timeData;
}
