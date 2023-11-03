/**
 * @file main_app.c
 * @brief main app program file
 * @date  Sep 7, 2022
 * @author User
 */

#include "main_app.h"
#include <string.h>

// 静的関数のプロトタイプ宣言（このファイル内のみ）
static void app_function_init(void);
static void app_global_Value_init(void);
static void app_oneshot_Task_init(void);
static void app_oneshot_Task_run(void);

// global value
void (*app_singleTask[MAX_SINGLE_TASK])(void);
TIM_HandleTypeDef *Timer_500ms;
uint8_t app_reflash_Flag;
uint8_t app_clear_Flag;
void app_oled_view_lte(void);
uint32_t adcData[3];
float app_get_cpu_temp(uint16_t data);

extern float gpsPos_data[2];
extern RTC_TimeTypeDef gpsTime_data;


/**
 * メイン処理関数
 * この関数で永久ループする。
 */
void main_app(void)
{
	app_global_Value_init();				// global変数の初期化
	app_function_init();					// 起動時に実行が必要な関数の実行
	send_sysBoot_cmd();						// sysBootコマンドをPCに送信
	app_get_rssi();
	while(1)
	{
		app_oneshot_Task_run();				// PCから登録されたコマンドに応じた関数を処理
		MX_USB_HOST_Process();				// USB関連処理
		drv_ltModule_Send_PC_responce();	// LTE受信データをPCへ送信
//		app_gps_rxCmd_txPC();				// GPS受信データをPCへ送信
		app_gps_rxData_WriteFIle();			// GPS受信データの処理とUSBへの書き込み処理
	}
}
extern uint8_t lte_OnOffFlag;

void app_lte_StartStop(void)
{
	if(drv_ltModule_getV_int() == ENABLE)
	{
		drv_lteModule_stopSeq();
	}
	else
	{
		drv_lteModule_startSeq();
	}
	lte_OnOffFlag = 0;
}

void app_lte_comm_init(void)
{
	app_set_mqtt_server("133.242.211.199", 1883);
	app_connect_mqtt_server();
}

void app_mqtt_send_tempData(void)
{
	float bme280_getData[3];
	char txData[128];
	RTC_TimeTypeDef sTime;

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);		// RTCから時刻情報を取得

	stream_sensor_data_normal_mode(&dev1,bme280_getData);
	if(drv_ltModule_getV_int() == ENABLE)
	{
		sprintf(txData,"%02d:%02d:%02d,temp:%0.2f,press:%0.2f,hum:%0.2f",sTime.Hours,sTime.Minutes,sTime.Seconds,bme280_getData[0],bme280_getData[1],bme280_getData[2]);
		app_set_mqtt_sendData("/TEMP",txData);
		app_send_mqtt_data();
	}
}

void app_mqtt_send_gpsData(void)
{
	char txData[128];

	if(drv_ltModule_getV_int() == ENABLE)
	{
		sprintf(txData,"%02d:%02d:%02d,%f,%f",gpsTime_data.Hours,gpsTime_data.Minutes,gpsTime_data.Seconds,gpsPos_data[1],gpsPos_data[0]);
		app_set_mqtt_sendData("/GPS",txData);
		app_send_mqtt_data();
	}
}

#define ADC_VSENCE_25	(0.76)
#define ADC_AVG_SLOPE	(2.5)

float app_get_cpu_temp(uint16_t data)
{
	float buf;
	float temp;

	buf = data * (3.3 / 4096.0);

	temp = ((buf - ADC_VSENCE_25) / ADC_AVG_SLOPE) +25.0;

	return (temp);
}

/**
 * 関数の初期化処理
 */
