#include <iostream>
#include<cstring>
#include <conio.h>
#include "ColorDisplay.h"
#include<Windows.h>
#include<string.h>
#include<ctime>
#include<cstdlib>
#include<sstream>
#include <fstream>
#include <vector>

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
	int xoaduoiitem1;
	TrangThai tt;
	TrangThai stop;
	ToaDo xoaduoi;
	int dem;
	int timelimit;
	int score;

	int timesleep;
	int liveup;
    string scoreSTR;
}    ;

struct HoaQua
{
    ToaDo td;
    ToaDo xoahoaqua;

};

struct NongDan
{
    ToaDo td;
    ToaDo xoand;

};
struct  Menu
{
	int x, y;
	string data;
};


class Items
{
public:
    ToaDo td;
    ToaDo xoaitem;
    char kitu;
    int mau;
    bool checkitem;
    int timelimit;
};


void SetItems(Items &item, vector<Items> &listItems)
{
//  item - 1 --- +5 diem
    item.td.x =1+ rand() % (hoanh-2);
    item.td.y =1+ rand() % (tung-2);
    item.kitu = 48;
    item.mau = 8;

    item.xoaitem;
    item.checkitem = false;
    item.timelimit = 0;
    listItems.push_back(item);

//  item - 2 --- giam dot
    item.td.x =1+ rand() % (hoanh-2);
    item.td.y =1+ rand() % (tung-2);
    item.kitu = 49;
    item.mau = 9;

    item.xoaitem;
    item.checkitem = false;
    item.timelimit = 0;
    listItems.push_back(item);

    //  item - 3 --- giam toc do
    item.td.x =1+ rand() % (hoanh-2);
    item.td.y =1+ rand() % (tung-2);
    item.kitu = 50;
    item.mau = 10;

    item.xoaitem;
    item.checkitem = false;
    item.timelimit = 0;
    listItems.push_back(item);

}

string genericToString(int &t)
{
    stringstream oss;
    oss << t;
    return oss.str();
}


void KhoiTao(Snake &snake, HoaQua &hq, NongDan &nd)
{
	snake.n = 2;
	snake.xoaduoiitem1=0;
	snake.dot[0].x = 2;
	snake.dot[0].y = 10;
	snake.dot[1].x = 1;
	snake.dot[1].y = 10;

	snake.tt = RIGHT;
	snake.score = 0;
	snake.dem = 0;
	snake.timelimit = 0 ;

    snake.timesleep = 300;
    snake.scoreSTR = "";

	hq.td.x = 10;
	hq.td.y = 10;


	nd.td.x = rand() % hoanh;
	nd.td.y = 0;
	while (nd.td.x == 0 || nd.td.x == snake.dot[0].x || nd.td.x == hoanh - 1)
	{
		nd.td.x = rand() % hoanh;
	}
}

void Draw(int dong, int cot, int kitu, int mau, COORD &pos, ColorCharacter &cc)
{
    pos.X = cot;
    pos.Y = dong;
    cc.character = kitu;
    cc.color = (unsigned) mau;
    cd.setColorCharacterAtPosition(cc, pos);
}


void DrawString( string str, int dong, int cot, int mau)
{
    for( int i = 0 ; i < str.length(); i ++)
    {
        Draw(dong, cot, str[i], mau, pos, cc);
        cot++;
    }
}

void DrawTextSnake()
{
    ifstream instream;
    instream.open("textsnake.txt", ios::in);
    while(!instream.eof())
    {
        string str;
        getline(instream, str);
        if ( str == "end.") break;
        cout<< str;
        cout<<endl;

    }
    instream.close();
}


