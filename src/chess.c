#include"chess.h"
#include"bmp.h"
#include"event.h"


#define BOARD_ROW 10 //棋盘的行
#define BOARD_COL 11 //棋盘的列
#define MAX_MOVES 11*10  //最大落子次数
#define START_X 35  // 棋盘左上角的x坐标
#define START_Y 30  // 棋盘左上角的y坐标
#define CELL_WIDTH 49  // 每个格子的宽度
#define CELL_HEIGHT 43  // 每个格子的高度
#define BOARDWIDTH 545 //棋盘的长度
#define BOARDHEIGHT 440 //棋盘的宽度

int chessboard[BOARD_ROW][BOARD_COL]  ={0};// 
int currentPlayer = -1 ; //1 黑棋 -1 白棋
int conut = 0 ; //计算下棋次数

typedef struct {
    int _x;    // 落子的x值
    int _y;    // 落子的y值
} Move;

Move blackStack[MAX_MOVES]; // 存储黑棋的落子记录
Move whiteStack[MAX_MOVES]; // 存储白棋的落子记录

int blackTop = -1; // 黑棋栈顶索引
int whiteTop = -1; // 白棋栈顶索引

void initboard()
{
    //棋盘归为初值
    for(int i = 0 ; i < BOARD_ROW ; i++)
    {
        for(int j = 0 ; j < BOARD_COL ; j++ )
            chessboard[i][j] = 0;
    }
    //初始化棋盘界面
    bmp_display(0,0,"qipan2.bmp");
    //栈初始化
    blackTop = -1;
    whiteTop = -1;
    //下棋次数归0
    conut = 0 ;

}

void chessboardgrame() //初始化棋盘
{
    bmp_display(0,0,"qipan2.bmp");
    int x0 = -1 ,y0 = -1 ; //保存点击坐标
    int flag = -1 , t = 0;
    currentPlayer = -1; //白棋
    //int cu=49 , le = 43;
    while(1)
    {
        flag = handleInput( &x0 , &y0 ,&t);
        if(flag == 5 ) //开始游戏，可以下棋     flag
        {
            printf("开始游戏\n");
            t = 5;
            initboard();
        }
        else if( flag == 6 )
        {
            surrender(flag);
            printf(" %d 投降函数逻辑\n",flag);
        }
        else if( flag == 7 )
        {
            undoMove(); 
            printf(" 7 悔棋函数逻辑\n");
        }
        else if( flag == 8 )
        {
            printf(" 8  退出游戏，返回电子相册\n");
            flag = electro_photo( flag );
        }
    }
}
int handleInput( int *x0 ,  int *y0 , int  *temp  ) //处理按钮
{
    // 处理用户输入，例如鼠标点击等 -- 开始： 5  投降：6  退出： 8 悔棋：7
    int flag = get_slide(x0, y0);
    printf("x:%d y:%d \n",*x0,*y0);
    if((*x0 > 600 && *x0 < 765) && (*y0 > 200 && *y0 < 260)) //点击开始按钮
    {
        flag = 5;
    }
    else if( ( *x0 > 600 && *x0 < 765 ) && ( *y0 > 275 && *y0 < 335)) //点击投降按钮
    {
        flag = 6;
    }
    else if( (*x0 > 600 && *x0 < 765) && ( *y0 > 350 && *y0 < 410)) //点击悔棋按钮
    {
        flag = 7;
    }
    else if((*x0 > 620 && *x0 < 760)&&(*y0 > 420 && *y0 < 480))       // printf("退出游戏\n");
    {
        flag = 8;
    }
    else if(*temp == 5 && *x0 >= START_X && *x0 <= START_X + BOARDWIDTH && *y0 >= START_Y && *y0 <= START_Y + BOARDHEIGHT) // 如果点击棋盘 并且 temp = 5
    {
            printf("下棋\n");
            int ch = 0 ; //判断下棋是否成功成功 为 5
            placePiece(currentPlayer , *x0 , *y0  ,&ch);
            // 2.下棋成功切换玩家
            if (ch == 5)
                currentPlayer = -currentPlayer;
    }
    return flag;
}

