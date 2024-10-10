#include <stdio.h>
#include <memory.h>
#include <Windows.h>
//enemy 구조체를 바꾸는 함수들을 모음

#include <time.h>

#include "Console.h"
#include "Enemy.h"


//적 움직임 처리
void EnemyMove(int stageIndex, EnemyInfo* EnemyArr)
{
	for (int i = 0; i < BUFSIZE; i++)
	{
		if (EnemyArr[i].life)
		{
			EnemyArr[i].previousTime++;
			if (EnemyArr[i].previousTime>= 10/EnemyArr[i].speed* stageIndex)
			{
				EnemyArr[i].previousTime = 0;

				int index = EnemyArr[i].PartternIndex;
				char Move = EnemyArr[i].PartternArray[index];
				int MoveCount = EnemyArr[i].PartternArray[index + 1] - '0';
				if (Move == 'L')
				{
					for (int j = 0; j < MoveCount; j++)
					{
						EnemyArr[i].xpos--;
					}

				}
				else if (Move == 'R')
				{
					for (int j = 0; j < MoveCount; j++)
					{
						EnemyArr[i].xpos++;
					}
				}
				else if (Move == 'U')
				{
					for (int j = 0; j < MoveCount; j++)
					{
						EnemyArr[i].ypos--;
					}
				}
				else if (Move == 'D')
				{
					for (int j = 0; j < MoveCount; j++)
					{
						EnemyArr[i].ypos++;
					}
				}
				EnemyArr[i].PartternIndex += 2;
				if (EnemyArr[i].PartternIndex >= EnemyArr[i].ArraySize)
				{
					EnemyArr[i].PartternIndex = 0;
				}
			}

		}

	}

}

