#include"event.h"
#include "lcd.h"
#include <linux/input.h>
// #include <linux/input-event-codes.h>


int get_slide(int *read_x , int *read_y)
{
	int x1 = -1,y1 = -1;//保存第一个点的坐标
	int x2 = -1,y2 = -1;//保存最后一个点的坐标
	struct input_event ts;//保存触摸数据
	int touch_fd = open("/dev/input/event0",O_RDWR);
	
	while(1)
	{
		//读
		read(touch_fd,&ts,sizeof(struct input_event));
		
		if(ts.type == EV_ABS)//触摸屏事件
        {
            if(ts.code == ABS_X)//x轴
            {
                //获取x的坐标
                if(x1>=0 && y1>=0)//第一个点已经获取
                {
                	x2 = ts.value*800/1024;
                }
                else
                {
                	*read_x = x1 = ts.value*800/1024;
                }
            }
            else if(ts.code == ABS_Y)//y轴
            {
            	//获取y的坐标
                if(x1>=0 && y1>=0)//第一个点已经获取
                {
                	y2 = ts.value*480/600;
                }
                else
                {
                	*read_y = y1 = ts.value*480/600;
                }
            }
        } 
        //printf("x：%d y:%d\n",read_x,read_y);     
        if(ts.type == EV_KEY && ts.code == BTN_TOUCH)//触摸屏点击
        {
        	if(ts.value == 0)
        	{
        		if(x2<0||y2<0)
                {
                    return 0;//点击
                }
        		if(abs(x1-x2) < abs(y1-y2) && y1>y2)//上滑
        			return 1;
        		else if(abs(x1-x2) < abs(y1-y2) && y1<y2)//下
        			return 2;
                else if(abs(x1-x2) > abs(y1-y2) && x1 > x2) // 左滑
                    return 3;
                else if(abs(x1-x2) > abs(y1-y2) && x1 < x2) //右滑
                    return 4;
        	}
        }
        
	}  
    close(touch_fd);
}