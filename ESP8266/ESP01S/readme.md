### ESP01s Relay bug修复
如果硬件使用了ESP01s Relay继电器模块，早期版本（<1.2)需要手动修复bug  
- 板子反面短接5(vcc) 和 7(ch-pd，也叫en)，使pin7高电平
- 板子正面剪除R2(SMD电阻标称103)，使pin3(gpio 0)置空

### esp01s pinout
![ESP01 引脚图](pinout_esp01.jpg)
