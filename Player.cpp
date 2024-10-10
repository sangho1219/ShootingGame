//플레이어 구조체를 바꾸는 함수들을 모음
#include "Player.h"
#include "time.h"
#include <Windows.h>


extern Player player1;

//플레이어 초기화
void PlayerInit()
{
	player1.xpos = 40;
	player1.ypos = 19;
	player1.hp = 10;
	player1.image = 'P';
}


void LeftMove(int rangeX, int rangeY)
{
	if (player1.xpos - 1 >= 0)
	{
		player1.xpos--;
	}
}
void RightMove(int rangeX, int rangeY)
{
	if (player1.xpos + 1 < rangeX - 1)
	{
		player1.xpos++;
	}
}

void UpMove(int rangeX, int rangeY)
{
	if (player1.ypos - 1 >= 0)
	{
		player1.ypos--;
	}
}

void DownMove(int rangeX, int rangeY)
{
	if (player1.ypos + 1 < rangeY)
	{
		player1.ypos++;
	}
}


