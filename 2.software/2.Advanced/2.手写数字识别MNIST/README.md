## ***手写数字识别 MNIST Demo***

该例程使用触摸屏p169h002-ctp，在LCD上进行手写数字，再进行手写数字的识别。



MNIST是一个非常经典的机器学习的入门案例，本demo将会使用keras进行训练，导出模型部署到FryPie炸鸡派上。

## 一、文件夹组成

```python
├─python_codes
│  │  mnist.npz
│  │  mnist.h5
│  │  train.py
│     
└─stm32_codes
    ├─MNIST
```

文件夹大致如上，`python_codes`存放的是：train.py用于训练网络，mnist.npz为网络上下载的MNIST数据集，用于网络的训练和测试，最后训练好的模型保存为mnist.h5。

## 二、触摸屏手写显示在LCD



## 三、数据处理，模型部署

