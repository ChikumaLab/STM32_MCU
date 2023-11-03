/*
 * drv_gps.c
 *
 *  Created on: 2022/08/22
 *      Author: User
 */

#include "drv_gps.h"
#include <string.h>
#include <stdlib.h>
#include "fatfs.h"

void gps_getTime(char *data);
float gps_getLongitude(char *gpsData);
float gps_getLatitude(char *gpsData);

extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

float gpsPos_data[2];
RTC_TimeTypeDef gpsTime_data;

uint8_t gps_fileWriteFlag = 0;
char gps_writeDataBuf[512];
uint8_t drv_gps_txData_buf[512];
UART_HandleTypeDef *drv_gps_port_if;
uint8_t drv_gps_data_rxBuf[DRV_GPS_RX_MAX_BUF_SIZE];
uint16_t drv_gps_buf_start_index;
uint8_t drv_gps_time_sync_flag;

uint8_t drv_gps_culc_checksum(uint8_t *data,uint16_t len)
{
	int i = 0;
	uint8_t buf = 0;

	for(i = 0;i < len;i++)
	{
		buf = buf ^ data[i];
	}

	return (buf);
}

void drv_gps_init(UART_HandleTypeDef *port)
{
	drv_gps_port_if = port;
	HAL_UART_Receive_DMA(drv_gps_port_if, drv_gps_data_rxBuf, sizeof(drv_gps_data_rxBuf));
}

void drv_gps_rx_restart(void)
{
	HAL_UART_Receive_DMA(drv_gps_port_if, drv_gps_data_rxBuf, sizeof(drv_gps_data_rxBuf));
}

void drv_gps_rxError_restart(void)
{
	int i;
	drv_gps_buf_start_index = 0;

	for(i = 0;i <DRV_GPS_RX_MAX_BUF_SIZE;i++ )
	{
		drv_gps_data_rxBuf[i] = 0x00;
	}
	HAL_UART_Receive_DMA(drv_gps_port_if, drv_gps_data_rxBuf, sizeof(drv_gps_data_rxBuf));
}

uint16_t drv_gps_check_lf_data(void)
{
	uint16_t cnt = 0;
	uint16_t cnt2 = 0;
	uint16_t cnt3 = 0;
	if(drv_gps_buf_start_index<DRV_GPS_RX_MAX_BUF_SIZE)
	{
		cnt2 = drv_gps_buf_start_index;
	}
	else
	{
		cnt2 = 0;
		drv_gps_buf_start_index = 0;
	}


	while(drv_gps_data_rxBuf[cnt + cnt2 - cnt3] != '\n')
	{
		if(drv_gps_data_rxBuf[cnt + cnt2 - cnt3] == 0)
		{
			cnt = 0;
			break;
		}
		cnt++;
		if((cnt + cnt2) >= DRV_GPS_RX_MAX_BUF_SIZE )
		{
			cnt3 = DRV_GPS_RX_MAX_BUF_SIZE;
		}
		else if(cnt + cnt2 - cnt3>=DRV_GPS_RX_MAX_BUF_SIZE)
		{
			cnt = 0;
			break;
		}
		else
		{
			cnt3 = 0;
		}
	}

	return cnt;
}

uint16_t drv_gps_get_data(uint8_t *rxData)
{
    int i;
    uint16_t cnt,cnt2;

	cnt = drv_gps_check_lf_data();
	cnt2 = 0;
	if(cnt == 0)
	{
		return 0;
	}

	for(i = 0;i <= cnt;i++)
	{
		if((i + drv_gps_buf_start_index) < DRV_GPS_RX_MAX_BUF_SIZE)
		{
			rxData[i] =drv_gps_data_rxBuf[i + drv_gps_buf_start_index];
			drv_gps_data_rxBuf[i + drv_gps_buf_start_index] = 0;
			cnt2 = i + drv_gps_buf_start_index;
		}
		else
		{
			rxData[i] = drv_gps_data_rxBuf[i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE];
			drv_gps_data_rxBuf[i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE] = 0;
			cnt2 = i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE;
		}
	}

	if((cnt2 + 1) < DRV_GPS_RX_MAX_BUF_SIZE)
	{
		drv_gps_buf_start_index = cnt2 + 1;
	}
	else
	{
		drv_gps_buf_start_index = 0;
	}

    return cnt;
}


