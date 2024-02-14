/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "spi.h"
#include "gpio.h"
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "delay.h"
#include "lcd.h"
#include "lcd_init.h"
#include "CST816.h"
#include "key.h"
#include "user_ai_run.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SCALL_RATE 8

// 原始图片尺寸
#define ORIGINAL_SIZE 240*240

// 压缩后图片尺寸
#define COMPRESSED_SIZE 30*30

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void downsample(uint8_t * originalImage, float * compressedImage) 
{
    for (int i = 0; i < COMPRESSED_SIZE; i++) {
        // 计算原始图片区域的左上角坐标
        int start_x = (i % 30) * (8); // 每行240个像素压缩成30个像素
        int start_y = (i / 30) * (8); // 每列240个像素压缩成30个像素

        // 计算原始图片区域的右下角坐标
        int end_x = start_x + 8; // 30 / 240 = 0.125，8 = 240 * 0.125
        int end_y = start_y + 8;

        // 计算原始图片区域内像素的平均值
        int sum = 0;
        for (int x = start_x; x < end_x; x++) {
            for (int y = start_y; y < end_y; y++) {
                sum += originalImage[x + y * 240];
            }
        }
        compressedImage[i] = sum / 64; // 8*8=64
				//norm
				compressedImage[i] /= 255;
    }
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_CRC_Init();
  MX_X_CUBE_AI_Init();
  /* USER CODE BEGIN 2 */

	delay_init();
	
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString(LCD_W/2-40,LCD_H/2,(uint8_t*)"Welcome!",WHITE,BLACK,24,0);//12*6,16*8,24*12,32*16
	LCD_Fill(0,0,LCD_W,LCD_H-40,WHITE);
	
	extern CST816_Info CST816_Instance;
	CST816_Init();
	CST816_RESET();
	
	Key_Port_Init();
	
  static uint8_t reg_state = 0;
  
  // 原始图片数据
  static uint8_t originalImage[ORIGINAL_SIZE];
  memset(originalImage, 0, ORIGINAL_SIZE);
  // 压缩后图片数据
  static float compressedImage[COMPRESSED_SIZE];
  memset(compressedImage, 0.0, COMPRESSED_SIZE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    //MX_X_CUBE_AI_Process();
    /* USER CODE BEGIN 3 */
		if(CST816_Get_FingerNum()!=0x00 && CST816_Get_FingerNum()!=0xFF)
		{
      if(!reg_state)
      {
        CST816_Get_XY_AXIS();
        if(CST816_Instance.X_Pos > SCALL_RATE/2 && CST816_Instance.Y_Pos > SCALL_RATE/2)
        {
          for(uint8_t i=0; i<SCALL_RATE; i++)
          {
            for(uint8_t j = 0; j < SCALL_RATE; j++)
            {
              originalImage[(CST816_Instance.X_Pos - SCALL_RATE/2 + j) + (CST816_Instance.Y_Pos - SCALL_RATE/2 + i)*240] = 0xFF;
            }
          }
          LCD_Fill(CST816_Instance.X_Pos - SCALL_RATE/2, CST816_Instance.Y_Pos - SCALL_RATE/2,
                  CST816_Instance.X_Pos + SCALL_RATE/2, CST816_Instance.Y_Pos + SCALL_RATE/2, BLACK);
        }
      }	
		}
		if(KeyScan(0))
		{
      
      if(reg_state == 1)
      {reg_state = 0;}
      else
      {reg_state = 1;}
      if(reg_state)
      {
        //pooling
        downsample(originalImage, compressedImage);
				
        //recongniztion
        float outputs[10]; //0~9
				user_ai_run(compressedImage, outputs);
        uint8_t number;
        number = findMaxIndex(outputs, 10);
        printf("the number is: %d\r\n", number);
        uint8_t strbuf[10];
        sprintf(strbuf, "number:%d", number);
        LCD_ShowString(60, 260, strbuf, WHITE, BLACK, 32, 0);
        
        //clear mem
        memset(originalImage, 0, sizeof(originalImage));
        memset(compressedImage, 0.0, sizeof(compressedImage));
      }
      else
      {
        //clear lcd
			  LCD_Fill(0,0,LCD_W,LCD_H-40,WHITE);
        LCD_Fill(0,LCD_H-40,LCD_W,LCD_H,BLACK);
				
      }
      HAL_GPIO_TogglePin(LED_T_GPIO_Port,LED_T_Pin);
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
