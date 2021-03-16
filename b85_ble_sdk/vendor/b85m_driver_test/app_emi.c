/********************************************************************************************************
 * @file     app_emi.c 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     May. 12, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/
/*
 * app_emi.c
 *
 *  Created on: 2018-9-21
 *      Author: Administrator
 */
#include "tl_common.h"
#include "drivers.h"



unsigned char  mode=1;//1
unsigned char  power_level = 0;
unsigned char  chn = 2;//2
unsigned char  cmd_now=1;//1
unsigned char  run=1;
unsigned char  tx_cnt=0;
unsigned char  hop=0;






struct  test_list_s {
	unsigned char  cmd_id;
	void	 (*func)(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);
};

void emicarrieronly(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);
void emi_con_prbs9(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);
void emirx(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);
void emitxprbs9(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);
void emitx55(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);
void emitx0f(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);
void emi_con_tx55(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);
void emi_con_tx0f(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn);

void emi_deepio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_deepio_ren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_deeptimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void emi_deeptimer_ren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void emi_suspendio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_suspendtimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void led_init(void);
void key_init(void);
void key_serviceloop(void);
void led_serviceloop(void);

struct  test_list_s  ate_list[] = {
		{0x01,emicarrieronly},
		{0x02,emi_con_prbs9},
		{0x03,emirx},
		{0x04,emitxprbs9},
		{0x05,emitx55},
		{0x06,emitx0f},
};



void app_emi_init(void)
{
	write_reg32(0x408,0x29417671 );//access code  0xf8118ac9

	write_reg8(0x40005,tx_cnt);//tx_cnt
	write_reg8(0x40006,run);//run
	write_reg8(0x40007,cmd_now);//cmd
	write_reg8(0x40008,power_level);//power
	write_reg8(0x40009,chn);//chn
	write_reg8(0x4000a,mode);//mode
	write_reg8(0x4000b,hop);//hop
	write_reg8(0x40004,0);
	write_reg32(0x4000c,0);
}



void app_rf_emi_test_start(void)
{
	unsigned char i=0;
	while(1)
	{
	   run = read_reg8(0x40006);  // get the run state!
	   if(run!=0)
	   {
		   power_level = read_reg8(0x40008);
		   chn = read_reg8(0x40009);
		   mode=read_reg8(0x4000a);
		   cmd_now = read_reg8(0x40007);  // get the command!
		   tx_cnt = read_reg8(0x40005);
			for (i=0; i<sizeof (ate_list)/sizeof (struct test_list_s); i++)
			{
				if(cmd_now == ate_list[i].cmd_id)
				{
					if(mode==0)//ble 2M mode
					{
						ate_list[i].func(RF_MODE_BLE_2M,power_level,chn);
					}
					else if(mode==1)//ble 1M mode
					{
						ate_list[i].func(RF_MODE_BLE_1M,power_level,chn);
					}
					else if(mode==2)//zigbee mode
					{
						ate_list[i].func(RF_MODE_ZIGBEE_250K,power_level,chn);
					}
					else if(mode==3)//BLE125K mode
					{
						ate_list[i].func(RF_MODE_LR_S8_125K,power_level,chn);
					}
					else if(mode==4)//BLE500K mode
					{
						ate_list[i].func(RF_MODE_LR_S2_500K,power_level,chn);
					}
					break;
				}
			}
			run = 0;
			write_reg8(0x40006, run);
	   }
	}

}



void emicarrieronly(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn)
{
	rf_emi_single_tone(pwr,rf_chn);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ));
	rf_emi_stop();
}

void emi_con_prbs9(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn)
{
	rf_emi_tx_continue_setup(rf_mode,pwr,rf_chn,0);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	{
		rf_continue_mode_run();
	}
	rf_emi_stop();

	write_reg16(0x60, 0x480);
	write_reg8(0xc24, 0x0);
	write_reg8(0xc0e, 0x60);
	write_reg16(0x60, 0x0);

}

void emirx(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn)
{
	rf_emi_rx(rf_mode,rf_chn);
	write_reg8(0x40004,0);
	write_reg32(0x4000c,0);

	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	{
		rf_emi_rx_loop();
		if(rf_emi_get_rxpkt_cnt()!=read_reg32(0x4000c))
		{
			write_reg8(0x40004,rf_emi_get_rssi_avg());
			write_reg32(0x4000c,rf_emi_get_rxpkt_cnt());
		}
	}
	rf_emi_stop();
}

void emitxprbs9(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
	rf_emi_tx_burst_setup(rf_mode,pwr,rf_chn,0);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode  && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
		rf_emi_tx_burst_loop(rf_mode,0);
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}


void emitx55(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
//	rf_emi_tx_burst_setup_ramp(rf_mode,pwr,rf_chn,2);
	rf_emi_tx_burst_setup(rf_mode,pwr,rf_chn,2);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
		rf_emi_tx_burst_loop(rf_mode,2);
//		rf_emi_tx_burst_loop_ramp(rf_mode,2);
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}

void emitx0f(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
	rf_emi_tx_burst_setup(rf_mode,pwr,rf_chn,1);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
		rf_emi_tx_burst_loop(rf_mode,1);
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}

void emi_con_tx55(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn)
{
	rf_emi_tx_continue_setup(rf_mode,pwr,rf_chn,2);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	rf_continue_mode_run();
	rf_emi_stop();
}



void emi_con_tx0f(RF_ModeTypeDef rf_mode,RF_PowerTypeDef pwr,signed char rf_chn)
{
	rf_emi_tx_continue_setup(rf_mode,pwr,rf_chn,1);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	rf_continue_mode_run();
	rf_emi_stop();
}