void placePiece(int player, int x, int y ,int *ch)
{
    // 转换坐标为棋盘索引
    int col = (x - START_X) / CELL_WIDTH; // 列索引
    int row = (y - START_Y) / CELL_HEIGHT; // 行索引
    //定义栈，保存棋子的x值和y值
    Move move;
    move._x = START_X + col * CELL_WIDTH;
    move._y = START_Y + row * CELL_HEIGHT;
    // 检查索引有效性
    if (col >= 0 && col < BOARD_COL && row >= 0 && row < BOARD_ROW && chessboard[row][col] == 0)
    {
        chessboard[row][col] = player; // 玩家为黑棋(1)或白棋(-1)
        printf("下棋行列 row %d  col %d", row , col);
        // 准备绘制棋子
        if (player == 1)  //黑棋入黑棋栈
        {
            bmp_display(move._x,move._y, "heiqi.bmp");
            bmp_display(621,100,"bai.bmp");
            conut++;
            printf("黑棋玩家下棋 %d %d\n" , currentPlayer, conut);
            blackStack[++blackTop] = move; // 将黑棋落子记录压入栈中
        } else 
        {
            bmp_display(move._x,move._y, "baiqi.bmp");
            bmp_display(621,100,"hei.bmp");
            conut++;
            printf("白棋玩家下棋 %d %d\n" , currentPlayer, conut);
            whiteStack[++whiteTop] = move;
        }
        printf("下棋成功\n");
        *ch = 5;
    } 
    else 
    {
        printf("点击错误,棋子重复(%d, %d) .\n", row, col);
    }
    // 检查是否有玩家获胜
    int winner = isWin();
    if (winner != -1)
    {
        if (winner == 0) // 白棋赢
        {
            printf("白棋胜利 %d \n", conut);
            bmp_display(0,0,"winbai.bmp");
        }
        else if (winner == 1) // 黑棋赢
        {
            printf("黑棋胜利 %d\n", conut);
            bmp_display(0,0,"winhei.bmp");
        }
        sleep(3);
        chessboardgrame();
    }
    else if( conut == MAX_MOVES)
    {
        printf(" conut : %d 和局!!!\n", conut);
        chessboardgrame();
    }

}
const int dir[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
//上、下、左、右、左上、右上、左下、右下）
int isWin()
{
    for (int row = 0; row < BOARD_ROW; row++)
    {
        for (int col = 0; col < BOARD_COL; col++)
        {
            if (chessboard[row][col] != 0) // 如果当前位置有棋子
            {
                for (int d = 0; d < 8; d++) // 检查8个方向
                {
                    int count = 1; // 初始化计数器，当前位置的棋子也算作1个
                    int x = row + dir[d][0]; // 计算下一个位置的坐标
                    int y = col + dir[d][1];

                    // 检查沿着该方向是否有连续的相同棋子
                    while (x >= 0 && x < BOARD_ROW && y >= 0 && y < BOARD_COL && chessboard[x][y] == chessboard[row][col])
                    {
                        count++; // 计数器加1
                        x += dir[d][0]; // 更新下一个位置的坐标
                        y += dir[d][1];
                    }
                    // 如果计数器大于等于5（连成一条直线），则该玩家获胜
                    if (count >= 5)
                    {
                        return chessboard[row][col] == -1 ? 0 : 1; // 黑棋赢返回0，白棋赢返回1
                    }
                }
            }
        }
    }
    return -1; // 没有玩家获胜
}
/*
    规则：如果 轮到白棋开始下棋，点了悔棋按钮，把最后下的黑棋去掉，黑棋重写下棋
          如果 轮到黑棋下棋，点了悔棋按钮，把最后的白棋去掉，白棋重写下棋
*/
void undoMove() 
{
    printf("悔棋: %d \n" ,currentPlayer);
    if (currentPlayer == -1 && blackTop >= 0)  // -1 表示当前玩家是白棋 -- 去掉黑棋
    {
        printf("当前玩家是白棋，黑棋玩家重新下\n");
        // 减少下棋次数
        conut--;
        // 恢复棋盘状态 -- 棋盘对应的黑棋坐标 置0
        int col = (blackStack[blackTop]._x - START_X ) / CELL_WIDTH;
        int row = (blackStack[blackTop]._y - START_Y) / CELL_HEIGHT;
        chessboard[row][col] = 0;
        printf(" 删除黑棋行列 row %d  col %d\n", row , col);
        // 删除黑棋图像
         //1.重新显示棋盘
         bmp_display(0,0,"qipan.bmp");
          //2.白棋全部重写画
         for( int i = 0 ; i <= whiteTop ; i++)
         {
            bmp_display(whiteStack[i]._x,whiteStack[i]._y,"baiqi.bmp");
         }
         //3.黑棋少一个
         for(int i = 0 ; i < blackTop; i++)
         {
            bmp_display(blackStack[i]._x,blackStack[i]._y,"heiqi.bmp");
         }
        blackTop--;
        // 切换玩家
        currentPlayer = 1;
        bmp_display(621,100,"hei.bmp");
    } 
    else if (currentPlayer == 1 && whiteTop >= 0) // 如果当前玩家是黑棋，去掉白棋，白棋重新下棋
    {
        // 减少下棋次数
        printf("当前玩家是黑棋，白棋玩家重新下\n");
        conut--;
        // 恢复棋盘状态 -- 棋盘对应的b棋坐标 置0
        int col = (whiteStack[whiteTop]._x - START_X ) / CELL_WIDTH;
        int row = (whiteStack[whiteTop]._y - START_Y) / CELL_HEIGHT;
        chessboard[row][col] = 0;
        printf(" 删除白棋行列 row %d  col %d\n", row , col);
        // 删除白棋图像
         //1.重新显示棋盘
         bmp_display(0,0,"qipan2.bmp");
          //2.白棋少一个
         for( int i = 0 ; i < whiteTop ; i++)
         {
            bmp_display(whiteStack[i]._x,whiteStack[i]._y,"baiqi.bmp");
         }
         //3.黑棋全部重新画
         for(int i = 0 ; i <= blackTop; i++)
         {
            bmp_display(blackStack[i]._x,blackStack[i]._y,"heiqi.bmp");
         }
        // 弹出白棋落子记录
        whiteTop--;
        // 切换玩家
        currentPlayer = -1;
        //白棋玩家下棋
        bmp_display(621,100,"bai.bmp");
    } 
    else 
    {
        printf("无法悔棋，没有可撤销的落子。\n");
    }
}


void surrender(int flag) 
{
    //1.如果conut的值为奇数 ru 1 3 5 7 则下一次是黑棋（点击投降）则白棋赢
    //2.如果是偶数则黑棋赢
    //3.显示谁赢并且调用初始化函数
    if(flag == 6 && conut > 0) //按了投降按钮
    {
        if( conut % 2 == 0)      //conut 是偶数
        {
            printf("黑棋胜利 %d \n") , conut;
            bmp_display(0,0,"winhei.bmp");
        }
        else if(conut % 2 != 0)
        {
            printf("白棋胜利 %d\n",conut);
            bmp_display(0,0,"winbai.bmp");
        }
    }
    else if( conut == 0 ) //第一次白棋下棋，投降，黑棋获胜
    {
        printf("没有下棋--黑棋获胜\n");
        bmp_display(0,0,"winhei.bmp");
    }
    sleep(3);
    chessboardgrame();
}

