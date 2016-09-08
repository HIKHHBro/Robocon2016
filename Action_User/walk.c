/**
  ******************************************************************************
  * @file      
  * @author  2016Robcon-NEU Action�Ŷ�
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  *
  * 
  ******************************************************************************
  */ 
/* Includes -------------------------------------------------------------------*/
#include <math.h>
#include "walk.h"
#include "GET_SET.h"
#include "stdint.h"
#include "action_math.h"
#include "math.h"
#include "elmo.h"
#include "timer.h"
#include "robs.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "fuzzypid.h"
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
/* Extern   variables ---------------------------------------------------------*/
extern float debug_Err[3];
extern int   debug_tagVel[6];
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/


/*************************************************************************/
/*----------------------------Walking Part-------------------------------*/
/*************************************************************************/

void LockWheel(void)
{
	VelCrl(1,0);
	VelCrl(2,0);
	VelCrl(3,0);	
}
/**
  * @brief  ����ֱ�߼�����ת
  * @param  Vel:���ĺ��ٶȣ����� Vel>0��
  * @param  ward:�����н�����
                   -180��+180
  * @param  Rotate:���������ת�ٶȣ�����ʱ���� ����ͼ��
  * @param  selfAngle:���������̬�Ƕ�
  * @retval none
  * @author lxy
  */
void BasicLine(int Vel,float ward,float Rotate)
{ 
	static  int tarV[3];
	static  int last_tarV[3]={0};
	        int V_sum;	
					int RotateVal;
					int maxV=0;
	        int i=0;
	        float reduceP;
	
	debug_tagVel[0]=last_tarV[0];
	debug_tagVel[1]=last_tarV[1];
	debug_tagVel[2]=last_tarV[2];
	
	/*������ת������*/
	RotateVal=VelTransform(Rotate);
  /*����ǰ���ܳ��ٵ�������*/
	V_sum=VelTransform(Vel);
	
/*�����ٶȼ���*/
	#ifdef    BLUE_FIELD
	tarV[2]= (V_sum*Cos(ward-Get_Angle()+ 60))+RotateVal;
	tarV[1]=-(V_sum*Cos(ward-Get_Angle()+  0))+RotateVal;
	tarV[0]=-(V_sum*Cos(ward-Get_Angle()+120))+RotateVal;
	#elif	    defined	RED_FIELD
	tarV[0]=-(V_sum*Cos(ward-Get_Angle()+ 60))-RotateVal;//������ʱ��tarV[0]���Զ��Ӹ����ţ�����0����2�ŶԵ�
	tarV[1]= (V_sum*Cos(ward-Get_Angle()+  0))-RotateVal;
	tarV[2]= (V_sum*Cos(ward-Get_Angle()+120))-RotateVal;
  #endif

	for(i=0;i<3;i++)
	{
		if(fabs(tarV[i])>maxV)
			  maxV=fabs(tarV[i]);
	}
//	USART_OUT(USART2,"maxV=%d\r\n",maxV/10);
	if(maxV>420000)
	{
		BEEP_ON;
		for(i=0;i<3;i++)
		{
			reduceP=(float)tarV[i]/(float)maxV;
			tarV[i]=420000*reduceP;
		}
	}

	/*�����ٶȸ���*/
	VelCrl(1,tarV[0]);
	VelCrl(2,tarV[1]);
	VelCrl(3,tarV[2]);
	
	last_tarV[0]=tarV[0];
	last_tarV[1]=tarV[1];
	last_tarV[2]=tarV[2];
}


