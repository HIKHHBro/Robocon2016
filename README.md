# HybridRobot
Source of HybridRobot

                       ｜      Angle    |  Gyro_Union.GyroAngle |
##顺时针自旋- ｜ increase |       decrease        |
##逆时针自旋+ ｜ decrease |       increase        |

##气阀：
- 0，1--------夹风扇  
    0-off  抓风扇  
	1-off  竖起

- 2，3 ------底盘夹柱子
    
- 6，7--------顶上夹柱子  
   6-on  &&  7-off 抱柱子  
   6-off &&  7-on  放柱子

##舵机减速比 52/30


##最大轮速度:    220000脉冲（合为1969.34mm/s)
##全程轨迹长度：19485.68mm   、改良后：19227.750

> `err`|—————————NB——NM    NS    Z   PS   PM   PB  
errValue[-300,300]—— -300 -200  -100  0   100  200  300   
errAngle   [-15,15]———  -15  -10   -5    0    5    10   15  
errActvel  [-600,450]—— -600 -400  -200  0   200  400  600
> errc       
errcValue  [-60,60]——— -60   -40   -20   0   20   40   60  
errcAngle  [-3, 3 ]———  -3    -2    -1    0   1    2    3  
errcActvel [-90,90]—— -90   -60   -30   0   30   60   90


> 2*n*(x-(a+b)/2)/(b-a)

##换场改动地方：1.坐标定位相关   初始角度   红场30  蓝场-30
             


