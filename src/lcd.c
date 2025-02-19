#include "lcd.h"

int lcd_fd = -1; 
int *plcd = NULL; 
/*
	画点子函数
	i:列
	j:行
	color:颜色
*/
void lcd_draw_point(int i,int j,unsigned int color)
{
	if(i>=0&&i<800 && j>=0&&j<480)
		*(plcd+800*j+i) = color;
}

//lcd初始化
void lcd_init()
{
    //打开屏幕
    lcd_fd = open("/dev/fb0",O_RDWR);
    if(lcd_fd == -1)
    {
        perror("open lcd failed");
        return ;
    }

    //映射
    plcd = (int*)mmap(NULL,800*480*4,PROT_EXEC|PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);
    if(plcd == NULL)
    {
        perror("mmap faile");
        return ;
    }

}

void lcd_clear()//清屏函数
{
            //显示颜色
        for(int i = 0;i<480;i++)//�?
        {
            for(int j = 0;j<800;j++)//�?
            {
                //画点
                lcd_draw_point(j,i,0x000000);
            }
        }
}
//lcd解初始化
void lcd_uninit()
{
    //解映射
    munmap(plcd, 800*480*4);

    //关闭屏幕
    close(lcd_fd);

}