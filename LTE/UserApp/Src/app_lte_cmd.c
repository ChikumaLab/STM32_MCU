/*
 * app_lte_cmd.c
 *
 *  Created on: 2022/06/16
 *      Author: User
 */


#include "app_lte_cmd.h"
#include <stdlib.h>

#define LTE_MAX_CMD_LENGTH		(512)

typedef struct 
{
	char serverName[32];
	uint16_t serverPort;
	char sendTopic[32];
	char sendData[512];
	char reciveTopic[32];
}MQTT_SERVER_PARAM;

typedef struct
{
	uint32_t eardcn;
	uint16_t band;
	uint8_t ul_bw;
	uint8_t dl_bw;
	int32_t rsrp;
	int32_t rsrq;
	int32_t sinr;
	int32_t rssi;
}LTE_RF_PARAM;

MQTT_SERVER_PARAM mqtt_param;
LTE_RF_PARAM rfParam;

void app_ltecmd_run(uint8_t *data,uint16_t len)
{
	uint16_t cmd;
	uint8_t *dummy;
	cmd = data[3];

	switch(cmd)
	{
	case LTE_SET_MODE:
		break;
	case LTE_GET_MODE:
		break;
	case LTE_SET_AT_CMD:
		dummy = &data[4];
		drv_ltModule_send_AtCmd((char* )dummy,len - 5);
		break;
	case LTE_CHECK_PWR:
		send_lteCheckPwr_cmd();
		break;
	case LTE_PWR_ON:	// DataGet all
		send_ltePwrOn_cmd(data[4]);
		break;
	case LTE_SET_DB:	// GyroDataGet
		break;
	case LTE_GET_DB:	// AcceleDataGet
		break;
	case SYS_GET_DB:	// MagDataGet
		break;
	case LTE_GET_RF_DATA:	// MagDataGet
		break;
	default:
		break;
	}
}

uint8_t app_set_mqtt_server(char *serverName,uint16_t port)
{
	char cmd[128];
	char rxBuf[256];

	sprintf(mqtt_param.serverName,"%s",serverName);
	mqtt_param.serverPort = port;

	sprintf(cmd,"AT+UMQTT=3,\"%s\",%d\r",mqtt_param.serverName,mqtt_param.serverPort);
	drv_ltModule_send_AtCmd(cmd,(uint8_t)strlen(cmd));
	drv_ltModule_cmd_respWait(rxBuf,"OK");

	return 0;
}

void app_set_mqtt_sendData(char *topic,char *data)
{
	sprintf(mqtt_param.sendTopic,"%s",topic);
	sprintf(mqtt_param.sendData,"%s",data);
}

uint8_t app_connect_mqtt_server(void)
{
	char cmd[] = {"AT+UMQTTC=1\r"};
	char rxBuf[256];
	uint8_t ret = 0;

	drv_ltModule_send_AtCmd(cmd,(uint16_t)strlen(cmd));
	drv_ltModule_cmd_respWait(rxBuf,"+UUMQTTC:");

	if(strstr(rxBuf,"+UUMQTTC: 1,1"))
	{
		ret = 0;
	}
	else
	{
		ret = 0xFF;
	}

	return ret;
}
uint8_t app_disconnect_mqtt_server(void)
{
	char cmd[] = {"AT+UMQTTC=0\r"};
	char rxBuf[256];
	uint8_t ret = 0;

	drv_ltModule_send_AtCmd(cmd,(uint16_t)strlen(cmd));
	if(drv_ltModule_cmd_respWait(rxBuf,"OK") == 0)
	{
		ret = 0;
	}
	else
	{
		ret = 0xFF;
	}

	return ret;
}
uint8_t app_send_mqtt_data(void)
{
	char cmd[567];
	char rxBuf[512];
	uint8_t ret = 0;

	sprintf(cmd,"AT+UMQTTC=2,0,0,0,\"%s\",\"%s\"\r",mqtt_param.sendTopic,mqtt_param.sendData);

	drv_ltModule_send_AtCmd(cmd,(uint16_t)strlen(cmd));
	drv_ltModule_cmd_respWait(rxBuf,"+UUMQTTC:");

	if(strstr(rxBuf,"+UUMQTTC: 2,1"))
	{
		ret = 0;

	}
	else
	{
		ret = 0xFF;
	}

	return ret;
}

uint8_t app_get_rssi(void)
{
	char cmd[64];
	char rxBuf[64];
	int dummy[2];

	sprintf(cmd,"AT+CSQ\r");
	drv_ltModule_send_AtCmd(cmd,(uint16_t)strlen(cmd));
	drv_ltModule_cmd_respWait(rxBuf,"+CSQ: ");
	sscanf(rxBuf,"%s %d,%d",cmd,&dummy[0],&dummy[1]);
	drv_ltModule_cmd_respWait(rxBuf,"OK");

	return (uint8_t)dummy[0];
}

uint8_t app_set_rf_param_init(void)
{
	char cmd[32];
	char rxBuf[32];

	sprintf(cmd,"AT+CEREG=2\r");
	drv_ltModule_send_AtCmd(cmd,(uint16_t)strlen(cmd));
	drv_ltModule_cmd_respWait(rxBuf,"OK");

	sprintf(cmd,"AT+UCGED=2\r");
	drv_ltModule_send_AtCmd(cmd,(uint16_t)strlen(cmd));
	drv_ltModule_cmd_respWait(rxBuf,"OK");

	return 0;
}

uint8_t app_get_bss_data(void)
{
	char cmd[32];
	char rxBuf[32];
	int dummy[32];

	sprintf(cmd,"AT+CEREG?\r");
	drv_ltModule_send_AtCmd(cmd,(uint16_t)strlen(cmd));
	drv_ltModule_cmd_respWait(rxBuf,"+CEREG: ");
	sscanf(rxBuf,"%s %d,%d",cmd,&dummy[0],&dummy[1]);

	sprintf(cmd,"AT+UCGED?\r");
	drv_ltModule_send_AtCmd(cmd,(uint16_t)strlen(cmd));
	drv_ltModule_cmd_respWait(rxBuf,"+UCGED:");

	return 0;
}

void send_lteCheckPwr_cmd(void)
{
	uint8_t data[1];

	data[0] = drv_ltModule_getV_int();
	drv_pc_cmd_tx_ansync(LTE_CHECK_PWR,CMD_RSP,data,1);
}
void send_ltePwrOn_cmd(uint8_t onOff)
{
	uint8_t data[1];

	data[0] = 0x01;

	if(drv_ltModule_getV_int() == 0x00)	// LTE Module OFF
	{
		if(onOff == 0x01)
		{
		    drv_lteModule_bufferEn(0x01);
		    drv_ltModule_pwrOnTimer_start();
		}
		else
		{
			drv_pc_cmd_tx_ansync(LTE_CHECK_PWR,CMD_RSP,data,1);
		}
	}
	else		//LTE Module ON
	{
		if(onOff == 0x00)
		{
		    drv_lteModule_bufferEn(0x00);
		    drv_ltModule_pwrOnTimer_start();
		}
		else
		{
			drv_pc_cmd_tx_ansync(LTE_CHECK_PWR,CMD_RSP,data,1);
		}
	}
}