void EndCloseLoopLine(void)
{
	 float	            errAngle            =0;
	 int                rotate              =0;
	 float              refAngle            =0;
	 #ifdef    BLUE_FIELD
	 int 								pAngle							=-1000;
	 #elif defined RED_FIELD
	 int                pAngle              =1000;
	 #endif 
   

	 errAngle=refAngle-Get_Angle();
     if(fabs(errAngle)<1)errAngle=0;
	 rotate = errAngle*pAngle;//-sumErrAngle*iAngle;
	 VelCrl(1,rotate);
	 VelCrl(3,rotate);

}
/**
  * @brief  �ջ�����,���귵��ֵ1��û���귵��ֵ0
  * @param  Vel:�������ٶȣ����� Vel>0��
  * @param  WardInit����ʼ�ķ���
  * @param  WardEnd������ķ���
  * @param  Radius�����İ뾶������Ϊ˳ʱ�룬����Ϊ��ʱ�룩
  * @param  IsRotate��IsRotate==1,Ҫ��ת��IsRotate==0������ת

  * @retval none
  * @author tzy
  */
int8_t BasicCircle(int Vel,float WardInit,float WardEnd,float Refangle,float Radius,int8_t IsRotate,FPID_TypeDef *fpid)
{
	float ActRadius,Pos_Ox,Pos_Oy,angle,WardAdd;
	
	Pos_Ox=get_origin_x(Radius,WardInit);//����ԭ��
	Pos_Oy=get_origin_y(Radius,WardInit);
	ActRadius=sqrt(pow((Get_POS_X()-Pos_Ox),2)+pow((Get_POS_Y()-Pos_Oy),2));//ʵ�ʰ뾶
	angle=((90-WardInit)-Acos(get_cos(Radius,WardInit)));//��Բ�ĽǶ� 

	WardAdd=WardInit+angle;	
	
	if(Radius>0)//˳ʱ��
	{
	  if(IsRotate==1)FuzPidLine(Vel,WardAdd,WardAdd,Radius,ActRadius,1,fpid);
    if(IsRotate==0)FuzPidLine(Vel,WardAdd,Refangle,Radius,ActRadius,1,fpid);
	  if(WardAdd<=WardEnd)return CIRCLE_END;
	  if(WardAdd>WardEnd) return 0;		
	}
	else if(Radius<0)//��ʱ��
	{
		if(IsRotate==1) FuzPidLine(Vel,WardAdd,WardAdd,-Radius,ActRadius,-1,fpid);
    if(IsRotate==0)	FuzPidLine(Vel,WardAdd,Refangle,-Radius,ActRadius,-1,fpid); 
		if(WardAdd>=WardEnd)return CIRCLE_END;
		if(WardAdd<WardEnd) return 0;
	}
  return  2;	
}


/**
  * @brief  �ջ�����,���귵��ֵ1��û���귵��ֵ0
  * @param  Vel:�������ٶȣ����� Vel>0��
  * @param  WardInit����ʼ�ķ���
  * @param  WardEnd������ķ���
  * @param  Radius�����İ뾶������Ϊ˳ʱ�룬����Ϊ��ʱ�룩
  * @param  IsRotate��IsRotate==1,Ҫ��ת��IsRotate==0������ת

  * @retval none
  * @author tzy
  */
int8_t HighCircle(int Vel,float WardInit,float WardEnd,float Refangle,float Radius,int8_t IsRotate,FPID_TypeDef *fpid)
{
	float ActRadius,Pos_Ox,Pos_Oy,angle,WardAdd;
	float p_highcircle=0.1;
	
	Pos_Ox=get_origin_x(Radius,WardInit);//����ԭ��
	Pos_Oy=get_origin_y(Radius,WardInit);
	ActRadius=sqrt(pow((Get_POS_X()-Pos_Ox),2)+pow((Get_POS_Y()-Pos_Oy),2));//ʵ�ʰ뾶
	angle=((90-WardInit)-Acos(get_cos(Radius,WardInit)));//��Բ�ĽǶ� 
	
  if(GetLaserValue(BASIC)-REFLASER<70)
  WardAdd=WardInit+angle+(GetLaserValue(BASIC)-REFLASER)*p_highcircle;
	else
	WardAdd=WardInit+angle;	
	
	if(Radius>0)//˳ʱ��
	{
	  if(IsRotate==1)FuzPidLine(Vel,WardAdd,WardAdd,Radius,ActRadius,1,fpid);
    if(IsRotate==0)FuzPidLine(Vel,WardAdd,Refangle,Radius,ActRadius,1,fpid);
	  if(WardAdd<=WardEnd)return CIRCLE_END;
	  if(WardAdd>WardEnd) return 0;		
	}
	if(Radius<0)//��ʱ��
	{
		if(IsRotate==1) FuzPidLine(Vel,WardAdd,WardAdd,-Radius,ActRadius,-1,fpid);
    if(IsRotate==0)	FuzPidLine(Vel,WardAdd,Refangle,-Radius,ActRadius,-1,fpid); 
		if(WardAdd>=WardEnd)return CIRCLE_END;
		if(WardAdd<WardEnd) return 0;
	}		
	return  2;	
}

