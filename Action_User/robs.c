#include "ROBS.h"
#include "usart.h"
#include "stm32f4xx_tim.h"
#include "math.h"
#include "walk.h"
#include "GET_SET.h"

//�򿪶����Ť�����
void Enable_ROBS()
{
	USART_OUT(USART1,"#1 W 40,1,1\r\n");
	 //#1 W 40,1,1\r\n
}

//ʹ���ŷ�ģʽ
void Enable_ServoMode()
{
	USART_OUT(USART1,"#1 W 20,1,0\r\n");
	 //#1 W 20,1,0\r\n
}

//ʹ�ܵ��ģʽ
void Enable_MotorMode()
{
	USART_OUT(USART1,"#1 W 20,1,1\r\n");
   //#1 W 20,1,1\r\n
}



//�ŷ�ģʽ�º���

 //��vel���ٶ�ת��angle�Ƕ�
void ROBS_PosCrl(float angle,int vel)
{
	float pos;

	//Enable_ROBS();
  pos= (2048-angle/360*4096/30*52);
	USART_OUT(USART1,"#1 W 42,2,%d:46,2,%d\r\n",(int)pos,vel);
	//#1 W 42,2,pos:46,2,vel\r\nca
}
                           


//���ģʽ�º���


//��vel���ٶ�תtime ms,time=0����һֱת
void ROBS_VelCrl(int vel,int time)
{
	USART_OUT(USART1,"#1 W 46,2,%d:44,2,%d\r\n",vel,time);
	//#1 W 46,2,vel:44,2,time\r\n
}

void TurnLeft(int vel)
{
	  USART_OUT(USART1,"#1 W 46,2,%d:44,2,0\r\n",-vel);
}

void TurnRight(int vel)
{
	USART_OUT(USART1,"#1 W 46,2,%d:44,2,0\r\n",vel);

}

void Stop(void)
{
	USART_OUT(USART1,"#1 W 46,2,0:44,2,0\r\n");
} 

void ReadROBSAngle(void)
{
	USART_OUT(USART1,"#1 R 56,2,1\r\n");
   //#1 W 20,1,1\r\n
}




