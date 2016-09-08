#ifndef __DEBUG_H
#define __DEBUG_H


//#ifdef DEBUG
//#pragma message("DEBUG macro activated!")
//#else
//#pragma message("DEBUG macro not activated!")
//#endif


/****************Debug Macro Define********************/
 #define RECEIVED_BY_ASSISANT
//#define RECEIVED_BY_MATLAB

 #define READ_POSY     
 #define READ_ERRPOS
 #define READ_ERRANGLE
// #define READ_ERRACVEL
// #define READ_PID_POS
// #define READ_PID_ANGLE      
// #define READ_PID_ACVEL
// #define READ_ACTVEL_X
 #define READ_ACTVEL_Y
 #define READ_ADJ_VEL
// #define READ_CPU
// #define READ_WHEEL_VEL      //������
 #define READ_CLAMB_VEL
// #define READ_CURRENT        //������
// #define READ_PID_ERRCLA     //��PID����
// #define READ_CHANGE_VEL
 #define READ_CHANGE_WARD
// #define READ_ROTATE
 #define  READ_POSX
 #define  READ_ANGLE
 
// #define  READ_TRACK_LASER
 #define  READ_POST_LASER
 #define  READ_BASIC_LASER
// #define  READ_FANFLAG
// #define  READ_FANFLAG_LIGHT
// #define  READ_FLAG_BT
 #define  READ_WINDSPEED     //������
// #define  READ_WINDANGLE    //������Ƕ�
// #define  READ_SWITCH        
 #define  READ_HIGHT_LIGHT
// #define READ_ECOPOS
// #define READ_ECOANGLE
 #define  READ_TIME           //��ʱ��

void ChooseWay(void);
void ShowInstruction(void);
void ShowbyGraph(void);
void ShowtheEnd(void);
void ShowData(void);


#endif
