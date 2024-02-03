## ***Thermal Camera Demo***

该例程使用MLX90640ESF-BAA模块热成像模块，进行热成像在LCD上显示，同时使用keras框架训练了一个CNN模型进行手势识别。

<img src="./images/手势识别测试.gif"/>

## 一、文件夹组成

```c
├─python_codes
│  │  data_2_imgfile.py
│  │  data_get.py
│  │  data_show.py
│  │  gesture.h5
│  │  test.py
│  │  train.py
│  │  
│  └─camera_data
│          test_data.npz
│          train_data.npz
│          
└─stm32_codes
    ├─ThermalCamera_data_send
    ├─Thermalgesture
```

文件夹大致如上，`python_codes`存放的是：从下位机获取数据、网络训练、测试的代码等。采集的热成像数据保存在.npz格式的文件中，用于网络的训练和测试，模型保存为.h5格式。

`stm32_codes`存放的是STM32的代码，第一个是LCD刷屏+串口发送炸鸡派获得的热成像原始数据。第二个则是已经部署好的热成像手势识别的代码。

## 二、热成像在LCD显示

直接使用提供的MLX90640API，然后对应更改了IO口即可，同时为了让刷屏更快，在`mlx90640_display_process(void)`函数中将`drawPicture(void)`中刷屏显示使用缓存空间进行刷屏，因为打点刷屏太慢了。

<img src="./images/MLXBSP.jpg"/>

```c
static void drawPicture(void) {

  static uint8_t lcd_buf[LCD_Buf_Size];//able to store 32*8 * 6*8 pixes
  uint16_t color;
  uint16_t indexes=0;
  uint8_t refreshtimes=0;
  uint8_t data[2];
  LCD_Address_Set(12+20 , 24 , 12+20+32*8-1, 24+24*8-1);//设置显示范围
  //pix_horizontal
	for (y=0; y<24; y++) {
    //pix_vertical
		for (x=0; x<32; x++) {
      //FillRect(Xpos, Ypos, Width, Height, RGBCode);

			//LCD_Fill(12 + x*8, 24 + y*8, 12 + x*8 + 8, 24 + y*8 + 8, TempToColor(tempValues[(31-x) + (y*32)]));
      uint16_t index_sta = 32*8*8*2*y + 8*2*x - refreshtimes*(LCD_Buf_Size);
      color = TempToColor(tempValues[(31-x) + (y*32)]);
      data[0] = color>>8;
      data[1] = color&0xff;
      for(uint8_t i=0; i<8 ; i++) {
        uint16_t line_gap = 32*8*2*i; 
        for(uint8_t j=0; j<8 ; j++) {
          lcd_buf[index_sta + line_gap + 2*j + 0] = data[0];
          lcd_buf[index_sta + line_gap + 2*j + 1] = data[1];
          indexes += 1;
        }
      }
     
      if(indexes>=LCD_Buf_Size/2) {
        indexes = 0;
        refreshtimes+=1;
        if(HAL_SPI_Transmit(&hspi1, lcd_buf, LCD_Buf_Size, 0))
	      {printf("\r\nspi erro\r\n");}	
      }
		}
	}
}
```

## 三、卷积神经网络搭建与部署

### 1、CNN搭建与训练

搭建的卷积神经网络如下图所示，训练集大概有4000多张图片数组数据，`num_epochs` 设置为了50，`batch_size` 设置为64，Adam优化器`learning_rate` 设置为0.01。最后Acc大概可以超过0.9。具体详细的网络搭建详见`./python_codes/train.py`。

<img src="./images/模型.jpg"/>

使用keras搭建CNN模型如下所示，其他相关代码详见python代码。

```python
#------------------------------【搭模型】---------------------------------
model = tf.keras.models.Sequential([
    tf.keras.layers.Conv2D(filters=5, kernel_size=(5, 5), padding='valid', activation=tf.nn.relu, input_shape=(24, 32, 1)),
    tf.keras.layers.MaxPool2D(pool_size=(2, 2), padding='same'),
    tf.keras.layers.Conv2D(filters=3, kernel_size=(3, 3), padding='valid', activation=tf.nn.relu, input_shape=(10, 14, 5)),
    tf.keras.layers.MaxPool2D(pool_size=(2, 2), padding='same'),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(units=32, activation=tf.nn.relu),
    tf.keras.layers.Dense(units=16, activation=tf.nn.relu),
    tf.keras.layers.Dense(units=6, activation=tf.nn.softmax)
])

model.summary()
```

### 2、CNN部署至STM32

与之前第1节的cubeAI使用例程demo一样，首先将训练好得到的`gesture.h5`模型使用cubemx AI工具生成代码后，调用API，这里主要使用的`ai_mnetwork_run()`，具体如下代码块中函数`user_ai_run(const ai_float *in_data, ai_float *out_data)`所示。由于CubeMX生成代码选择的systemperform，所以要注释`MX_X_CUBE_AI_Process()`，这个里面主要是用来测试模型的性能，他默认使用的随机数填充输入。

<img src="./images/手势识别测试.gif"/>

```c
uint8_t user_ai_run(const ai_float *in_data, ai_float *out_data)
{
    int idx = 0;
    int batch = 0;
    ai_buffer ai_input[AI_MNETWORK_IN_NUM];
    ai_buffer ai_output[AI_MNETWORK_OUT_NUM];

    ai_float input[1] = {0};  
    ai_float output[1] = {0};

    if (net_exec_ctx[idx].handle == AI_HANDLE_NULL)
    {
        printf("E: network handle is NULL\r\n");
        return -1;
    }

	ai_input[0] = net_exec_ctx[idx].report.inputs[0];
    ai_output[0] = net_exec_ctx[idx].report.outputs[0];
		
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    batch = ai_mnetwork_run(net_exec_ctx[idx].handle, ai_input, ai_output);
    if (batch != 1) {
      aiLogErr(ai_mnetwork_get_error(net_exec_ctx[idx].handle),
          "ai_mnetwork_run");
      return -2;
    }

    return 0;

}
```

注意，这里我给到输入的数据，还是正常做了归一化的，如下代码所示。

```c
static void normalizeArray()
{
  float range = maxTemp - minTemp;
  for(uint16_t i=0; i<24*32; i++) {
    normalizetempValues[i] = (tempValues[i] - minTemp) / range;
  }
}
```

输出的数据格式为一个长度6的数组，分别对应无手势和手势1~5，类别判定条件为选择最大的数，即判哪个数最大，其对应的手势判断为当前手势，以下代码为寻找最大数对应的index。

```c
static uint8_t findMaxIndex(float arr[], uint8_t size) {
    if (size <= 0) {
        // 处理空数组或无效大小的情况
        return -1;
    }

    uint8_t maxIndex = 0;  // 假设最大值的索引为第一个元素的索引

    for (int i = 1; i < size; ++i) {
        // 检查当前元素是否大于当前最大值
        if (arr[i] > arr[maxIndex]) {
            maxIndex = i;  // 更新最大值的索引
        }
    }

    return maxIndex;
}
```

最后给出整个一轮识别的流程如下所示。

```c
normalizeArray();//先归一化
if(user_ai_run(normalizetempValues, outputs))//将归一化数据代入，正向运算
{
	printf("\r\n run erro \r\n");
}
uint8_t temp = findMaxIndex(outputs, sizeof(outputs) / sizeof(outputs[0]));//寻找最大值的索引
printf("\r\npredict gesture:%d\r\n", temp);//打印出判断的手势
```

