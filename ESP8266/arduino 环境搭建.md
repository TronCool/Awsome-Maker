两种方案，在公司可用方案1，在家用方案2

方案1.打开Arduino IDE 菜单>文件>首选项，添加以下网址到 附加开发板管理器网址
http://arduino.esp8266.com/stable/package_esp8266com_index.json
打开Arduino IDE 菜单>工具>开发板>开发板管理器，搜索“8266”，并安装
在Arduino IDE 菜单>工具>开发板>开发板管理器 中选择你正在使用的8266开发板，即可开始开发啦



方案2.下载我们打包的esp8266安装包，直接运行并解压即可。
下载地址1：
https://pan.baidu.com/s/19yEQ4UDF-bQCzt3BsdV6Yw

下载地址2：
https://share.weiyun.com/5CGEKJO


备注：在Mac编译程序可能会出现python3找不到的情况，可以对python3做个软连接
sudo ln -s /usr/bin/python3 ～/Library/Arduino15/packages/esp8266/tools/python3/3.7.2-post1/python3
