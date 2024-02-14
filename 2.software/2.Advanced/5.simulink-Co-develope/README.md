## ***simulink hardware-in-the-loop(HIL) Demo***

​	*该例程使用simulink对STM32F4进行在环开发，功能实现分为以下几个步骤：*

## 一、simulink在环开发

**1.在matlab下载STM的硬件支持包，这里还有很多搭建环境的步骤就不一一说明了**

<div align=center>
<img src="./images/step1.png" alt="step1" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step2.png" alt="step2" style="zoom:100%;" />
</div>
**2.配置好所有环境后，创建simulink工程和cubemx工程。注意这里需要先配置好CubeMx和cubeprogramer**

<div align=center>
<img src="./images/step3.png" alt="step3" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step4.png" alt="step4" style="zoom:100%;" />
</div>

**3.在库中找到STM，搭建好模型，再在simulink进行设置，并指定刚才cubemx工程的路径**

<div align=center>
<img src="./images/step5.png" alt="step5" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step6.png" alt="step6" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step7.png" alt="step7" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step8.png" alt="step8" style="zoom:100%;" />
</div>

**4.编译+部署。有可能会出现一些cubemx的报错，按照提示排除就好**

<div align=center>
<img src="./images/step9.jpg" alt="step9" style="zoom:100%;" />
</div>



## 二、附加内容：simulink生成算法代码，嵌入到设备

**1.创建文件夹在D盘或者其他盘的目录下，我这里创建了如下文件夹**

<div align=center>
<img src="./images/step21.jpg" alt="step21" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step22.jpg" alt="step22" style="zoom:100%;" />
</div>

**2.在Simulink中创建一个新的仿真图，PI等种种模块，都需要改成离散的！以下图的简单算法作为例子。**

<div align=center>
<img src="./images/step23.jpg" alt="step23" style="zoom:100%;" />
</div>
**3.打开设置，如下这两处需要更改，step size可以尝试一下设置，还没试出什么影响。**

<div align=center>
<img src="./images/step24.jpg" alt="step24" style="zoom:100%;" />
</div>
**3.打开code generation，更改system target file为ert.tlc，此为embedded coder，语言设置成想要的（一般都是C吧）。**

<div align=center>
<img src="./images/step25.jpg" alt="step25" style="zoom:100%;" />
</div>

**4.打开code Generation 的Report选项，把如下两个选项勾选，到时生成代码会弹出弹窗给你方便看一点。**

<div align=center>
<img src="./images/step26.jpg" alt="step26" style="zoom:100%;" />
</div>

**5.现在就已经可以点击生成代码，如下图所示。**

<div align=center>
<img src="./images/step27.jpg" alt="step27" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step28.jpg" alt="step28" style="zoom:100%;" />
</div>

**6.但是现在的代码还很乱，可以选择性进行如下操作，首先将算法部分封装起来。**

<div align=center>
<img src="./images/step29.jpg" alt="step29" style="zoom:100%;" />
</div>
**7.然后，再双击数据线命名，不然到时候全是结构体，很难受。**

<div align=center>
<img src="./images/step210.jpg" alt="step210" style="zoom:100%;" />
</div>

**8.而后进入Model Explorer，点击Add，然后点击Add Custom。**

<div align=center>
<img src="./images/step211.jpg" alt="step211" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step212.jpg" alt="step212" style="zoom:100%;" />
</div>

**9.添加备注的信号线等，设置为mpt.signal。**

<div align=center>
<img src="./images/step213.jpg" alt="step213" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step214.jpg" alt="step214" style="zoom:100%;" />
</div>

**10.添加x,y,z信号对象后，须要关联信号名，关联信号名，在信号线上右键，点击Signal Properties…，点击Signal Properties…后，按下图勾选Signal name must resolve to Simulink signal object ，让信号名解析到信号对象上去 ，x,y,z都按以下设置，保存。**

<div align=center>
<img src="./images/step215.jpg" alt="step215" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step216.jpg" alt="step216" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step217.jpg" alt="step217" style="zoom:100%;" />
</div>

**11.把模块单独出来做为一个子系统，能够把算法单独放在一个文件中当做一个函数使用，选中你的封装好的算法，在菜单栏点击Diagram->Block Parameter (Subsystem )。**

<div align=center>
<img src="./images/step218.jpg" alt="step218" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step219.jpg" alt="step219" style="zoom:100%;" />
</div>

**12.按ctrl+B或者点击代码生成，最终代码预览如下。**

<div align=center>
<img src="./images/step220.jpg" alt="step220" style="zoom:100%;" />
</div>

**13.最后生成的代码自动创建如下子目录。**

<div align=center>
<img src="./images/step221.jpg" alt="step221" style="zoom:100%;" />
</div>

**14.上机测试部分。把需要的.c和.h文件全复制到你的工程文件下。**

<div align=center>
<img src="./images/step222.jpg" alt="step222" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step223.jpg" alt="step223" style="zoom:100%;" />
</div>
**15.测试**

<div align=center>
<img src="./images/step224.jpg" alt="step224" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step225.jpg" alt="step225" style="zoom:100%;" />
</div>

<div align=center>
<img src="./images/step226.jpg" alt="step226" style="zoom:100%;" />
</div>
