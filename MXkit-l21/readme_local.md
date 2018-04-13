# How to use MXkit-L21 sdk
  
-------------------------------------
目录

1. [简介](#简介)
2. [下载 IDE](#下载ide)
3. [安装 IDE](#安装ide)
4. [获取 SDK](#获取sdk)
4. [导入工程](#导入工程)
5. [编译工程](#编译工程)
6. [下载与调试](#下载与调试)
7. [IoT示例使用流程](#iot示例使用流程)
    1. [准备工作](#准备工作)
    2. [启动配网](#启动配网)
    3. [控制设备](#控制设备)


## 简介
本 sdk 提供一个基于 Microchip 开发板，通过与 Wi-Fi 模块进行 AT 指令串口通信，实现与阿里云 SDS 直连的 IoT 物联网典型开发应用示例源代码。

下面来讲解如何在您的 PC 上安装 Atmel Studio 开发环境，并进行编译，下载。 



## 下载IDE
点击 [ Atmel Studio 下载](http://www.microchip.com/avr-support/atmel-studio-7)，进入开发工具下载列表页面。在列表中，选择适用于您 PC 系统类型的安装程序版本。

Tips:您也可以选择 在线安装 方式，但一定记得不要让电脑断网哦。


![下载 as](/image/download_as.png)



## 安装IDE
下载完成后，解压缩，双击 “.exe” 安装文件，即可启动安装，一路默认，next到底即可。 

到安装目录中，双击 Atmel Studio.exe 文件，打开开发环境。 您还需更新 SAML21_DFP 组件至：“Atmel SAML21 Series Device Support (1.2.120) ” 版本，下载地址： 

[下载地址 1](http://packs.download.atmel.com/)   或  [下载地址 2](https://microchip.box.com/s/q9lbrcwdt7fhomqkzozjg1vry07xvdzg)，或 [下载地址3](http://developer.mxchip.com/fileDownload/583)

下载后，双击默认安装即可。



## 获取SDK
点击 [MXKit-L21下载](https://github.com/neooxu/MXkit-L21)，进入工程源代码 github 仓库。 您可以：

1. 如已安装 Git 软件，使用 git 命令克隆到 PC 本地：`git clone https://github.com/neooxu/MXkit-L21.git` 
2. 如未安装 Git 软件，可直接下载压缩包文件 PC 本地，点击仓库文件列表右上方， “Download ZIP",下载到本地后解压缩即可。



## 导入工程

菜单栏  “File”--->"Open"--->"Project/Solutiion", 选择 SDK 中的 “MXkit-L21.atsln” 文件，稍等片刻，即可成功导入工程。

![导入工程](/image/import_project.png) 



## 编译工程
点击工具栏中编译图标，开始工程编译过程，全程大约耗时几十秒，请耐心等待。

![开始编译](/image/compiler.png) 

查看编译结果，如图：表示编译成功，可以下载。

![编译完成](/image/compiler_ok.png) 



## 下载与调试

### 连接仿真器与开发板

编译完成后，请将 Jlink 仿真器与开发板连接，确认红色电源灯常亮，保证供电正常。 

![连接仿真器](/image/device_tools.png) 


### 确认连接成功

请选择  "Tools" ---> "Device Programing", 

![进入jlink](/image/into_jlink.png) 


打开后，请依次确认已自动识别到 J-Link 工具，同时出现设备类型：ATSAML21J18B， 接口类型：SWD。 以上显示正常，请点击 “Apply”， 然后点击 “Read”,看 Device Signature 是否有值，Target Voltage 中出现电压值。 如未正常显示，请重新插拔 Jlink 尝试， 还失败，请重新启动 Atmel Studio IDE 开发环境重试。

![确认电压](/image/jlink_assure.png) 


### 开始下载

此时可点击工具栏下载图标，开始下载程序到开发板。

![开始下载](/image/download.png) 

下载过程大约耗时 几秒钟，如下图：

![下载过程](/image/download_process.png) 



### 调试界面

下载完成后，IDE 会自动跳转至 “在线调试” 状态，如下图所示：

![调试过程](/image/debug_status.png) 

此时，开发者可以进行 单步调试 等操作。

若要退出调试状态，请点击工具栏 红色正方形 图标，如下图。

![退出调试](/image/quit_debug.png) 



## IoT示例使用流程

本示例工程提供一个典型的 IoT 应用示例。 系统结构组成如下图：

![IOT典型应用系统组成](/image/demo_frame.png)

Microchip 的 SAML2X 单片机， 通过串口连接 EMW3080 Wi-Fi 模块，并与之进行 AT 指令通信，实现与阿里云 SDS 的通信，同时手机 APP 端可监控开发板的外部设备。

使用具体步骤如下：


### 准备工作

**—— 手机 APP 端**

1.下载安装APP: 点击 [示例 APP 下载页面](https://www.pgyer.com/TIAnd)，或扫描下方二维码，请根据手机系统类型选择下载。

![app下载](/image/app_download_address.png)  

2.使用手机号码，进行 APP 用户账号注册，验证，登录。

![app注册登录](/image/app_userid.png)

3.请确保手机已经成功连接至现场路由器。




**—— 开发板设备端**

1. 请使用 microUSB 连接线为开发板供电，并确认红色电源灯常亮，保证供电正常。

![设备供电](https://github.com/neooxu/MXkit-L21/blob/master/MXkit-l21/image/device_power.png) 


### 启动配网
打开手机 APP, 点击右上角 “+” 号，选择设备 “TI开发板”，根据提示进行操作。

1. 长按开发板上盖板的 USER 按键，恢复设备出厂设置，此时开发板 OLED 屏最后一行将打印显示："Restore default",继而跳变至 “Wi-Fi config....” 等待手机配网中；
2. APP 配网输入界面中，设置正确的路由器用户名和密码，启动配网，此后开发板 OLED 屏最后一行将显示 设备的联网状态，
    1.  “Wi-Fi connected” 代表成功获取路由器的 SSID 和 密码
    2.  “Cloud connected” 表示与云服务器连接成功，此时需进行身份认证；
3. 当 APP 提示身份验证时，请短按上盖板的 USER 按键，完成身份认证；
4. 以上步骤完成后，APP 会自动跳转设备列表页面，显示在线设备；
5. 点击设备图标，进入设备控制页面。

![恢复出厂](https://github.com/neooxu/MXkit-L21/blob/master/MXkit-l21/image/reset_device.png)——> ![启动配网](https://github.com/neooxu/MXkit-L21/blob/master/MXkit-l21/image/net_config.png)

![身份验证](https://github.com/neooxu/MXkit-L21/blob/master/MXkit-l21/image/id_ensure.png)  ——> ![设备列表](https://github.com/neooxu/MXkit-L21/blob/master/MXkit-l21/image/device_list.png)



### 控制设备
进入设备控制页面，可对各外部设备参数进行监测或控制，或在线调试。

1. 监测：
   1. 温湿度值：设备向云端上报数据
   2. 开关状态：可通过拨动开发板上的拨码开关体验。 开关1：S1， 开关2： S2。 高电平为关，低电平为开。
2. 控制：
   1. RGB灯颜色，饱和度，亮度（通过滑动色彩环 或 滑动条来调整）。
3. 在线调试：
   1. APP 调试界面向设备发送字符串，会在 OLED 屏最后一行打印出来（注意屏幕打印不支持中文）。
   2. APP 发送数据会同步显示在接收区，带来 echo 回声体验效果。


![设备监控](https://github.com/neooxu/MXkit-L21/blob/master/MXkit-l21/image/device_control.png) ——>  ![在线调试](https://github.com/neooxu/MXkit-L21/blob/master/MXkit-l21/MXkit-L21/image/app_debug.png)


## 结束语

以上为本 SDK 在 CCS 开发环境中的具体使用方法，请仔细参考，祝学习愉快，谢谢。
  





