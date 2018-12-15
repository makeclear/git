/*
 * led.c
 *
 *  Created on: 2018-9-26
 *      Author: ZJ_Kris
 */


//LED_YELLOW高电平亮
#ifndef _LED_C
#define _LED_C

#include "global.h"
#include "led.h"
#pragma CODE_SECTION(LedGetErrorCode, 				"ramfuncs");
#pragma CODE_SECTION(LedStatusFault, 				"ramfuncs");
#pragma CODE_SECTION(LedStatusInitialize, 			"ramfuncs");
#pragma CODE_SECTION(LedStatusAwait, 				"ramfuncs");
#pragma CODE_SECTION(LedStatusRun, 					"ramfuncs");
#pragma CODE_SECTION(LedStatusShow, 				"ramfuncs");

//########################################################################################
// Function Name:	LedShowErrorcode
// Input:			none
// Output:
// Description:		LED显示故障代码函数
// Execute Time:
//########################################################################################

void LedShowErrorCode(void)
{

	if(LedErrorCodeArray[LedErrorCodeNum] != 0)
	{
		LedStatusFault(LedErrorCodeArray[LedErrorCodeNum]);
	}
	else
	{
		LedErrorCodeNum++;
	}
	if(LedErrorCodeNum > 32)
	{
		LedErrorCodeNum = 0;
	}
}
//########################################################################################
// Function Name:	LedGetErrcode
// Input:			none
// Output:
// Description:		获取故障代码函数
// Execute Time:
//########################################################################################

 void LedGetErrorCode()
 {
	Uint16 i;
	if(errorCode.all[1]||errorCode.all[0])
	{
		for(i=0;i<16;i++)
		{
			if((errorCode.all[0]&(1<<i))!= 0)
			{
				LedErrorCodeArray[i] =  i+1;
			}
			if((errorCode.all[1]&(1<<i))!=0)
			{
				LedErrorCodeArray[i+16] =  i + 17;
			}
		}
	}


 }
//########################################################################################
// Function Name:	LedStatusInitialize
// Input:			none
// Output:
// Description:		LED初始化状态函数
// Execute Time:
//########################################################################################

void LedStatusInitialize(void)
{
#if 1
	/*LedErrorCodeArray[0] = 3;
	LedErrorCodeArray[8] = 8;
	LedErrorCodeArray[18] = 18;
	LedGetErrorCode();
	LedShowErrorCode();*/
	LED_GREEN_ON;			//LED灯长亮
	LED_YELLOW_ON;
#endif
}
//########################################################################################
// Function Name:	LedStatusAwait
// Input:			none
// Output:
// Description:		LED待机状态函数
// Execute Time:
//########################################################################################

void LedStatusAwait(void)
{
#if 1
	LedCount++;
	if(LedCount < 5)
	{
		LED_GREEN_ON;				//绿色灯亮
	}
	else
	{
		LED_GREEN_OFF;				//绿色灯灭
		if(LedCount > 9)
		{
			LedCount = 0;
		}
	}
#endif
}
//########################################################################################
// Function Name:	LedStatusRun
// Input:			none
// Output:
// Description:		LED运行状态函数
// Execute Time:
//########################################################################################

void LedStatusRun(void)
{
#if 1
	LedCount++;
	if(LedCount < 2)
	{
		LED_GREEN_ON;
	}
	else
	{
		LED_GREEN_OFF;
		if(LedCount > 4)
		{
			LedCount = 0;
		}
	}

#endif
}
//########################################################################################
// Function Name:	LedYellowLight
// Input:			none
// Output:
// Description:		LED黄色灯闪烁次数函数
// Execute Time:	0.15us
//########################################################################################

void LedYellowLight(Uint16 LightNum)
{
	if(LedYellowLowNum < LightNum)				//黄色灯闪烁两次，表示低位
	{
		if(LedCount < 4 )
		{
			LED_YELLOW_ON;				//黄色灯亮
		}
		else
		{
			LED_YELLOW_OFF;				//黄色灯灭
			if(LedCount > 8)
			{
				LedCount = 0;
				LedYellowLowNum++;
			}
		}
	}
}
//########################################################################################
// Function Name:	LedGreenLight
// Input:			none
// Output:
// Description:		LED绿色灯闪烁次数函数
// Execute Time:	0.45us
//########################################################################################

