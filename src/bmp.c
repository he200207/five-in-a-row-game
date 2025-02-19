#include"chess.h"
#include"bmp.h"
#include"event.h"

#define MAX_IMAGES 5

char pix[100];

/*
	x0:要显示图片左上角的坐标 列
	y0:要显示图片左上角的坐标 行
	bmp_path:图片的路径
*/
int electro_photo( int ev)
{
    //电子相册
    int x = -1 , y = -1;
    int  i = 4; //i = 0 ,1 ,2 3, 4
    char* bmpname[MAX_IMAGES] = {"1.bmp", "2.bmp", "3.bmp", "4.bmp", "5.bmp"}; 
    bmp_display(0,0,bmpname[i]);
    while (1)
    {
        ev = get_slide(&x,&y);
        if( ev == 0 ) //如果点击进入棋盘界面
        {
            chessboardgrame();
            break;
        }
        // 根据滑动事件更新图片索引
        if (ev == 3) 
        { // 左滑
            i = (i + 1) % MAX_IMAGES;
            printf("left\n");
        } else if (ev == 4) 
        { // 右滑
            i = (i + MAX_IMAGES - 1) % MAX_IMAGES;
            printf("right\n");
        }
        // 显示新图片
        bmp_display(0, 0, bmpname[i]);
    }
    return ev;
}

void bmp_display(int x0,int y0,char *bmp_path)
{
    //打开图片
    int bmp_fd = open(bmp_path,O_RDWR);
    if(bmp_fd == -1)
    {
        perror("open bmp failed");
        return ;
    }

    //读取图片宽高色深
    int width = 0;
    lseek(bmp_fd,0x12,SEEK_SET);//跳光标
    read(bmp_fd,&width,4);//读取数据
    int height = 0;
    lseek(bmp_fd,0x16,SEEK_SET);//跳光标
    read(bmp_fd,&height,4);//读取数据
    int depth = 0;
    lseek(bmp_fd,0x1c,SEEK_SET);//跳光标
    read(bmp_fd,&depth,2);//读取数据
   // printf("width = %d,height = %d,depth = %d\n",width,height,depth);

    //计算像素数组的大小
    int line_bytes = abs(width)*depth/8;
    int laizi = 0;//癞子
	if(line_bytes%4)
	{
		laizi = 4-line_bytes%4;
	}
    line_bytes+=laizi;
    int total_bytes = line_bytes*abs(height);

    //读取像素数组数据
    char pix[total_bytes];//用来保存读到的像素数组的数据
    //char *pix = malloc(sizeof(char)*total_bytes);
    lseek(bmp_fd,54,SEEK_SET);//跳转光标到像素数组的位置 54
    read(bmp_fd,pix,total_bytes);

    //解析像素数组的数据
    unsigned char a,r,g,b;//0xff0000
    int i = 0;
    unsigned int color;
    /*
        char a = -1;
        a << 8 => int
        char -> int
        11111111 32个1

        g:11101001
        b:10000000
        r:01000100
        a:11100000
        g<<8  unsigned char ->int
        00000000 00000000 11101001 00000000
        g<<8|b
        00000000 00000000 11101001 00000000
        00000000 00000000 00000000 10000000
        00000000 00000000 11101001 10000000
        color = a<<24|r<<16|g<<8|b
        00000000 00000000 11101001 00000000
        00000000 00000000 00000000 10000000
        00000000 01000100 00000000 00000000
        11100000 00000000 00000000 00000000
        11100000 01000100 11101001 10000000
    */
    for(int y = 0;y<abs(height);y++)//行
    {
        for(int x = 0;x<abs(width);x++)//遍历图片的宽度 列
        {
            b = pix[i++];
            g = pix[i++];
            r = pix[i++];
            if(depth == 32)
                a = pix[i++];
            else if(depth == 24)
                a = 0;
            //组装一个颜色
            color = a<<24 | r<<16 | g<<8 | b;
            //画 第1个参数列 第二个参数行 第三个参数颜色
            lcd_draw_point(width>0?(x+x0)%800:(abs(width)-x-1+x0)%800,height>0?(abs(height)-y-1+y0)%480:(y+y0)%480,color);
        }
        //跳过癞子
        i+=laizi;
    }

    //关闭图片
    close(bmp_fd);

}