//Enemy구조체를 바꾸는 함수 모음
#include<Windows.h>
#pragma once


struct EnemyInfo
{
	char attributes;

	int xpos;
	int ypos;

	int life;

	char* textFileName;

	int PartternIndex;
	char* PartternArray;
	int ArraySize;

	int speed;
	long previousTime;

	char bulletImage;
	long bulletSpeed1;
	long bulletSpeed2;
	long creatSpeed1;
	long creatSpeed2;
};


void EnemyMove(int stageIndex, EnemyInfo* EnemyArr);