static void app_function_init(void)
{
	drv_xbee_set_uartPort(&huart4);	// XBeeに接続しているUARTのポートを設定
	drv_cmd_if_set_Uart(&huart6);	// PCに接続するUARTポートを設定
	drv_lteModule_set_timer(&htim6);		// LTEモジュールのON/OFF制御用タイマーの設定
	drv_lteModule_InitSeq(&huart3);		// LTEモジュールに接続するUARTポートを設定し、LTEモジュールを起動する。
	drv_gps_init(&huart1);			// GPSモジュールに接続するUARTポートを設定する
	HAL_ADC_Start_DMA(&hadc1, adcData, 1);

	api_bme280_init(&hspi1,&dev1);	// 温湿度センサのポートを設定する。
	app_oled_view_init();			// OLEDディスプレイの画面表示を初期化する
	app_lte_comm_init();			// LTEモジュールMQTT接続設定

	drv_set_app_timer(&htim7);		// アプリケーション用の管理タイマーを設定
	drv_xbee_reset();				// Xbeeモジュールに対してReset信号をアサインする
	app_oneshot_Task_init();		// メイン処理で実行されるタスクを初期化（NULL）
}



/**
 * global変数の初期化設定
 */
static void app_global_Value_init(void)
{
	drv_gps_time_sync_flag = 1;		// GPSからの時刻同期を有効に設定する
}

/**
 * シングルタスクの追加処理
 * NULLになっている部分に関数を追加する。
 * すべてのタスクが埋まっていて設定できない場合は0xFFをエラーコードとして返す
 */
uint8_t app_add_onshot_Task (void *task)
{
	int i = 0;
	uint8_t ret = 0xFF;
	for(i = 0;i < MAX_SINGLE_TASK;i++)
	{
		if(app_singleTask[i] == NULL)
		{
			ret = 0x00;
			app_singleTask[i] = task;
			break;
		}
	}
	return ret;
}

/**
 * ワンショットタスクの初期化処理
 * 登録可能なタスクの中身をすべてNULLに初期化する
 */
void app_oneshot_Task_init(void)
{
	int i = 0;

	for(i = 0;i < MAX_SINGLE_TASK;i++)
	{
		app_singleTask[i] = NULL;
	}

}

/**
 * 登録されているタスクを実行し、処理が完了したら
 * 登録されているタスクを解除する。
 */
void app_oneshot_Task_run(void)
{
	int i;

	for(i = 0; i< MAX_SINGLE_TASK;i++)
	{
		if(app_singleTask[i] != NULL)
		{
			app_singleTask[i] ();
			app_singleTask[i] = NULL;
		}
	}
}

void app_oled_view_init(void)
{
	uint8_t strView_carr[] = {"-"};
	uint8_t strView_temp[] = {"TEMP:-deg"};
	uint8_t strView_humd[] = {"HUMD:-%"};
	uint8_t strView_press[] = {"PRES:-hPa"};

	drv_oled_ssd1306_init(&hi2c1);
	drv_oled_ssd1306_clear();
	drv_oled_ssd1306_set_string(0,2,strView_temp,sizeof(strView_temp)-1);
	drv_oled_ssd1306_set_string(0,4,strView_humd,sizeof(strView_humd)-1);
	drv_oled_ssd1306_set_string(0,6,strView_press,sizeof(strView_press)-1);
	drv_oled_ssd1306_set_string(12,0,strView_carr,sizeof(strView_carr)-1);
	drv_oled_ssd1306_set_ant_param(0);
	drv_oled_ssd1306_set_batt_param(0);
	drv_oled_ssd1306_set_time();

}

void app_oled_view_temp(void)
{
	float bme280_getData[3];

	stream_sensor_data_normal_mode(&dev1,bme280_getData);
	drv_oled_ssd1306_set_temp_param(bme280_getData[0]);
	drv_oled_ssd1306_set_humdity_param(bme280_getData[2]);
	drv_oled_ssd1306_set_presser_param(bme280_getData[1]);
	drv_oled_ssd1306_set_time();
}

void app_oled_view_lte(void)
{
	drv_oled_ssd1306_set_lte_qual(23,1,2, 3);
	drv_oled_ssd1306_set_lte_cellData(4,5);
	drv_oled_ssd1306_set_lte_band(6,7,8);

}
void drv_set_app_timer(TIM_HandleTypeDef *timer)
{
	Timer_500ms = timer;
	HAL_TIM_Base_Start_IT(Timer_500ms);
}
