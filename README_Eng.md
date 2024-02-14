<p align="right">
  <a href="./README.md">中文</a> | <b>English</b> 
</p> 


<h1 align="center">FryPi</h1>

<div align=center>
        <img src=https://img.shields.io/badge/version-1.1-blue>
        <img src=https://img.shields.io/badge/License-GPL3.0-green)>
        <img src=https://img.shields.io/github/stars/No-Chicken/FryPie-F411DevKit.svg>
</div>

<h3 align="center">STM32F411RET6 DevKit</h3>

<p align="center">
	<img border="1px" width="50%" src="./images/LVGL演示.gif"><img border="1px" width="50%" src="./images/手势识别测试.gif">
</p>
<p align="center">
	<img border="1px" width="50%" src="./images/MNIST演示.gif"><img border="1px" width="50%" src="./images/在环开发.jpg">
</p>


<p align="center">
	<a href="https://space.bilibili.com/34154740">Bilibili </a> |
	<a href="https://www.bilibili.com/video/BV19g4y1N7YR/">video </a> |
    <a href="https://oshwhub.com/no_chicken">hardware </a> 
</p>


## :ledger: Overview

This is a STM32F411RET6 mini development board, which can be used for AI development, UI development, digital power control board, etc. And can even be used for your other projects.

<p align="center">
	<img width="30%" src="./images/Core_V1.1后视图.jpg"><img width="30%" src="./images/Core_V1.1前视图.jpg">
</p>

The original purpose of making this board is because of the previous smartwatch project *[OV-Watch](https://github.com/No-Chicken/OV-Watch)*，many copy people said that the device is very difficult to weld, secondary development is not convenient, In addition, I also plan to deploy some AI-related things on STM32 and do a tutorial, so the FryPi fried chicken pie was born.

## :rocket: Features

  - MCU use STM32F411RET6, Cortex-M4 core with DSP and FPU, 512 Kbytes of Flash memory, 100 MHz CPU, ART Accelerator.
  - External SPI Flash can be welded in addition.
  - Rich Demo: smart watch, thermal imaging gesture recognition, handwritten digit recognition, joint development with matlab, simulink HIL development and so on.
  - Leave the port for external expansion board. (For example, the top demo .gif showed is pluged the Cam expansion board into the Core board)
  - There are dual TypeC and single TypeC versions of the Core board.
  - 
