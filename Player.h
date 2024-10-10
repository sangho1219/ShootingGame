#pragma once
struct Player
{
	int xpos;
	int ypos;
	int hp;
	char image;
};
void PlayerInit();
void LeftMove(int rangeX, int rangeY);
void RightMove(int rangeX, int rangeY);
void UpMove(int rangeX, int rangeY);
void DownMove(int rangeX, int rangeY);


void UserLeft(int rangeX, int rangeY);
void UserRight(int rangeX, int rangeY);
void UserUp(int rangeX, int rangeY);
void UserDown(int rangeX, int rangeY);
