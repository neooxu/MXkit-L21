# How to use MXkit-L21 sdk
  
-----------------------------------------
目录

* [1. 简介](#1-简介)
* [2. 使用Atmel Studio开发](#2-使用atmel-studio开发) 
   * [2.1 下载 IDE](#21-下载ide)
   * [2.2 安装 IDE](#22-安装ide)
   * [2.3 获取 SDK](#23-获取sdk)
   * [2.4 导入工程](#24-导入工程)
   * [2.5 编译工程](#25-编译工程)
   * [2.6 下载与调试](#26-下载与调试)
* [3. 使用AliOS Studio开发](#3-使用alios-studio开发)
   * [3.1 Windlow环境配置](#31-windows环境配置)
   * [3.2 获取 AliOS Things SDK](#32-获取alios-things-sdk)
   * [3.3 SDK编译](#33-sdk编译)
   * [3.4 固件下载](#34-固件下载) 
* [4. IoT示例使用流程](#4-iot-示例使用流程)
   * [4.1 准备工作](#41-准备工作)
   * [4.2 启动配网](#42-启动配网)
   * [4.3 控制设备](#43-控制设备)


## 1. 简介
本 sdk 提供一个基于 Microchip 开发板，通过与 Wi-Fi 模块进行 AT 指令串口通信，实现与阿里云 SDS 直连的 IoT 物联网典型开发应用示例源代码。

下面分别讲解如何使用 Atmel Studio IDE 或者 AliOS Studio IDE，进行开发板相应的 SDK 编译与下载。 

## 2. 使用Atmel Studio开发

### 2.1 下载IDE
点击 [ Atmel Studio 下载](http://www.microchip.com/avr-support/atmel-studio-7)，进入开发工具下载列表页面。在列表中，选择适用于您 PC 系统类型的安装程序版本。

Tips:您也可以选择在线安装方式，但一定记得不要让电脑断网。


![下载 as](https://github.com/neooxu/MXkit-L21/blob/master/image/download_as.png)



### 2.2 安装IDE
下载完成后，解压缩，双击 “.exe” 安装文件，即可启动安装，一路默认，next到底即可。 

到安装目录中，双击 Atmel Studio.exe 文件，打开开发环境。 您还需更新 SAML21_DFP 组件至：“Atmel SAML21 Series Device Support (1.2.120) ” 版本，下载地址： 

[下载地址 1](http://packs.download.atmel.com/)   或  [下载地址 2](https://microchip.box.com/s/q9lbrcwdt7fhomqkzozjg1vry07xvdzg)，或 [下载地址3](http://developer.mxchip.com/fileDownload/583)

下载后，双击默认安装即可。



### 2.3 获取SDK
点击 [MXKit-L21下载](https://github.com/neooxu/MXkit-L21)，进入工程源代码 github 仓库。 您可以：

1. 如已安装 Git 软件，使用 git 命令克隆到 PC 本地：`git clone https://github.com/neooxu/MXkit-L21.git` 
2. 如未安装 Git 软件，可直接下载压缩包文件 PC 本地，点击仓库文件列表右上方， “Download ZIP",下载到本地后解压缩即可。



### 2.4 导入工程

菜单栏  “File”--->"Open"--->"Project/Solutiion", 选择 SDK 中的 “MXkit-L21.atsln” 文件，稍等片刻，即可成功导入工程。

![导入工程](https://github.com/neooxu/MXkit-L21/blob/master/image/import_project.png) 



### 2.5 编译工程
点击工具栏中编译图标，开始工程编译过程，全程大约耗时几十秒，请耐心等待。

![开始编译](https://github.com/neooxu/MXkit-L21/blob/master/image/compiler.png) 

查看编译结果，如图：表示编译成功，可以下载。

![编译完成](https://github.com/neooxu/MXkit-L21/blob/master/image/compiler_ok.png) 



### 2.6 下载与调试

#### 2.6.1 连接仿真器与开发板

编译完成后，请将 Jlink 仿真器与开发板连接，确认红色电源灯常亮，保证供电正常。 

![连接仿真器](https://github.com/neooxu/MXkit-L21/blob/master/image/device_tools.png) 


#### 2.6.2 确认连接成功

请选择  "Tools" ---> "Device Programing", 

![进入jlink](https://github.com/neooxu/MXkit-L21/blob/master/image/into_jlink.png) 


打开后，请依次确认已自动识别到 J-Link 工具，同时出现设备类型：ATSAML21J18B， 接口类型：SWD。 以上显示正常，请点击 “Apply”， 然后点击 “Read”,看 Device Signature 是否有值，Target Voltage 中出现电压值。 如未正常显示，请重新插拔 Jlink 尝试， 还失败，请重新启动 Atmel Studio IDE 开发环境重试。

![确认电压](https://github.com/neooxu/MXkit-L21/blob/master/image/jlink_assure.png) 


#### 2.6.3 开始下载

此时可点击工具栏下载图标，开始下载程序到开发板。

![开始下载](https://github.com/neooxu/MXkit-L21/blob/master/image/download.png) 

下载过程大约耗时 几秒钟，如下图：

![下载过程](https://github.com/neooxu/MXkit-L21/blob/master/image/download_process.png) 



#### 2.6.4 调试界面

下载完成后，IDE 会自动跳转至 “在线调试” 状态，如下图所示：

![调试过程](https://github.com/neooxu/MXkit-L21/blob/master/image/debug_status.png) 

此时，开发者可以进行 单步调试 等操作。

若要退出调试状态，请点击工具栏 红色正方形 图标，如下图。

![退出调试](https://github.com/neooxu/MXkit-L21/blob/master/image/quit_debug.png) 

## 3. 使用AliOS-Studio开发

### 3.1 Windows环境配置 

此处请参考：[AliOS Windows环境配置方法](https://github.com/alibaba/AliOS-Things/wiki/AliOS-Things-Windows-Environment-Setup)

注意：请忽略该页面中 关于 `aos make helloworld@mk3060` 编译的命令内容。

### 3.2 获取AliOS Things SDK

请进入 [AliOS-Things SDK 仓库页面](https://github.com/neooxu/AliOS-Things/tree/saml21)，您可以：

* 如果您安装了 Git, 可通过命令： `git clone https://github.com/neooxu/AliOS-Things.git` 克隆SDK仓库到本地，并切换分支： `git checkout saml21`。

* 如果您未安装 Git，在确认该仓库切换分支到 “saml21”后，通过点击 “Downlod zip”,下载 SDK 源代码到本地。

下载后，进入 SDK 目录，可找到本 IOT 示例对应的 源代码文件，如下图：

![microchip](https://github.com/neooxu/MXkit-L21/blob/master/image/microchipdemo.png)


### 3.3 SDK编译

PC端通过快捷键 “win+R”, 输入 “cmd”， 打开 “系统命令行”窗口 ，进入 AliOS-Things SDK 目录下，输入编译命令：

`aos make microchip.IOT_AT3080_demo@saml21_iot_sk`，

并回车。

编译成功，log信息如下图，说明您的环境配置成功。

![AliOS-Things SDK编译](https://github.com/neooxu/MXkit-L21/blob/master/image/aosmake.png)


### 3.4 固件下载

#### 3.4.1 下载 OpenOCD

如果想通过 Jlink 下载编译固件到 SMAL2x IOT SK 开发板，还需要下载 OpenOCD 工具文件。
请点击进入：[OpenOCD 官方网站](http://www.freddiechopin.info/en/download/category/4-openocd)，下载最新版本的 OpenOCD 压缩包，并解压。

**注意：MacOS 的 OpenOCD 压缩包文件下载链接：[gnu-mcu-eclipse-openocd-0.10.0-8-20180512-1921-macos.tgz](https://github.com/gnu-mcu-eclipse/openocd/releases/download/v0.10.0-8-20180512/gnu-mcu-eclipse-openocd-macos.tgz)，并解压。**

#### 3.4.2 配置 OpenOCD

1. Windows系统中，请将解压后的 OpenOCD 文件夹放在 “\AliOS-Things\build” 根目录下，并修改文件夹名字为：“OpenOCD”;
2. 修改该目录：“\AliOS-Things\build\OpenOCD” 中的文件夹 “bin” 的名字为： “Win32”。

**注意：MacOS系统中，直接复制解压后的文件：“openocd” 到 “\AliOS-Things\build” 根目录下。**


#### 3.4.3 固件下载

1. 首先确保开发板连接至 Jlink（注意：若jlink无法提供外接电源，必须通过usb线为开发板供电）。
2. 打开步骤 “3.3 SDK编译” 中的PC端命令行窗口，在正确的目录下，输入下载命令： 

 `aos make microchip.IOT_AT3080_demo@saml21_iot_sk download`

并回车，即可下载固件到开发板，log信息如下图：

![AliOS-Things SDK下载](https://github.com/neooxu/MXkit-L21/blob/master/image/aosdownload.png)

至此，固件下载完毕，可继续步骤 4， 开始学习示例使用。


## 4. IoT示例使用流程

本示例工程提供一个典型的 IoT 应用示例。 系统结构组成如下图：

![IOT典型应用系统组成](https://github.com/neooxu/MXkit-L21/blob/master/image/demo_frame.png)

Microchip 的 SAML2X 单片机， 通过串口连接 EMW3080 Wi-Fi 模块，并与之进行 AT 指令通信，实现与阿里云 SDS 的通信，同时手机 APP 端可监控开发板的外部设备。

使用具体步骤如下：


### 4.1 准备工作

**—— 手机 APP 端**

1.下载安装APP: 点击 [示例 APP 下载页面](http://www.pgyer.com/MICROa)，或扫描下方二维码，请根据手机系统类型选择下载。

![app下载](https://github.com/neooxu/MXkit-L21/blob/master/image/microchip_app.png)  

2.使用手机号码，进行 APP 用户账号注册，验证，登录。

![app注册登录](https://github.com/neooxu/MXkit-L21/blob/master/image/app_userid.png)

3.请确保手机已经成功连接至现场路由器。




**—— 开发板设备端**

1. 请使用 microUSB 连接线为开发板供电，并确认红色电源灯常亮，保证供电正常。

![设备供电](https://github.com/neooxu/MXkit-L21/blob/master/image/device_power.png) 


### 4.2 启动配网
打开手机 APP, 点击右上角 “+” 号，选择设备 “SAML21 IOT SK”，根据提示进行操作。

1. 长按开发板上盖板的 USER 按键，恢复设备出厂设置，此时开发板 OLED 屏最后一行将打印显示："Restore default",继而跳变至 “Wi-Fi config....” 等待手机配网中；
2. APP 配网输入界面中，设置正确的路由器用户名和密码，启动配网，此后开发板 OLED 屏最后一行将显示 设备的联网状态，
    1.  “Wi-Fi connected” 代表成功获取路由器的 SSID 和 密码
    2.  “Cloud connected” 表示与云服务器连接成功，此时需进行身份认证；
3. 当 APP 提示身份验证时，请短按上盖板的 USER 按键，完成身份认证；
4. 以上步骤完成后，APP 会自动跳转设备列表页面，显示在线设备；
5. 点击设备图标，进入设备控制页面。

![恢复出厂](https://github.com/neooxu/MXkit-L21/blob/master/image/reset_device.png)——> ![启动配网](https://github.com/neooxu/MXkit-L21/blob/master/image/net_config.png)

![身份验证](https://github.com/neooxu/MXkit-L21/blob/master/image/id_ensure.png)  ——> ![设备列表](https://github.com/neooxu/MXkit-L21/blob/master/image/device_list.png)



### 4.3 控制设备
进入设备控制页面，可对各外部设备参数进行监测或控制，或在线调试。

1. 监测：
   1. 温湿度值：设备向云端上报数据
   2. 开关状态：可通过拨动开发板上的拨码开关体验。 开关1：S1， 开关2： S2。 高电平为关，低电平为开。
2. 控制：
   1. RGB灯颜色，饱和度，亮度（通过滑动色彩环 或 滑动条来调整）。
3. 在线调试：
   1. APP 调试界面向设备发送字符串，会在 OLED 屏最后一行打印出来（注意屏幕打印不支持中文）。
   2. APP 发送数据会同步显示在接收区，带来 echo 回声体验效果。


![设备监控](https://github.com/neooxu/MXkit-L21/blob/master/image/device_control.png) ——>  ![在线调试](https://github.com/neooxu/MXkit-L21/blob/master/image/app_debug.png)


## 结束语

以上为本 SDK 在 Atmel Studio IDE 和 AliOS Studio 中的使用方法，请仔细参考，祝学习愉快，谢谢。
  





