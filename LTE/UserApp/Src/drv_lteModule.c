/*
 * drv_lteModule.c
 *
 *  Created on: 2022/06/04
 *      Author: User
 */
#include "drv_lteModule.h"

#define LTE_APN_DATA_LENGTH			(32)				
#define LTE_PWR_ON_WAIT_TIME		(1800)
#define LTE_PWR_ONOFF_TIMEOUT		(1000)
#define LTE_PWR_ONOFF_WAIT_REPEAT	(1000)
#define LTE_UART_ONE_CMD_MAX_LEN	(1024)
#define LTE_UART_RX_DATA_BUF 		(LTE_UART_ONE_CMD_MAX_LEN * 4)
#define LTE_UART_TX_DATA_BUF 		(LTE_UART_ONE_CMD_MAX_LEN)
#define LTE_PWR_UP_WAIT_TIME		(8000)

extern uint16_t lte_buf_start_index;

uint8_t lte_txDataBuf[LTE_UART_TX_DATA_BUF];
uint8_t lte_rxDataBuf[LTE_UART_RX_DATA_BUF];
uint8_t lte_PC_txDataBuf[LTE_UART_TX_DATA_BUF + 8];
uint8_t lte_oneCmd_rxDataBuf[LTE_UART_ONE_CMD_MAX_LEN];

uint8_t lte_apn_name[LTE_APN_DATA_LENGTH];			// SIM APN Name Data
uint8_t lte_apn_user_name[LTE_APN_DATA_LENGTH];		// SIM User Name data
uint8_t lte_apn_pass[LTE_APN_DATA_LENGTH];			// SIM Pass data
uint8_t lte_carrier;								// LTE MNO Profile data

uint8_t lte_ponState = 0x00;

uint8_t lte_uart_start = 0x00;
uint16_t lte_buf_start_index = 0;

TIM_HandleTypeDef *lteModuleTimer;
UART_HandleTypeDef *lteModuleUart;

static uint16_t drv_ltModule_check_lf_data(void);

void drv_lteModule_set_timer(TIM_HandleTypeDef *timer)
{
	lteModuleTimer = timer;
}

void drv_lteModule_rxStart(void)
{
	int i;

	for(i = 0;i < sizeof(lte_rxDataBuf);i++)
	{
		lte_rxDataBuf[i] = 0;
	}

	lte_buf_start_index = 0;
    HAL_UART_Receive_DMA(lteModuleUart, lte_rxDataBuf, sizeof(lte_rxDataBuf));
}

// LTE Module startup sequence
void drv_lteModule_InitSeq(UART_HandleTypeDef *uart)
{
	lteModuleUart = uart; // set LTE UART PORT
	drv_lteModule_startSeq();
}

void drv_lteModule_startSeq(void)
{
	int i;

	drv_lteModule_bufferEn(ENABLE);	// Buffer IC Enable

	if(drv_ltModule_getV_int() == DISABLE)
	{
		drv_ltModule_pon_cnt();
		HAL_UART_Abort(lteModuleUart);
	}
	for(i = 0;i < LTE_PWR_ONOFF_WAIT_REPEAT;i++)
	{
		if(drv_lteModule_get_rxdPort_State() == ENABLE)
		{
			HAL_UART_Receive(lteModuleUart, lte_rxDataBuf, 1,100);
			break;
		}
		else
		{
			HAL_Delay(LTE_PWR_ONOFF_TIMEOUT);
		}
	}
	HAL_Delay(LTE_PWR_UP_WAIT_TIME);
	drv_lteModule_rxStart();
}

void drv_lteModule_stopSeq(void)
{
	int i;
	if(drv_ltModule_getV_int() == ENABLE)
	{
		drv_ltModule_pon_cnt();
		HAL_UART_Abort(lteModuleUart);
	}
	for(i = 0;i < LTE_PWR_ONOFF_WAIT_REPEAT;i++)
	{
		if(drv_ltModule_getV_int() == DISABLE)
		{
			break;
		}
		else
		{
			HAL_Delay(LTE_PWR_ONOFF_TIMEOUT);
		}
	}
	lte_buf_start_index = 0;
}


uint8_t drv_lteModule_get_rxdPort_State(void)
{
	return HAL_GPIO_ReadPin(LTE_UART_RXD_GPIO_Port, LTE_UART_RXD_Pin);
}


