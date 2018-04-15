#include <iostream>
#include <conio.h>
#include "ColorDisplay.h"
#include<Windows.h>
#include<string.h>
#include<ctime>
#include<cstdlib>
using namespace std;

ColorCharacter cc;
ColorDisplay cd;
COORD pos;

#define hoanh 30
#define tung 25

enum TrangThai {STOP = 0, UP , DOWN, LEFT, RIGHT };

struct ToaDo
{
	int x;
	int y;           // y là tung do , x là hoành do
};

struct Snake
{
    ToaDo dot[51];
	int n;
	TrangThai tt;

}    ;

void KhoiTao(Snake &snake)
{
	snake.n = 2;
	snake.dot[0].x = 2;
	snake.dot[0].y = 1;
	snake.dot[1].x = 1;
	snake.dot[1].y = 1;

	snake.tt = RIGHT;
}

void Draw(int dong, int cot, int kitu, int mau, COORD &pos, ColorCharacter &cc)
{
    pos.X = cot;
    pos.Y = dong;
    cc.character = kitu;
    cc.color = (unsigned) mau;
    cd.setColorCharacterAtPosition(cc, pos);
}

void Screen()
{
    for(int i=0; i < 25; i ++ )
        {
            for(int j = 0; j < 80 ; j++)
            {
                if (j < 40) {
                    if  ( i == 0 || i == 24) Draw(i,j,'#',7,pos,cc);
                    else
                    {
                        if ( j == 0 ) Draw(i,0,'#',7,pos,cc);
                        else if ( j == 39 ) Draw(i,39,'#',7,pos,cc);
                    }
                }
            }
        }
}


void HienThi(Snake snake)
{
    Screen();

    for(int i = 0 ; i < snake.n ; i ++ )
    {
        if ( i != 0 )
        {
            Draw(snake.dot[i].y, snake.dot[i].x,4,7, pos, cc);
        }
        else Draw(snake.dot[0].y, snake.dot[0].x, 2, 7, pos, cc);
    }
}

void DieuKhien_Dichuyen(Snake &snake)
{
    for (int i = snake.n - 1; i > 0; i--)
	{
		snake.dot[i].x = snake.dot[i - 1].x;
		snake.dot[i].y = snake.dot[i - 1].y;
	}
	if (_kbhit())      // neu phat hien co phim nhan vao
	{

		// Dieu khien dau ran
		if (GetAsyncKeyState(VK_LEFT)) snake.tt = LEFT;
		else if (GetAsyncKeyState(VK_RIGHT)) snake.tt = RIGHT;
		else if (GetAsyncKeyState(VK_UP)) snake.tt = UP;
		else if (GetAsyncKeyState(VK_DOWN)) snake.tt = DOWN;

	}
	if (snake.tt == UP) snake.dot[0].y--;
	else if (snake.tt == DOWN) snake.dot[0].y++;
	else if (snake.tt == LEFT) snake.dot[0].x--;
	else if (snake.tt == RIGHT) snake.dot[0].x++;
}



int main()
{
    ToaDo td;
    Snake snake;

    cc.character = ' ';

    KhoiTao(snake);

    while (1)
    {
       HienThi(snake);

       DieuKhien_Dichuyen(snake);

       Sleep(300);
    }
    return 0;
}