void DrawScreen(Snake &snake,Items &item, vector<Items> &listItems)
{
    for(int i=0; i < tung; i ++ )
        {
            for(int j = 0; j < 80 ; j++)
            {
                if (j < hoanh) {
                    if  ( i == 0 || i == tung -1) Draw(i,j,'#',7,pos,cc);
                    else
                    {
                        if ( j == 0 ) Draw(i,0,'#',7,pos,cc);
                        else if ( j == hoanh-1 ) Draw(i,hoanh-1,'#',7,pos,cc);
                    }
                }
            }
        }

    DrawString("0 - Ban duoc cong +5 diem.",  5, 40, 7);
    DrawString("1 - Ran ngan lai.",  6, 40, 7);
    DrawString("2 - Giam toc do di chuyen.",  7, 40, 7);
    DrawString("SPACE de tam dung", 8, 40, 7);
    DrawString("Score : " + snake.scoreSTR, 10, 50, 9);

}

// hop le tra ve true, khong hop le tra ve false
bool Kiemtrahoaqua(const Snake &snake, const HoaQua &hq)
{
	bool check = true;
	// hoa qua trung voi rao
	if (hq.td.x == 0 || hq.td.x == hoanh - 1
		|| hq.td.y == 0 || hq.td.y == tung - 1)
	{
		check = false;
	}

	// hoa qua trung voi dot cua ran
	for (int i = 0; i < snake.n; i++)
	{
		if (snake.dot[i].x == hq.td.x && snake.dot[i].y == hq.td.y)
		{
			check = false;
			break;
		}
	}

	return check;
}

bool KiemtraItem(Snake snake, HoaQua hq, int chisoItem, vector<Items> listItems)
{
    bool check = true;
    // Item trung voi dot cua ran
	for (int i = 0; i < snake.n; i++)
	{
		if (snake.dot[i].x == listItems[chisoItem].td.x && snake.dot[i].y == listItems[chisoItem].td.y)
		{
			check = false;
			break;
		}
	}
		//hoa qua trung voi Item
	if (hq.td.x == listItems[chisoItem].td.x && hq.td.y == listItems[chisoItem].td.y )
    {
        check = false;
    }
	return check;
}


void HienThi(Snake &snake, HoaQua &hq, NongDan &nd, Items &item, vector<Items> listItems, int chisoItem)
{
    // in ra rao
    DrawScreen(snake, item, listItems);

    // Xoa
    if ( snake.xoaduoiitem1 != 0 )
    {
        for( int i=snake.xoaduoiitem1 ; i > 1; i--)
        {
            Draw(snake.dot[i].y, snake.dot[i].x, ' ', 7, pos, cc);
        }
        snake.xoaduoiitem1 = 0;
    }

    Draw(snake.xoaduoi.y, snake.xoaduoi.x, ' ', 7, pos, cc);
    Draw(hq.xoahoaqua.y, hq.xoahoaqua.x, ' ', 7, pos, cc);
    for( int i = 0 ; i <= 2; i ++)
    {
    Draw(listItems[i].xoaitem.y, listItems[i].xoaitem.x, ' ', 7, pos, cc);
    }
    if (nd.xoand.y != 0 )
    {
        Draw(nd.xoand.y, nd.xoand.x, ' ', 7, pos, cc);
    }

    // in ra hoa qua
    Draw(hq.td.y, hq.td.x, 15, 12, pos, cc);

    // in ra nguoi nong dan
    Draw(nd.td.y, nd.td.x, 'X', 10, pos, cc);

    // in ra items
    if ( snake.dem % 4 == 0 && snake.dem != 0 && snake.timelimit < 30)
    {
    Draw(listItems[chisoItem].td.y, listItems[chisoItem].td.x, listItems[chisoItem].kitu, rand() % 16, pos, cc);
    }

    // in ra ran
    for(int i = 0 ; i < snake.n ; i ++ )
    {
        if ( i != 0 )
        {
            Draw(snake.dot[i].y, snake.dot[i].x, 4, 14, pos, cc);
        }
        else Draw(snake.dot[0].y, snake.dot[0].x, 2, 14, pos, cc);
    }
}