void drv_ltModule_pon_cnt(void)
{
	HAL_GPIO_WritePin(LTE_PWR_ON_GPIO_Port,LTE_PWR_ON_Pin,GPIO_PIN_RESET);
	HAL_Delay(LTE_PWR_ON_WAIT_TIME);
	HAL_GPIO_WritePin(LTE_PWR_ON_GPIO_Port,LTE_PWR_ON_Pin,GPIO_PIN_SET);
}

void drv_lteModule_bufferEn(uint8_t enable)
{
	if(enable == ENABLE)
	{
		HAL_GPIO_WritePin(LTE_BUF_EN_GPIO_Port,LTE_BUF_EN_Pin,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(LTE_BUF_EN_GPIO_Port,LTE_BUF_EN_Pin,GPIO_PIN_SET);
	}

}

// LTE V_INT Port state check
uint8_t drv_ltModule_getV_int(void)
{
	if(HAL_GPIO_ReadPin(LTE_V_INT_GPIO_Port, LTE_V_INT_Pin) == GPIO_PIN_RESET)
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

void drv_ltModule_pwrOnTimer_start(void)
{
	__HAL_TIM_CLEAR_IT(lteModuleTimer, TIM_IT_UPDATE);
	if(lteModuleTimer->State == HAL_TIM_STATE_READY)
	{
		HAL_TIM_Base_Start_IT(lteModuleTimer);
	    HAL_GPIO_WritePin(LTE_PWR_ON_GPIO_Port, LTE_PWR_ON_Pin, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	}
}
void drv_ltModule_pwrOnTimer_stop()
{
	uint8_t txData[1];
	HAL_TIM_Base_Stop_IT(lteModuleTimer);
    HAL_GPIO_WritePin(LTE_PWR_ON_GPIO_Port, LTE_PWR_ON_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    txData[0] = drv_ltModule_getV_int();
    if(txData[0] == 0x01)
    {

    }
	drv_pc_cmd_tx_ansync(LTE_PWR_ON,CMD_IND,txData,1);
}

void drv_ltModule_send_AtCmd(char *data,uint16_t len)
{
	uint16_t i;

	for(i = 0;i <len;i++)
	{
		lte_txDataBuf[i] = (uint8_t)data[i];
	}

	HAL_UART_Transmit_IT(lteModuleUart, lte_txDataBuf, len);
}

void drv_ltModule_Send_PC_responce(void)
{
	uint16_t cnt ;
	uint16_t i;
	uint16_t cnt2 = 0;
    int dummy,dummy2;

    lte_PC_txDataBuf[0] = 0x7E;

    lte_PC_txDataBuf[3] = CMD_IND;
    dummy = lte_PC_txDataBuf[3] ;

    lte_PC_txDataBuf[4] = MY_ADDRESS;
    dummy += lte_PC_txDataBuf[4] ;

    lte_PC_txDataBuf[5] = PC_ADDRESS;
    dummy += lte_PC_txDataBuf[5] ;

    lte_PC_txDataBuf[6] = LTE_SET_AT_CMD;
    dummy += lte_PC_txDataBuf[6];

    do
    {
    	dummy2 = dummy;
    	cnt = drv_ltModule_check_lf_data();
    	if(cnt == 0)
    	{
    		return;
    	}
    	lte_PC_txDataBuf[1] = (uint8_t)(((cnt + 6)  & 0xFF00) >> 8);
    	lte_PC_txDataBuf[2] = (uint8_t)((cnt + 6)  & 0x00FF) ;

    	for(i = 0;i <= cnt;i++)
    	{
    		if((i + lte_buf_start_index) < LTE_UART_RX_DATA_BUF)
    		{
    			lte_PC_txDataBuf[7 + i] =lte_rxDataBuf[i + lte_buf_start_index];
    		    dummy2 += lte_PC_txDataBuf[7 + i];
    			lte_rxDataBuf[i + lte_buf_start_index] = 0;
    			cnt2 = i + lte_buf_start_index;
    		}
    		else
    		{
    			lte_PC_txDataBuf[7 + i] = lte_rxDataBuf[i + lte_buf_start_index - LTE_UART_RX_DATA_BUF];
    		    dummy2 += lte_PC_txDataBuf[7 + i];
    			lte_rxDataBuf[i + lte_buf_start_index - LTE_UART_RX_DATA_BUF] = 0;
    			cnt2 = i + lte_buf_start_index - LTE_UART_RX_DATA_BUF;
    		}

    	}
    	lte_PC_txDataBuf[7 + i] = (uint8_t)(0xff - (dummy2));

    	if((cnt2 + 1) < LTE_UART_RX_DATA_BUF)
    	{
    		lte_buf_start_index = cnt2 + 1;
    	}
    	else
    	{
    		lte_buf_start_index = 0;
    	}

		HAL_UART_Transmit(uart_pc_if, lte_PC_txDataBuf, cnt + 9,10);
   }
    while(cnt != 0);
}

static uint16_t drv_ltModule_check_lf_data(void)
{
	uint16_t cnt = 0;
	uint16_t cnt2 = 0;
	uint16_t cnt3 = 0;

	if(lte_buf_start_index < LTE_UART_RX_DATA_BUF)
	{
		cnt2 = lte_buf_start_index;
	}
	else
	{
		cnt2 = 0;
		lte_buf_start_index = 0;;
	}
	while(lte_rxDataBuf[cnt + cnt2 - cnt3] != '\n')
	{
		if(lte_rxDataBuf[cnt + cnt2 - cnt3] == 0)
		{
			cnt = 0;
			break;
		}
		cnt++;
		if((cnt + cnt2) >= LTE_UART_RX_DATA_BUF )
		{
			cnt3 = LTE_UART_RX_DATA_BUF;
		}
		else if(cnt + cnt2 - cnt3>=LTE_UART_RX_DATA_BUF)
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
uint8_t drv_ltModule_cmd_respWait(char *rxData,char *checkStr)
{
	uint16_t cnt;
	uint16_t cnt2 = 0;
	uint8_t endFlag = 0;
	uint16_t timeoutCnt = 0;
	int i;

    do
    {
    	cnt = drv_ltModule_check_lf_data();		// バッファに存在するデータ数を確認
		if(cnt != 0)
		{
			for(i = 0;i <= cnt;i++)
			{
				if((i + lte_buf_start_index) < LTE_UART_RX_DATA_BUF)
				{
					lte_oneCmd_rxDataBuf[i] =lte_rxDataBuf[i + lte_buf_start_index];
					lte_rxDataBuf[i + lte_buf_start_index] = 0;
					cnt2 = i + lte_buf_start_index;
				}
				else
				{
					lte_oneCmd_rxDataBuf[i] = lte_rxDataBuf[i + lte_buf_start_index - LTE_UART_RX_DATA_BUF];
					lte_rxDataBuf[i + lte_buf_start_index - LTE_UART_RX_DATA_BUF] = 0;
					cnt2 = i + lte_buf_start_index - LTE_UART_RX_DATA_BUF;
				}
			}
			if((cnt2 + 1) < LTE_UART_RX_DATA_BUF)
			{
				lte_buf_start_index = cnt2 + 1;
			}
			else
			{
				lte_buf_start_index = 0;
			}
			if(strstr((char*)lte_oneCmd_rxDataBuf,checkStr) != 0)
			{
				sprintf(rxData,"%s",lte_oneCmd_rxDataBuf);
				endFlag = 1;
			}
		}
		timeoutCnt++;
		HAL_Delay(100);

		if(timeoutCnt > 1000)
		{
			return 0xFF;
		}
    }
    while(endFlag == 0);

	return 0x00;
}
void drv_ltModule_ucged_read(void)
{
	uint8_t endFlag = 0;
	uint16_t cnt,cnt2;
	uint16_t timeoutCnt = 0;
	int i;

	cnt2 = 0;
	do
	{
		cnt = drv_ltModule_check_lf_data();		// バッファに存在するデータ数を確認
		if(cnt != 0)
		{
			for(i = 0;i <= cnt;i++)
			{
				if((i + lte_buf_start_index) < LTE_UART_RX_DATA_BUF)
				{
					lte_oneCmd_rxDataBuf[i] =lte_rxDataBuf[i + lte_buf_start_index];
					lte_rxDataBuf[i + lte_buf_start_index] = 0;
					cnt2 = i + lte_buf_start_index;
				}
				else
				{
					lte_oneCmd_rxDataBuf[i] = lte_rxDataBuf[i + lte_buf_start_index - LTE_UART_RX_DATA_BUF];
					lte_rxDataBuf[i + lte_buf_start_index - LTE_UART_RX_DATA_BUF] = 0;
					cnt2 = i + lte_buf_start_index - LTE_UART_RX_DATA_BUF;
				}
			}
			if((cnt2 + 1) < LTE_UART_RX_DATA_BUF)
			{
				lte_buf_start_index = cnt2 + 1;
			}
			else
			{
				lte_buf_start_index = 0;
			}
			if(cnt > 12)
			{
				endFlag = 1;
			}
		}
		timeoutCnt++;
		HAL_Delay(10);

		if(timeoutCnt > 1000)
		{
			return;
		}
	}
	while(endFlag == 0);

}
