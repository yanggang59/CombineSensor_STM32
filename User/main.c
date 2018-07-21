#include "stm32f10x.h"
#include "bsp_GeneralTim.h"  
#include "bsp_BaseTim.h"
#include "bsp_can.h"
#include "bsp_adc.h"


volatile uint32_t servo_delay_time = 0; // ms 计时变量 

volatile uint32_t can_delay_time = 0; // ms 计时变量 

TIM_OCInitTypeDef  TIM_OCInitStructure;

CanTxMsg TxMessage;			     //CAN发送缓冲区

int direct=0; //舵机旋转方向  0：500-2500   0-180度    1：2500-500   180-0度 

extern __IO uint16_t ADC_ConvertedValue; // ADC1转换的电压值通过MDA方式传到SRAM
	 
float ADC_ConvertedValueLocal;     // 局部变量，用于保存转换计算后的电压值    



/**
*舵机旋转函数
*/
void ServoMove(void)
{
	if ( servo_delay_time == 15 ) /* 15 * 1 ms = 1s 时间到 */
    {
        servo_delay_time = 0;	
			
				if(direct == 0)
				{					
					TIM_OCInitStructure.TIM_Pulse+=10;
					
					TIM_Cmd(GENERAL_TIM, DISABLE);
					
					TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
	
					TIM_Cmd(GENERAL_TIM, ENABLE);
					
					if(TIM_OCInitStructure.TIM_Pulse==2500)   direct=1;														
				}	
			else
				{
					TIM_OCInitStructure.TIM_Pulse-=10;
					
					TIM_Cmd(GENERAL_TIM, DISABLE);
					
					TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
	
					TIM_Cmd(GENERAL_TIM, ENABLE);
					
					if(TIM_OCInitStructure.TIM_Pulse==500)    direct=0;							
			  }						
    }        
}	

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	/*ADC 初始化*/
	ADCx_Init();
	
	/*基本定时器初始化*/
	BASIC_TIM_Init();
	
	/* 通用定时器初始化 */
	GENERAL_TIM_Init();
		
	  /*初始化USART1*/
  Debug_USART_Config();
	
		/*初始化can*/
	CAN_Config();
	
	
	
	printf("\r\n CAN 使用回环模式，不接收外部数据\r\n");
	
	
  while(1)
  {  
			
		
			ServoMove();
		
		
			if(can_delay_time==15)
			{
				can_delay_time = 0;
				
				CAN_SetMsg(&TxMessage);	
				
				CAN_Transmit(CANx, &TxMessage);		
				
				printf("\r\n已使用CAN发送数据包！\r\n"); 
				
				printf("\r\n发送的报文内容为：\r\n");
				
				printf("\r\n 扩展ID号ExtId：0x%x \r\n",TxMessage.ExtId);	
				
				CAN_DEBUG_ARRAY(TxMessage.Data,8); 		



			  ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // 读取转换的AD值
			
			  printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
			
		    printf("\r\n The 1current AD value = %f V \r\n",ADC_ConvertedValueLocal); 
				
			}
			

		
		  
  }
}
/*********************************************END OF FILE**********************/