void DieuKhien_Dichuyen(Snake &snake, NongDan &nd)
{
    snake.xoaduoi.x = snake.dot[snake.n-1].x;
    snake.xoaduoi.y = snake.dot[snake.n-1].y;

    for (int i = snake.n - 1; i > 0; i--)
	{
		snake.dot[i].x = snake.dot[i - 1].x;
		snake.dot[i].y = snake.dot[i - 1].y;
	}

	if (_kbhit())      // neu phat hien co phim nhan vao
	{

        // Dieu khien dau ran
		if (GetAsyncKeyState(VK_LEFT) && snake.tt != RIGHT) snake.tt = LEFT;
		else if (GetAsyncKeyState(VK_RIGHT) && snake.tt != LEFT) snake.tt = RIGHT;
		else if (GetAsyncKeyState(VK_UP) && snake.tt != DOWN) snake.tt = UP;
		else if (GetAsyncKeyState(VK_DOWN) && snake.tt != UP) snake.tt = DOWN;

		else if (GetAsyncKeyState(VK_SPACE)) snake.tt = STOP;

	}
	if (snake.tt == UP) snake.dot[0].y--;
	else if (snake.tt == DOWN) snake.dot[0].y++;
	else if (snake.tt == LEFT) snake.dot[0].x--;
	else if (snake.tt == RIGHT) snake.dot[0].x++;



    // NongDan chay xuong

    nd.xoand.x = nd.td.x;
    nd.xoand.y = nd.td.y;

	nd.td.y++;
	if (nd.td.y == tung)
	{
		nd.td.y = 0;
		nd.td.x = rand() % hoanh;
		while (nd.td.x == 0 || nd.td.x == hoanh - 1)
		{
			nd.td.x = rand() % hoanh;
		}
	}

}



void XuLyItems(Snake &snake, Items &item, vector<Items> &listItems, int &chisoItem)
{
    if (listItems[chisoItem].checkitem = true)
    {
        listItems[chisoItem].checkitem = false;
        if ( chisoItem == 0 )
        {
            snake.score+=5;
        }
        else if ( chisoItem == 1 )
        {
            snake.xoaduoiitem1 = snake.n;
            snake.n = 2;
        }
        else if ( chisoItem == 2)
        {
            snake.timesleep = 300;
        }
    }

}