void drv_gps_set_output_rate(int time)
{
	char data[16];
	uint8_t checksum;

	sprintf(data,"PMTK220,%d",time);

	checksum = drv_gps_culc_checksum((uint8_t *)data,(uint16_t)strlen(data));
	sprintf((char *)drv_gps_txData_buf,"$%s*%02X,\r\n",data,checksum);

	HAL_UART_Transmit(drv_gps_port_if, drv_gps_txData_buf, strlen((char*)drv_gps_txData_buf), 1000);
}

uint8_t drv_gps_chenge_baudrate(UART_HandleTypeDef *port,uint32_t budrate)
{
	return 0;
}
uint8_t gps_rxCmd[512];

void app_gps_rxCmd_txPC(void)
{
	uint16_t cnt ;
	uint16_t i;
	uint16_t cnt2 = 0;
    uint8_t dummy,dummy2;

    gps_rxCmd[0] = 0x7E;

    gps_rxCmd[3] = CMD_IND;
    dummy = gps_rxCmd[3] ;

    gps_rxCmd[4] = MY_ADDRESS;
    dummy += gps_rxCmd[4] ;

    gps_rxCmd[5] = PC_ADDRESS;
    dummy += gps_rxCmd[5] ;

    gps_rxCmd[6] = GPS_RX_DATA;
    dummy += gps_rxCmd[6];

    do
    {
    	dummy2 = dummy;
    	cnt = drv_gps_check_lf_data();
    	if(cnt == 0)
    	{
    		return;
    	}
    	gps_rxCmd[1] = (uint8_t)(((cnt + 6)  & 0xFF00) >> 8);
    	gps_rxCmd[2] = (uint8_t)((cnt + 6)  & 0x00FF) ;

    	for(i = 0;i <= cnt;i++)
    	{
    		if((i + drv_gps_buf_start_index) < DRV_GPS_RX_MAX_BUF_SIZE)
    		{
    			gps_rxCmd[7 + i] =drv_gps_data_rxBuf[i + drv_gps_buf_start_index];
    			dummy2 += gps_rxCmd[7 + i];
    			drv_gps_data_rxBuf[i + drv_gps_buf_start_index] = 0;
    			cnt2 = i + drv_gps_buf_start_index;
    		}
    		else
    		{
    			gps_rxCmd[7 + i] = drv_gps_data_rxBuf[i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE];
    			dummy2 += gps_rxCmd[7 + i];
    			drv_gps_data_rxBuf[i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE] = 0;
    			cnt2 = i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE;
    		}
    	}
    	gps_rxCmd[7 + i] = (uint8_t)(0xff - (dummy2));

    	if((cnt2 + 1) < DRV_GPS_RX_MAX_BUF_SIZE)
    	{
    		drv_gps_buf_start_index = cnt2 + 1;
    	}
    	else
    	{
    		drv_gps_buf_start_index = 0;
    	}

		HAL_UART_Transmit(uart_pc_if, gps_rxCmd, cnt + 9,1000);
		HAL_Delay(10);
   }
    while(cnt != 0);
}

