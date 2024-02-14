<p align="right">
  <b>中文</b> | <a href="./README_Eng.md">English</a> 
</p>


<h1 align="center">FryPi 炸鸡派</h1>

<div align=center>
        <img src=https://img.shields.io/badge/version-1.1-blue>
        <img src=https://img.shields.io/badge/License-GPL3.0-green)>
        <img src=https://img.shields.io/github/stars/No-Chicken/FryPie-F411DevKit.svg>
</div>

<h3 align="center">STM32F411RET6开发板</h3>

<p align="center">
	<img width="45%" src="./images/LVGL演示.gif">
    &nbsp;
    <img width="45%" src="./images/手势识别演示.gif">
</p>
<p align="center">
	<img width="45%" src="./images/MNIST演示.gif">
    &nbsp;
    <img width="45%" src="./images/在环开发.jpg">
</p>


<p align="center">
	<a href="https://space.bilibili.com/34154740">bilibili主页 </a> |
	<a href="https://www.bilibili.com/video/BV19g4y1N7YR/">演示视频 </a> |
    <a href="https://oshwhub.com/no_chicken">硬件开源 </a> 
</p>



## :ledger: 简介

这是一个比手掌还小的STM32F411RET6迷你开发板，可以用于AI开发，UI开发，数字电源控制板等等，甚至可以拿去用于你的毕业设计和其他相关的项目。

<p align="center">
	<img width="30%" src="./images/Core_V1.1后视图.png">
    &nbsp;
    <img width="30%" src="./images/Core_V1.1前视图.png">
</p>

最开始做这块板子的目的是因为上一个智能手表的项目[OV-Watch](https://github.com/No-Chicken/OV-Watch)，很多复刻的人说器件非常难焊接，二次开发不太方便，再加上我也打算在STM32上部署一下AI相关的东西，做下教程，因此，FryPi炸鸡派就诞生了。这一块开发板不仅适用于初学者，也适用于进阶开发者，项目中的高级例程可能需要一定的知识储备。

## :rocket: 功能特点

  - MCU使用STM32F411RET6，Cortex-M4 core with DSP and FPU，512 Kbytes of Flash memory, 100 MHz CPU, ART Accelerator，.
  - 可额外焊接外置SPI Flash.
  - 例程丰富：高级例程：[智能手表](https://github.com/No-Chicken/FryPi/tree/master/2.software/2.Advanced/6.LVGL-SmartWatch)，[热成像手势识别](https://github.com/No-Chicken/FryPi/tree/master/2.software/2.Advanced/3.Thermal-camera-gesture-recognition)，[手写数字识别](https://github.com/No-Chicken/FryPi/tree/master/2.software/2.Advanced/2.%20MNIST-Handwritten-number-recognition)，与matlab联合开发，[simulink在环开发](https://github.com/No-Chicken/FryPi/tree/master/2.software/2.Advanced/5.simulink-Co-develope)等等.
  - 留有端口可外接扩展板.（例如最上面的演示动态图，都是在Core板插上Cam扩展板）
  - 有双TypeC和单TypeC版本的Core板.

## :package: 开机测试

开发板焊接完毕拿到手后，烧录[模板例程](https://github.com/No-Chicken/FryPi/tree/master/2.software/1.Basic/0.template)观察现象，如果硬件没有问题，那么插串口对应的TypeC连接电脑，L2会闪烁，按下Key切换模式，L2闪烁频率会改变，同时上位机会接收到模式的信息；

<div align=center>
	<img  width="50%" src="./2.software/1.Basic/0.template/images/开机测试.gif" />
    &nbsp;
    <img src="./2.software/1.Basic/0.template/images/上位机.jpg" width="24%"/>
</div>
如果插开发板的USB对应的TypecC连接电脑，电脑会出现格式化U盘，然后格式化完后，会模拟出一个U盘。

<div align=center>
	<img src="./2.software/1.Basic/0.template/images/格式化.jpg" width="35%"/>
</div>
详细的细节详见例程：[模版工程](https://github.com/No-Chicken/FryPi/tree/master/2.software/1.Basic/0.template)项目。

## ✨贡献者

<table>
  <tbody>
    <tr>
      <td align="center" valign="top" width="16.66%">
          <img src="https://avatars.githubusercontent.com/u/85883051?v=4" width="100px;" alt="kingham"/><br /><sub><b>不吃油炸鸡</b></sub></a><br /><a title="Code">💻</a> <a title="Documentation">📖</a> <a title="Tests">⚠️</a> <a title="Reviewed Pull Requests">👀</a></td>
<td align="center" valign="top" width="16.66%">
          <img src="https://avatars.githubusercontent.com/u/109140632?v=4" width="100px;" alt="lantian"/><br /><sub><b>花生大少</b></sub></a><br /> <a title="Code">💻</a> <a title="Tests">⚠️</a> <a title="Ideas, Planning, & Feedback">🤔</a> <a title="Bug reports">🐛</a></td>
    </tr>
  </tbody>
</table>
