#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "SteeringEngine.h"
#include "BlueTooth.h"
#include "LED.h"

/*****中断函数使用表********/
//void USART1_IRQHandler()//Using

int main()
{
	uint8_t ReceiveData = 0;
	float angle = 90;
	OLED_Init();
	blueToothInit();
	steeringEngineInit();
	LED_Init();
	OLED_ShowString(1, 1, "Welcome to use:");
	OLED_ShowString(2, 1, "Steering Engine");
	OLED_ShowString(3, 1, "Data:");
	OLED_ShowHexNum(3, 6, ReceiveData, 2);
	OLED_ShowString(4, 1, "Angle:");
	setSteeringEngineAngle(angle);
	OLED_ShowNum(4, 7, angle, 3);

	while(1)
	{
		if(1 == getBlueToothReceiveFlag())
		{
			ReceiveData = getBlueToothReceiveData();
			OLED_ShowHexNum(3, 6, ReceiveData, 2);
		}
		
		if(0xFD == ReceiveData)
		{
			angle += 5;
			if(angle > 180)
				angle = 0;
			ReceiveData = 0xFF;
		}		
		else if(0xFC == ReceiveData)
		{
			angle -= 5;
			if(angle < 0)
				angle = 180;
			ReceiveData = 0xFF;
		}
		else if(0x11 == ReceiveData)
		{
			angle += 10;
			if(angle > 180)
				angle = 0;
			ReceiveData = 0xFF;
		}
		else if(0x12 == ReceiveData)
		{
			angle -= 10;
			if(angle < 0)
				angle = 180;
			ReceiveData = 0xFF;
		}
		else if(0x08 == ReceiveData)
		{
			angle = 70;
			ReceiveData = 0xFF;			
		}
		else if(0x09 == ReceiveData)
		{
			angle = 110;
			ReceiveData = 0xFF;			
		}
		setSteeringEngineAngle(angle);
		OLED_ShowNum(4, 7, angle, 3);
	}
}

