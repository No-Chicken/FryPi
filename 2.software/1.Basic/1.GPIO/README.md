### **this is a template project, using the CubeMX code frame**

## ***GPIO Demo***

*该例程对使用CubeMX初始化GPIO做了示范，GPIO使用HAL库进行GPIO编程分为以下几个步骤：*

## 一、例程简述

### 1. 包含必要的头文件和HAL库的相关头文件

CubeMX初始化会自动包含，对手敲HAL感兴趣的同学可以熟悉下生成的代码框架学习哦~

### 2. 初始化GPIO外设

    `GPIO_InitTypeDef GPIO_InitStructure;`

### 3. 配置GPIO引脚的模式和参数

```c
GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;    // 输出模式
GPIO_InitStructure.Pull = GPIO_NOPULL;            // 不使用内部上下拉电阻
GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;   // 低速
GPIO_InitStructure.Pin = GPIO_PIN_13;             // 配置的引脚
HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);        // 初始化GPIOC外设
```

### 4. 设置GPIO引脚的电平

```c
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // 设置引脚为高电平
```

### 5. 读取GPIO引脚的电平

```c
if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)   // 判断引脚是否为高电平
{
    // 做一些操作...
}
```

### **6. GPIO应用示例代码**

下面是一个简单的示例，说明如何使用HAL库配置一个GPIO引脚并控制其电平：

```c
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 初始化GPIOC外设
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

int main(void)
{
    // 初始化HAL库
    HAL_Init();

    // 配置GPIO引脚
    GPIO_Config();

    // 此处范例为500ms翻转一次PC13，对应炸鸡板的用户灯引脚，效果为闪光灯。
    while (1)
    {
        // 设置引脚为高电平
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

        // 延时一段时间
        HAL_Delay(500);

        // 设置引脚为低电平
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

        // 延时一段时间
        HAL_Delay(500);
    }
}
```

## 二、工程创建

***（仅本次进行完全示范，之后例程README.md仅演示新内容）***

### **1.打开CubeMX，选择ACCESS TO MCU SELECTOR**

<img src=".\Images\step1.png"/>

### **2.选择对应炸鸡板MCU--STM32F411RET6**

<img src=".\Images\step2.1.png"/>
<img src=".\Images\step2.2.png"/>

### **3.初始化时钟（配置时钟树）**

<img src=".\Images\step3.1.png"/>
<img src=".\Images\step3.2.png"/>
<img src=".\Images\step3.3.png"/>
<img src=".\Images\step3.4.png"/>
<img src=".\Images\step3.5.png"/>

### **4.初始化GPIO（用于控制炸鸡板用户灯）**

<img src=".\Images\step4.1.png"/>
<img src=".\Images\step4.2.png"/>
<img src=".\Images\step4.3.png"/>

### **5.生成初始化工程**

<img src=".\Images\step5.1.png"/>
<img src=".\Images\step5.2.png"/>
<img src=".\Images\step5.3.png"/>
<img src=".\Images\step5.4.png"/>

## 三、GPIO扩展知识（该部分摘自 [尝试使用CubeMX做stm32开发之四：GPIO配置](https://blog.csdn.net/changxiaoyong8/article/details/128176144)）

1）GPIO output level：只有当引脚设置为“GPIO output”时才需要设置

        High：GPIO输出初始化为高电平

        Low：GPIO输出初始化为低电平

2）GPIO mode

        Output Push Pull：推挽输出，能输出高低电平，且高低电平都有驱动能力。以PB13引脚为例，若需要通过其控制LED灯，则该引脚应配置为“Output Push Pull”模式，对应标准库函数中的“GPIO_Mode_Out_PP”

        Output Open Drain：开漏输出，只能输出低电平，需要借助外部上拉电阻才能输出高电平，对应标准库函数中的“GPIO_Mode_Out_OD”

        Analog mode：模拟输入，ADC采样信号输入引脚的配置模式，对应标准库函数中的“GPIO_Mode_AIN”

        Alternate Function Push Pull：推挽式复用功能，对应标准库函数中的“GPIO_Mode_AF_PP”

        Input mode：输入模式，配合No pull-up/pull-down可形成GPIO_Mpde_IN_FLOATING、GPIO_Mode_IPD、GPIO_Mode_IPU等不同工作模式

3）GPIO Pull-up/Pul-down

        No pull-up/pull-down：无内部上拉或下拉

        Pull-up：内部上拉

        Pull-down：内部下拉

4）Maximum output speed

        Low：低速，对应标准库函数中的“GPIO_Speed_2MHz”

        Medium：中速，对应标准库函数中的“GPIO_Speed_10MHz”

        High：高速，对应标准库函数中的“GPIO_Speed_50MHz”

5）User Label：用户标签，可以按照需要给引脚命名

普通GPIO口配置（用于控制LED灯）
<img src=".\Images\1.png"/>
ADC引脚配置
<img src=".\Images\2.png"/>
串行烧录引脚配置
<img src=".\Images\3.png"/>
TIM比较输出引脚配置
<img src=".\Images\4.png"/>
UART引脚配置
<img src=".\Images\5.png"/>
