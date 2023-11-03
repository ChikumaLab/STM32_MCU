/*
 * drv_ir_cnt.c
 *
 *  Created on: 2023/11/01
 *      Author: User
 */

/*
 * drv_ir_cnt.c
 *
 *  Created on: 2022/01/03
 *      Author: User
 */

#include "drv_ir_cnt.h"

uint8_t ir_State;
uint8_t ir_cnt;
uint8_t ir_t_cnt;
uint8_t ir_t_readerCode_on_cnt;
uint8_t ir_t_readerCode_off_cnt;

uint8_t ir_t_data_on_cnt;
uint8_t ir_t_data_off_cnt;
uint8_t ir_t_data_off_0_cnt;
uint8_t ir_t_data_off_1_cnt;
uint8_t ir_t_data_on_0_cnt;
uint8_t ir_t_data_on_1_cnt;

uint8_t *ir_txData;
uint16_t ir_txData_cnt;
uint16_t ir_txData_bit_cnt;
uint16_t ir_txData_len;

int max_ir_cnt;
TIM_HandleTypeDef *irTimer;
uint32_t irTimer_ch;

// NEC フォーマットパラメータ
#define	NEC_FORMAT_MAX_IR_CNT	(0x7)
#define NEC_READER_CODE_ON_CNT	(0x29)
#define NEC_READER_CODE_OFF_CNT	(0x14)
#define NEC_DATA_0_ON_CNT		(0x01)
#define NEC_DATA_0_OFF_CNT		(0x01)
#define NEC_DATA_1_ON_CNT		(0x01)
#define NEC_DATA_1_OFF_CNT		(0x07)

#define NEC_PWM_DUTY_ON			(0x46)


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

void ir_txInit(TIM_HandleTypeDef *htim,uint32_t timerCh,uint8_t mode)
{
	ir_State = 0;
	irTimer = htim;
	irTimer_ch = timerCh;
	if(mode == 0)
	{
		max_ir_cnt = NEC_FORMAT_MAX_IR_CNT;
		ir_t_readerCode_on_cnt = NEC_READER_CODE_ON_CNT;
		ir_t_readerCode_off_cnt = NEC_READER_CODE_OFF_CNT;
		ir_t_data_off_0_cnt = NEC_DATA_0_OFF_CNT;
		ir_t_data_off_1_cnt = NEC_DATA_1_OFF_CNT;
		ir_t_data_on_0_cnt = NEC_DATA_0_ON_CNT;
		ir_t_data_on_1_cnt = NEC_DATA_1_ON_CNT;
	}
	HAL_TIM_Base_Start(irTimer);
	HAL_TIM_PWM_Start_IT(irTimer, irTimer_ch);

}

void ir_dataTx(uint8_t *data,uint16_t len)
{
	ir_txData_len = len;
	ir_txData_cnt = 0;
	ir_txData_bit_cnt = 0;
	ir_txData = data;
	__HAL_TIM_SET_COMPARE(irTimer, irTimer_ch, NEC_PWM_DUTY_ON );
	HAL_TIM_Base_Start(irTimer);
	HAL_TIM_PWM_Start_IT(irTimer, irTimer_ch);
}

void drv_IR_int_task(void)
{
	__IO uint8_t buf = 0;

	if(ir_cnt < max_ir_cnt)
	{
		ir_cnt++;
	}
	else
	{
		ir_cnt =0;
		switch(ir_State)
		{
		case 0:
			if(ir_t_cnt < ir_t_readerCode_on_cnt)
			{
				ir_t_cnt++;
			}
			else
			{
				ir_t_cnt = 0;
				ir_State = 1;
				__HAL_TIM_SET_COMPARE(irTimer, irTimer_ch, 0 );
			}
			break;
		case 1:
			if(ir_t_cnt < ir_t_readerCode_off_cnt)
			{
				ir_t_cnt++;
			}
			else
			{
				ir_t_cnt = 0;
				ir_State = 2;
				if(buf == 0x01)
				{
					ir_t_data_off_cnt = ir_t_data_off_1_cnt;
					ir_t_data_on_cnt = ir_t_data_on_1_cnt;
				}
				else
				{
					ir_t_data_off_cnt = ir_t_data_off_0_cnt;
					ir_t_data_on_cnt = ir_t_data_on_0_cnt;
				}
				__HAL_TIM_SET_COMPARE(irTimer, irTimer_ch, NEC_PWM_DUTY_ON );
			}
			break;
		case 2:
			if(ir_t_cnt < ir_t_data_on_cnt)
			{
				ir_t_cnt++;
			}
			else
			{
				ir_t_cnt = 0;
				ir_State = 3;
				buf = ir_txData[ir_txData_cnt];
				buf = (buf >> ir_txData_bit_cnt) & 0x01;

				if(buf == 0x01)
				{
					ir_t_data_off_cnt = ir_t_data_off_1_cnt;
					ir_t_data_on_cnt = ir_t_data_on_1_cnt;
				}
				else
				{
					ir_t_data_off_cnt = ir_t_data_off_0_cnt;
					ir_t_data_on_cnt = ir_t_data_on_0_cnt;
				}
				if(ir_txData_bit_cnt < 7)
				{
					ir_txData_bit_cnt++;
				}
				else
				{
					ir_txData_cnt++;
					ir_txData_bit_cnt = 0;

				}
				__HAL_TIM_SET_COMPARE(irTimer, irTimer_ch, 0 );
			}

			break;
		case 3:
			if(ir_t_cnt < ir_t_data_off_cnt)
			{
				ir_t_cnt++;
			}
			else
			{
				ir_t_cnt = 0;
				if(ir_txData_cnt < ir_txData_len)
				{
					ir_State = 2;
					__HAL_TIM_SET_COMPARE(irTimer, irTimer_ch, NEC_PWM_DUTY_ON );
				}
				else
				{
					ir_State = 4;
					__HAL_TIM_SET_COMPARE(irTimer, irTimer_ch, NEC_PWM_DUTY_ON );
				}
			}
			break;
		case 4:
			if(ir_t_cnt < ir_t_data_on_cnt)
			{
				ir_t_cnt++;
			}
			else
			{
				HAL_TIM_PWM_Stop_IT(irTimer, irTimer_ch);
				HAL_TIM_Base_Stop(irTimer);
				ir_State = 0;
			}
			break;
		}
	}
}



void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
}

