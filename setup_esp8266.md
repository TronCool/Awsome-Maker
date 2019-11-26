### 基础知识
烧 Nodemcu 固件 - 用lua写代码
烧 Micro Python 固件 - 用 python写代码

### 给esp8266配置micro python 
从这里下载固件 https://micropython.org/download#esp8266https://micropython.org/download#esp8266
插上mcu, ls /dev 查看USB串口的端口名字，我的叫 /dev/cu.wchusbserial1450

pip3 install esptool
esptool.py --port /dev/cu.wchusbserial1450 erase_flash
esptool.py --port /dev/cu.wchusbserial1450 --baud 115200 write_flash --flash_size=detect 0 esp8266-20190529-v1.11.bin
esptool.py --port /dev/cu.wchusbserial1450 --baud 115200 write_flash --flash_size=detect -fm dio 0 esp8266-20190529-v1.11.bin