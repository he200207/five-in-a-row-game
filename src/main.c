#include "lcd.h"
#include "bmp.h"
#include "event.h"
#include "font.h"
#include <pthread.h>
#include"chess.h"

#define MAX_IMAGES 5

int main()
{
    //屏幕初始化
    lcd_init();
    lcd_clear();
    int dott = -1;//获取是否点击电子相册
    //pthread_t eid  ;//保存电子相册id, 五子棋id
    //electro_photo( );
    //创建 电子相册 线程
   // pthread_create(&eid, NULL ,electro_photo , &dott);


    //use_font( );
    //     //获取滑动或点击坐标?
    int r = 0;
    int read_x = -1,read_y = -1;
    while(1)
    {
        if( r < 5)
            r = electro_photo(r);
        else
            break;
    }
    //屏幕解初始化
    lcd_uninit();
}