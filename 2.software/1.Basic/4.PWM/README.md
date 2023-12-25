## ***PWM Demo***



### 1. 示例一：PWM_base例程

cubemx配置好后，在main函数中启动pwm。这里使用的是TIM2 channel3。

```c
//Start the pwm
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);//PA2
```

如果要更改占空比，更改`MX_TIM2_Init()`中的pulse即可，这里预分频设置为99，那么每加一个count则需要1÷(100÷(99+1))s，为1MHz，1us，这里再设置`htim2.Init.Period`为999，那么定时器的周期则为1ms了。

`sConfigOC.Pulse = 300`为设置载波的数值，调节此处可以调节占空比。原理详见手册。

```c
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 99;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 300;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}
```

第一张图是pulse设置为300，第二张图是设置为800。

<div align=center>
<img src=".\images\pulse=300.jpg" alt="pulse=300" style="zoom:100%;" />
</div>

<div align=center>
<img src=".\images\pulse=800.jpg" alt="pulse=800" style="zoom:100%;" />
</div>

### 2.示例二： PWM互补输出，带死区

详细设置见代码和cubemx工程，下图为带死区的PWM互补输出。

<div align=center>
<img src=".\images\22.jpg" alt="22" style="zoom:100%;" />
</div>

<div align=center>
<img src=".\images\21.jpg" alt="21" style="zoom:100%;" />
</div>

在`MX_TIM1_Init()`中设置如下代码，即可更改死区时间

```c
sBreakDeadTimeConfig.DeadTime = 20;
```

死区的计算方式如下图所示，详细见参考手册。

<div align=center>
<img src=".\images\DTS.jpg" alt="DTS" style="zoom:100%;" />
</div>

### 3. 作业

这里仅展示了PWM的部分功能，更多详细的可以看官方的手册和官方的例程。

感兴趣可以尝试下如何生成SPWM，完成呼吸灯的效果。