/*************************************************************************/
/*----------------------------Adjustment Part-------------------------------*/
/*************************************************************************/

/**
  * @brief  �Ѽ���ֵת��ΪX����ֵ
  * @param  LaserValue ����ֵ
  * @retval none
  * @author tzy
  */
int Trans_Laser_to_Xvalue(float LaserValue,float Angle)
{	
	#ifdef  BLUE_FIELD
	return (HIGH_WALL_XVALUE-(Trans_Laser_to_Dis(LaserValue)*Cos(Angle)-DIS_LASER_Y*Sin(Angle)));
	#elif   defined	RED_FIELD
	return (HIGH_WALL_XVALUE-(Trans_Laser_to_Dis(LaserValue)*Cos(Angle)+DIS_LASER_Y*Sin(Angle)));
	#endif
}
/**
  * @brief  ��̨����������
  * @retval none
  * @author tzy
**/

void AdjPosX_A2(int AccurateX)
{
	Set_Offset_X(Get_POS_X()-AccurateX);
}

void AdjPosX(int LaserValue,int8_t flag)
{
   static int accurateX;
   static float offset_x;
   if(flag==0){
   accurateX=Trans_Laser_to_Xvalue(LaserValue,Get_Angle());
   offset_x=Get_POS_X()-accurateX;
	 }
   if(flag==1)Set_Offset_X(offset_x);
}
void AdjPosY(float PosY)
{
	 static float accuratey;
	 #ifdef  BLUE_FIELD
   accuratey=ACCURATEY+DIS_LASER_Y;
   #elif 	 defined  RED_FIELD
   accuratey=ACCURATEY-DIS_LASER_Y;
   #endif
	//if there is a accurate laser value,and then 
	//blue:accuratey= 7175+Trans_Laser_to_Dis(Laser)*sin(angle)+DIS_LASER_X*cos(angle)
	//red: accuratey=7175+Trans_Laser_to_Dis(Laser)*sin(angle)-DIS_LASER_X*cos(angle)
   Set_Offset_Y(PosY-accuratey);
}

int Trans_Dis_to_Laser(float distance)
{
	return (1.071f*(distance-DIS_LASER_X)-209.6f);
}

int Trans_Laser_to_Dis(float Laser)
{
	return  ((Laser+209.6f)/1.071f+DIS_LASER_X);
}







void  SetActualVel(void)
{
		int posX=0,posY=0;
		int vel_X=0,vel_Y=0;
		int i=0;
		static int prePosX[10],prePosY[10];	
	/*����ʵ���ٶ�*/
		 posX=Get_POS_X();
		 posY=Get_POS_Y();
		
		 vel_X=(posX-prePosX[0])*10;
		 vel_Y=(posY-prePosY[0])*10;
		 		 
		 Set_ActVel_X(vel_X);
		 Set_ActVel_Y(vel_Y);	
     /*���´洢������*/
		 for(i=0;i<9;i++)
		 {
			  prePosX[i]=prePosX[i+1];
			  prePosY[i]=prePosY[i+1];
		 }
		 prePosX[9]=posX;
		 prePosY[9]=posY;
}

/************************ (C) COPYRIGHT 2015 ACTION *****END OF FILE****/