int XuLy(Snake &snake, HoaQua &hq, NongDan &nd, Items &item, vector<Items> &listItems, int &chisoItem)
{
    // Xu ly Gameover

    // Ran va vao tuong
        if (snake.dot[0].x <= 0 || snake.dot[0].x >= hoanh - 1
		|| snake.dot[0].y <= 0 || snake.dot[0].y >= tung - 1)
	{
		return -2;
	}


	// Ran can vao duoi
	for (int i = 1; i < snake.n; i++)
	{
		if (snake.dot[0].x == snake.dot[i].x
			&&  snake.dot[0].y == snake.dot[i].y)
		{
			return -2;
		}
	}

	// cham vao nguoi nong dan
	for (int i = 0; i < snake.n; i++)
	{
		if (nd.td.x == snake.dot[i].x
			&&  nd.td.y == snake.dot[i].y  )
		{
		    nd.td.y = nd.td.y-1;
			return -1;
		}
	}
	if ( (snake.dot[0].y - nd.td.y == 1) && snake.tt == UP && snake.dot[0].x == nd.td.x )
    {
        return -1;
    }

// xu ly an hoa qua
    if (snake.dot[0].x == hq.td.x && snake.dot[0].y == hq.td.y)
    {
   snake.score++;
   if (snake.dem == 4 ) snake.dem = snake.dem;
   else if (snake.dem < 4) snake.dem++;



      // ran dai ra, them dot o duoi
		snake.n++;
		if (snake.dot[snake.n - 2].x == snake.dot[snake.n - 3].x)
		{
			snake.dot[snake.n - 1].x = snake.dot[snake.n - 2].x;
			if (snake.dot[snake.n - 2].y - snake.dot[snake.n - 3].y == 1)
			{
				snake.dot[snake.n - 1].y = snake.dot[snake.n - 2].y + 1;
			}
			else {
				snake.dot[snake.n - 1].y = snake.dot[snake.n - 2].y - 1;
			}
		}
		else if (snake.dot[snake.n - 2].y == snake.dot[snake.n - 3].y)
		{
			snake.dot[snake.n - 1].y = snake.dot[snake.n - 2].y;
			if (snake.dot[snake.n - 2].x - snake.dot[snake.n - 3].x == 1)
			{
				snake.dot[snake.n - 1].x = snake.dot[snake.n - 2].x + 1;
			}
			else {
				snake.dot[snake.n - 1].x = snake.dot[snake.n - 2].x - 1;
			}

		}
        // ran chay nhanh hon
        if ( snake.timesleep >= 30 )
        {
            snake.timesleep = snake.timesleep - 10;
        }

        // Update hoa qua
        hq.xoahoaqua.x = hq.td.x;
        hq.xoahoaqua.y = hq.td.y;

        hq.td.x = rand() % hoanh;
        hq.td.y = rand() % tung;
        while (!Kiemtrahoaqua(snake, hq))
        {
                hq.td.x = rand() % hoanh;
                hq.td.y = rand() % tung;
        }


    }

    // ran an duoc item
    if ( snake.dem % 4 == 0 && snake.dem != 0  )
    {
        if ( snake.timelimit < 30)
        {
            snake.timelimit++;
            if ( snake.dot[0].x == listItems[chisoItem].td.x && snake.dot[0].y == listItems[chisoItem].td.y)
            {
                listItems[chisoItem].checkitem = true;
                // goi ham xu ly
                XuLyItems(snake, item, listItems, chisoItem);

                snake.dem = 0;
                snake.timelimit= 0;

                chisoItem = rand() % 3;
                do
                {
                listItems[chisoItem].td.x =1 + rand() % (hoanh-2);
                listItems[chisoItem].td.y =1 +  rand() % (tung-2);
                }
                 while( !KiemtraItem(snake, hq, chisoItem, listItems));
            }
        }
        else if ( snake.timelimit == 30 && snake.dot[0].x != listItems[chisoItem].td.x && snake.dot[0].y != listItems[chisoItem].td.y )
        {
            snake.dem=0;
            snake.timelimit =0;
            listItems[chisoItem].xoaitem.x = listItems[chisoItem].td.x;
            listItems[chisoItem].xoaitem.y = listItems[chisoItem].td.y;
            chisoItem = rand() % 3;
            do
            {
            listItems[chisoItem].td.x =1 + rand() % (hoanh-2);
            listItems[chisoItem].td.y =1 +  rand() % (tung-2);
            }
             while( !KiemtraItem(snake, hq, chisoItem, listItems));
        }
    }



    snake.scoreSTR = genericToString(snake.score);
}






void Show_Menu(vector<Menu> &listMenu, Menu &part)
{
    part.x = 37;
    part.y = 15;
    part.data = "Choi game";
    listMenu.push_back(part);

    part.x = 37;
    part.y = 17;
    part.data = "Cach choi";
    listMenu.push_back(part);

    part.x = 37;
    part.y = 19;
    part.data = "Thong tin";
    listMenu.push_back(part);

    part.x = 37;
    part.y = 21;
    part.data = "Thoat game";
    listMenu.push_back(part);

    for(int i = 0 ; i < listMenu.size(); i ++)
    {
        DrawString(listMenu[i].data, listMenu[i].y, listMenu[i].x, 7);
    }
}


