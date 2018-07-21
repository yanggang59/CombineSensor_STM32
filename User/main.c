#include "stm32f10x.h"
#include "bsp_GeneralTim.h"  
#include "bsp_BaseTim.h"
#include "bsp_can.h"
#include "bsp_adc.h"


volatile uint32_t servo_delay_time = 0; // ms ��ʱ���� 

volatile uint32_t can_delay_time = 0; // ms ��ʱ���� 

TIM_OCInitTypeDef  TIM_OCInitStructure;

CanTxMsg TxMessage;			     //CAN���ͻ�����

int direct=0; //�����ת����  0��500-2500   0-180��    1��2500-500   180-0�� 

extern __IO uint16_t ADC_ConvertedValue; // ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
	 
float ADC_ConvertedValueLocal;     // �ֲ����������ڱ���ת�������ĵ�ѹֵ    



/**
*�����ת����
*/
void ServoMove(void)
{
	if ( servo_delay_time == 15 ) /* 15 * 1 ms = 1s ʱ�䵽 */
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
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	/*ADC ��ʼ��*/
	ADCx_Init();
	
	/*������ʱ����ʼ��*/
	BASIC_TIM_Init();
	
	/* ͨ�ö�ʱ����ʼ�� */
	GENERAL_TIM_Init();
		
	  /*��ʼ��USART1*/
  Debug_USART_Config();
	
		/*��ʼ��can*/
	CAN_Config();
	
	
	
	printf("\r\n CAN ʹ�ûػ�ģʽ���������ⲿ����\r\n");
	
	
  while(1)
  {  
			
		
			ServoMove();
		
		
			if(can_delay_time==15)
			{
				can_delay_time = 0;
				
				CAN_SetMsg(&TxMessage);	
				
				CAN_Transmit(CANx, &TxMessage);		
				
				printf("\r\n��ʹ��CAN�������ݰ���\r\n"); 
				
				printf("\r\n���͵ı�������Ϊ��\r\n");
				
				printf("\r\n ��չID��ExtId��0x%x \r\n",TxMessage.ExtId);	
				
				CAN_DEBUG_ARRAY(TxMessage.Data,8); 		



			  ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // ��ȡת����ADֵ
			
			  printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
			
		    printf("\r\n The 1current AD value = %f V \r\n",ADC_ConvertedValueLocal); 
				
			}
			

		
		  
  }
}
/*********************************************END OF FILE**********************/
