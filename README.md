# CombineSensor_STM32

现在主要有4个模块

1.ADC
2.TIMER
3.USART
4.CAN

ADC使用的是ADC1的channel 11，采集的是C1和GND之间的电压

Timer使用了一个基本定时器和一个通用定时器，基本定时器用来定时1ms，通用定时器用来发出pwm方波控制舵机，pwm方波的周期为20ms，也就是说频率为50HZ,占空比的大小对应了舵机的角度，占空比为2.5%时，对应0度，7.5%对应90，12.5%对应180度

USART使用的是串口1，波特率为115200
CAN总线设置成波特率为250K，和协议规定的一致
