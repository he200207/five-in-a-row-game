#include"font.h"

void use_font( )
{
    Init_Font();
    Display_characterX(200,200,"你好世界",0xff0000,6); 
	UnInit_Font();
}