void LedGreenLight(Uint16 LightNum)
{
	if(LedGreenLowNum < LightNum )
	{
		if(LedCount < 3 )
		{
			LED_GREEN_ON;				//绿色灯亮
		}
		else
		{
			LED_GREEN_OFF;				//绿色灯灭
			if(LedCount > 6)
			{
				LedCount = 0;
				LedGreenLowNum++;
			}
		}
	}
}
//########################################################################################
// Function Name:	LedStatusFault
// Input:			none
// Output:
// Description:		LED故障状态函数
// Execute Time:	2.3us
//########################################################################################

void LedStatusFault(Uint16 Errorcodenum)
{
#if 1

	//Uint16 fault_high = errorInfo.code / 10;
	//Uint16 fault_low = errorInfo.code % 10;
	if(Errorcodenum != 0)
	{
		Uint16 ErrorCode_high = Errorcodenum / 10;	//故障码高位
		Uint16 ErrorCode_low = Errorcodenum % 10;	//故障码低位

		LedCount++;
		if(ErrorCode_high == 0)					//高位为0
		{
			LedYellowLight(LED_YELLOW_LOW);
			//黄色灯显示结束，绿色灯显示
			if(LedGreenLowNum < ErrorCode_low && LedYellowLowNum >= 2)
			{
				LedGreenLight(ErrorCode_low);
			}
		}
		else									//高位不为0
		{
			if(LedYellowHighNum < 1)			//黄色灯闪烁一次，表示高位
			{
				if(LedCount < 4)
				{
					LED_YELLOW_ON;				//黄色灯亮
				}
				else
				{
					LED_YELLOW_OFF;				//黄色灯灭
					if(LedCount > 8)
					{
						LedCount = 0;
						LedYellowHighNum++;
					}
				}
			}
			//LedYellowLight(LED_YELLOW_HIGH);
			//黄色灯显示结束，绿色灯显示，绿色灯闪烁次数表示故障码高位数字
			if(LedGreenHighNum < ErrorCode_high && LedYellowHighNum >= 1)
			{
				if(LedCount < 3)
				{
					LED_GREEN_ON;				//绿色灯亮
				}
				else
				{
					LED_GREEN_OFF;				//绿色灯灭
					if(LedCount > 6)
					{
						LedCount = 0;
						LedGreenHighNum++;
					}
				}
				//LedGreenLight(ErrorCode_high);
			}
			//高位显示结束，显示低位。黄色灯闪烁两次，表示低位
			if(LedYellowLowNum < 2 && LedGreenHighNum >= ErrorCode_high)
			{

				LedYellowLight(LED_YELLOW_LOW);
			}
			//黄色灯显示结束，绿色灯显示，绿色灯闪烁次数表示故障码低位数字
			if(LedGreenLowNum < ErrorCode_low && LedYellowLowNum >= 2)
			{
				LedGreenLight(ErrorCode_low);
			}
		}
		//故障码显示结束，清零
		if(((LedYellowLowNum >= 2) && (LedGreenLowNum >= ErrorCode_low))
			|| ((LedYellowLowNum >= 2) && (LedGreenLowNum >= ErrorCode_low)))
		{
			LedYellowHighNum = 0;
			LedYellowLowNum = 0;
			LedGreenLowNum = 0;
			LedGreenHighNum = 0;
			LedCount = 0;
			LedStatusFlag += 1;
			LedErrorCodeNum += 1;
		}
	}
#endif
}

//########################################################################################
// Function Name:	LedStatusShow
// Input:			none
// Output:
// Description:		LED状态显示函数
// Execute Time:
//########################################################################################

void LedStatusShow()
{
	if((sysStatus.MainStep == SYS_STATUS_TUNE )
		|| (sysStatus.MainStep == SYS_STATUS_POWERUP)
	 	||(sysStatus.MainStep == SYS_STATUS_POWERON))
		{
			LedStatusInitialize();
		}
	else if(sysStatus.MainStep == SYS_STATUS_READY)
		{
			LedStatusAwait();
		}
	else if(sysStatus.MainStep == SYS_STATUS_RUN)
		{
			LedStatusRun();
		}
	else if(sysStatus.MainStep == SYS_STATUS_FAULT)
		{
			LedGetErrorCode();
			LedShowErrorCode();

		}
	else
		{

		}
}


#endif





