#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "LED.h"

uint8_t blueToothReceiveFlag;
uint8_t blueToothReceiveData;

void blueToothInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
	
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef USART1_InitStructure;
	USART1_InitStructure.USART_BaudRate = 9600;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStructure.USART_Mode = (USART_Mode_Rx | USART_Mode_Tx); 
	USART1_InitStructure.USART_Parity = USART_Parity_No;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART1_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

uint8_t getBlueToothReceiveFlag()
{
	if(1 == blueToothReceiveFlag)
	{
		blueToothReceiveFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t getBlueToothReceiveData()
{
	return blueToothReceiveData;
}

void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		turn_LED2();
		blueToothReceiveData = USART_ReceiveData(USART1);
		blueToothReceiveFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