void app_gps_rxData_WriteFIle(void)
{
	uint16_t cnt ;
	uint16_t i;
	uint16_t cnt2 = 0;
    uint8_t rxData[256];
    char serchData[] = "$GPRMC";
    char *dummy;
	RTC_TimeTypeDef sTime;
	float latitude,longitude;

    do
    {
    	cnt = drv_gps_check_lf_data();		// バッファに存在するデータ数を確認
    	if(cnt == 0)						// バッファデータが空だった場合の処理
    	{
    		return;
    	}

    	for(i = 0;i <= cnt;i++)				// バッファから取得したデータをrxData変数に代入し、読み取った領域をNULLに置き換え
    	{
    		if((i + drv_gps_buf_start_index) < DRV_GPS_RX_MAX_BUF_SIZE)	// 取得したデータが最大を超える場合の処理（リングバッファ）
    		{
    			rxData[i] =drv_gps_data_rxBuf[i + drv_gps_buf_start_index];
    			drv_gps_data_rxBuf[i + drv_gps_buf_start_index] = 0;
    			cnt2 = i + drv_gps_buf_start_index;
    		}
    		else
    		{
    			rxData[i] = drv_gps_data_rxBuf[i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE];
    			drv_gps_data_rxBuf[i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE] = 0;
    			cnt2 = i + drv_gps_buf_start_index - DRV_GPS_RX_MAX_BUF_SIZE;
    		}
    	}
    	if((cnt2 + 1) < DRV_GPS_RX_MAX_BUF_SIZE)
    	{
    		drv_gps_buf_start_index = cnt2 + 1;
    	}
    	else
    	{
    		drv_gps_buf_start_index = 0;
    	}

    	dummy = strstr((char*)rxData,serchData);		// 取得したデータに「GPRMC」が含まれるか確認
    	if(dummy != 0)							// [GPRMC]が含まれる場合の処理
    	{
    		sprintf(gps_writeDataBuf,"%s\n",(dummy));	
    		if(drv_gps_time_sync_flag == 1)
    		{
        		gps_getTime(gps_writeDataBuf);
        		drv_gps_time_sync_flag = 0;
    		}

    		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);		// RTCから時刻情報を取得
    		latitude = gps_getLatitude(gps_writeDataBuf);		// 位置情報を取得
			longitude = gps_getLongitude(gps_writeDataBuf);     // 位置情報を取得
			gpsPos_data[0] = latitude;
			gpsPos_data[1] = longitude;
			gpsTime_data = sTime;
    		sprintf(gps_writeDataBuf,"%02d:%02d:%02d,%f,%f\n",sTime.Hours,sTime.Minutes,sTime.Seconds,longitude,latitude);

    		fs_file_open("gps_data_num.txt");							// ファイルをオープン
    		fs_file_write(gps_writeDataBuf,strlen(gps_writeDataBuf));	// ファイルを書き込み
    		fs_file_close();											// ファイルをクローズ
    	}
   }
    while(cnt != 0);
}

float gps_getLongitude(char *gpsData)
{
    char serchData[] = ",A,";
    char *dummy;
	char degr[4];
	float data;
	char minut[12];
	int i;

	dummy = strstr(gpsData,serchData) ;
	if(dummy == NULL)
	{
		return 0.0;
	}
	dummy = dummy + 3;

	degr[0] = dummy[0];
	degr[1] = dummy[1];
	degr[2] = 0;

	data = (float)atoi(degr);

	dummy = dummy + 2;
	for(i = 0; i<sizeof(minut) -2;i++)
	{
		if(*(dummy+i) == ',')
		{
			minut[i] = 0;
			break;
		}
		else
		{
			minut[i] = *(dummy + i);
		}
	}
	data = data + (atof(minut) / 60.0);

	return data;
}

float gps_getLatitude(char *gpsData)
{
    char serchData[] = ",N,";
    char *dummy;
	char degr[4];
	float data;
	char minut[12];
	int i;
	dummy = strstr(gpsData,serchData) ;
	if(dummy == NULL)
	{
		return 0.0;
	}

	dummy = dummy + 3;

	degr[0] = dummy[0];
	degr[1] = dummy[1];
	degr[2] = dummy[2];
	degr[3] = 0;

	data = (float)atoi(degr);

	dummy = dummy + 3;
	for(i = 0; i<sizeof(minut) -2;i++)
	{
		if(*(dummy+i) == ',')
		{
			minut[i] = 0;
			break;
		}
		else
		{
			minut[i] = *(dummy + i);
		}
	}
	data = data + (atof(minut) / 60.0);

	return data;

}


void gps_getTime(char *data)
{
	char hh[3];
	char mm[3];
	char ss[3];
	RTC_TimeTypeDef sTime;

	sTime.TimeFormat = RTC_HOURFORMAT_24;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ; // サマータイムではない
    sTime.StoreOperation = RTC_STOREOPERATION_RESET; // サマータイム設定に変更なし(？)

	hh[0] = *(data + 7);
	hh[1] = *(data + 8);
	hh[2] = 0;
	sTime.Hours = (uint8_t)atoi(hh);

	mm[0] = *(data + 9);
	mm[1] = *(data + 10);
	mm[2] = 0;
	sTime.Minutes = (uint8_t)atoi(mm);

	ss[0] = *(data + 11);
	ss[1] = *(data + 12);
	ss[2] = 0;
	sTime.Seconds = (uint8_t)atoi(ss);

	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}