void MoveSelect(const vector<Menu> &listMenu, int &number)
{
    while(1)
    {

        DrawString("--->"+listMenu[number].data, listMenu[number].y, listMenu[number].x-4, 10);
        if ( _kbhit() )
        {
            int key = _getch();

            if ( key == 72 ) // up
                {
                     DrawString("    "+listMenu[number].data, listMenu[number].y, listMenu[number].x-4, 7);
                    number--;
                    if ( number == -1 ) number = 3;
                }
                else if (key == 80)  // down
                {
                    DrawString("    "+listMenu[number].data, listMenu[number].y, listMenu[number].x-4, 7);
                    number++;
                    if ( number == 4) number = 0;
                }
                DrawString("--->"+listMenu[number].data, listMenu[number].y, listMenu[number].x-4, 10);
                break;
        }

    }

}

bool Checkselect()
{
    if ( GetAsyncKeyState(VK_RETURN))
    {
        return true;
    }
    else return false;
}

void ChoiGame()
{
    system("cls");
    srand(time(0));
    Snake snake;
    HoaQua hq;
    NongDan nd;

    Items item;
    vector<Items> listItems;

    cc.character = ' ';


    int check = 0;
    int chisoItem=rand() % 3;

    SetItems(item, listItems);
    KhoiTao(snake, hq, nd);

    while (1)
    {
       HienThi(snake, hq, nd, item, listItems, chisoItem);

       if ( GetAsyncKeyState(VK_SPACE) )
       {
           snake.stop = snake.tt;
           snake.tt = STOP;
           DrawString("GAME PAUSE", 13, 55, 7);
           DrawString("Enter de tiep tuc", 14, 55, 7);
           while( _getch()!=13 );
           DrawString("          ", 13, 55, 7);
           DrawString("                 ", 14, 55, 7);
           snake.tt = snake.stop;
       }
       DieuKhien_Dichuyen(snake, nd);

       check = XuLy(snake, hq, nd, item, listItems, chisoItem);


       if ( check == -1 || check == -2 )
       {
           if (check == -1)
           {
                HienThi(snake, hq, nd, item, listItems, chisoItem);
           }
           DrawString("GameOver", 15, 55, 7);

           while( _getch() != 13);
           break;




       }
       Sleep(snake.timesleep);
    }
}


void HuongDan()
{
    ifstream instream;
    instream.open("guide.txt", ios::in);
    while(!instream.eof())
    {
        string line;
        string str;
        getline(instream, line);

        cout<< line;
        cout<<endl;
        Sleep(1000);
    }
    instream.close();
}


void ThongTin()
{
    ifstream instream;
    instream.open("info.txt", ios::in);
    while(!instream.eof())
    {
        char c;
        instream >> c;
        if ( c == '#') c = ' ';
        cout<< c;
        if ( c == '.')
        {
            cout<<endl;
        }
        Sleep(50);
    }
    instream.close();
}

void Thoat()
{
    DrawString("ENTER TO EXIT", 10, 20, 7);
    exit(0);
}



int main()
{
     Menu part;
    vector<Menu> listMenu;
    int number=0;
    bool check;

    DrawTextSnake();
    Show_Menu(listMenu, part);
    while(1)
    {
        MoveSelect(listMenu,number);
        check = Checkselect();

        if ( check == true )
        {
            check = false;
            if ( number == 0)
            {
                system("cls");
                ChoiGame();
                DrawString("Do you want to play again? double  y/n", 16, 40, 14);
                while( !_kbhit());
                if ( _getch() == 110 )
                {
//                    system("cls");
//                    int number=0;
//                    bool check;
//                    DrawTextSnake();
//                    Show_Menu(listMenu, part);
//                    continue;
                      break;
                }
                else if ( _getch() == 121)
                {
                    system("cls");
                    int number=0;
                    bool check;
                    DrawTextSnake();
                    Show_Menu(listMenu, part);
                    continue;
                }

            }
            else if ( number == 1)
            {
                system("cls");
                Show_Menu(listMenu, part);
                HuongDan();

            }
            else if ( number == 2 )
            {
                system("cls");
                Show_Menu(listMenu, part);
                ThongTin();
            }
            else if ( number == 3)
            {
                system("cls");
                Show_Menu(listMenu, part);
                Thoat();
            }
        }

    }
    return 0;

}
