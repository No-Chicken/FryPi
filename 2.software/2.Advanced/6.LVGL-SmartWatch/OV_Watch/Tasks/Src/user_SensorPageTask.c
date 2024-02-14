/* Private includes -----------------------------------------------------------*/
//includes
#include "user_TasksInit.h"
#include "user_ScrRenewTask.h"
#include "user_SensorPageTask.h"
#include "ui_HRPage.h"
#include "ui_SPO2Page.h"
#include "ui_ENVPage.h"
#include "ui_CompassPage.h"
#include "main.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t user_HR_timecount=0;

/* Private function prototypes -----------------------------------------------*/
extern uint8_t GET_BP_MAX (void);
extern uint8_t GET_BP_MIN (void);
extern void Blood_Process (void);
extern void Blood_50ms_process (void);
extern void Blood_500ms_process(void);
extern int em70xx_bpm_dynamic(int RECEIVED_BYTE, int g_sensor_x, int g_sensor_y, int g_sensor_z);
extern int em70xx_reset(int ref);

/**
  * @brief  HR data renew task
  * @param  argument: Not used
  * @retval None
  */
void HRDataRenewTask(void *argument)
{
	uint8_t value_strbuf[4];
	uint8_t IdleBreakstr=0;
	uint16_t dat=0;
	uint8_t hr_temp=0;
	while(1)
	{
		if(ScrRenewStack.Data[ScrRenewStack.Top_Point-1] == (long long int)&ui_HRPage)
		{
			osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);
			/*
			//sensor wake up
			EM7028_hrs_Enable();
			//receive the sensor wakeup message, sensor wakeup
			if(!Sensor_EM_Erro)
			{
				//Hr messure
				vTaskSuspendAll();
				hr_temp = HR_Calculate(EM7028_Get_HRS1(),user_HR_timecount);
				xTaskResumeAll();
				if(ui_HRValue != hr_temp && hr_temp>50 && hr_temp<120)
				{
					//set text
					ui_HRValue = hr_temp;
					sprintf(value_strbuf, "%d", ui_HRValue);
					lv_label_set_text(ui_HRPageNumLabel, value_strbuf);
				}
			}
			*/
		}
		osDelay(50);
	}
}


/**
  * @brief  Sensor data renew task
  * @param  argument: Not used
  * @retval None
  */
void SensorDataRenewTask(void *argument)
{
	uint8_t value_strbuf[6];
	uint8_t IdleBreakstr=0;
	while(1)
	{
		if(ScrRenewStack.Data[ScrRenewStack.Top_Point-1] == (long long int)&ui_SPO2Page)
		{
			osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);
			//sensor wake up
			
			

		}
		else if(ScrRenewStack.Data[ScrRenewStack.Top_Point-1] == (long long int)&ui_CompassPage)
		{
			osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);
			
		}
		osDelay(300);
	}
}
