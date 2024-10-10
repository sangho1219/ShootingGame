#include "Bullet.h"
#include "Player.h"
#include "Console.h"
#include "Enemy.h"
#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <time.h>


//醚舅 框流烙 贸府
void BulletMove(int range, int stageIndex, EnemyInfo* EnemyArr, Bullet* BulletArr, Bullet* BulletCmp, int cmpAmount)
{
		for (int i = 0; i < BUFSIZE; i++)
		{
			if (BulletArr[i].valid == '!')
			{
				BulletArr[i].ypos = BulletArr[i].ypos - 1;
				if (BulletArr[i].ypos < 0)
				{
					BulletArr[i].valid = 0;
					continue;
				}
			}
			for (int j = 0; j < cmpAmount; j++)
			{
				if (BulletArr[i].valid == (BulletCmp+j)->valid)
				{
					BulletArr[i].count++;
					if (BulletArr[i].count > (BulletCmp + j)->speed / (stageIndex + 1))
					{
						BulletArr[i].count = 0;
						BulletArr[i].ypos = BulletArr[i].ypos + 1;
						if (BulletArr[i].ypos > range + 1)
						{
							BulletArr[i].valid = 0;
							continue;
						}
					}
				}
			}
		}
}
//醚舅 面倒 贸府
void BulletCollision(int stageIndex, EnemyInfo* EnemyArr, Bullet* BulletArr, Player *player1, Bullet* BulletCmp, int cmpAmount)
{
	for (int i = 0; i < BUFSIZE; i++)
	{
		if (BulletArr[i].valid == '!')
		{
			for (int j = 0; j < BUFSIZE; j++)
			{
				if (EnemyArr[j].life > 0)
				{
					if (BulletArr[i].xpos == EnemyArr[j].xpos && BulletArr[i].ypos == EnemyArr[j].ypos)
					{
						BulletArr[i].valid = 0;
						EnemyArr[j].life = EnemyArr[j].life - 1;
					}
				}
			}
		}
		for (int j = 0; j < cmpAmount; j++)
		{
			if (BulletArr[i].valid == (BulletCmp+j)->valid)
			{
				if (BulletArr[i].xpos == (*player1).xpos && BulletArr[i].ypos == (*player1).ypos)
				{
					BulletArr[i].valid = 0;
					(*player1).hp = (*player1).hp - (BulletCmp + j)->damage;
				}
			}
		}
	}
}
//醚舅 积己
void BulletCreate(int stageIndex, EnemyInfo* EnemyArr, Bullet* BulletArr, Player *player1)
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		for (int i = 0; i < BUFSIZE; i++)
		{
			if (BulletArr[i].valid == 0)
			{
				BulletArr[i].valid = '!';
				BulletArr[i].xpos = (*player1).xpos;
				BulletArr[i].ypos = (*player1).ypos;
				break;
			}
		}
	}
	for (int i = 0; i < BUFSIZE; i++)
	{
		EnemyArr[i].creatSpeed2++;
		if (EnemyArr[i].life > 0)
		{
			if (EnemyArr[i].creatSpeed2 >= EnemyArr[i].creatSpeed1 * 6/ (stageIndex+1))
			{
				EnemyArr[i].creatSpeed2 = 0;
				for (int j = 0; j < BUFSIZE; j++)
				{
					if (BulletArr[j].valid == 0)
					{
						BulletArr[j].valid = EnemyArr[i].bulletImage;
						BulletArr[j].xpos = EnemyArr[i].xpos;
						BulletArr[j].ypos = EnemyArr[i].ypos;
						break;
					}
				}
			}
		}
	}
}