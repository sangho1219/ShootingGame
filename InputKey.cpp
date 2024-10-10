#include <Windows.h>
#include "InputKey.h"
#include "Scene.h"
#include "Player.h"


void RepeatLoad_R(scene* currentScene)
{
	static bool check = true;
	if (GetAsyncKeyState(0x52))
	{
		if (check)
		{
			*currentScene = LOAD;
			check = false;
		}
	}
	else
	{
		check = true;
	}
}

void Load_Enter(scene* currentScene)
{
	static bool check = true;
	if (GetAsyncKeyState(VK_RETURN))
	{
		if (check)
		{
			*currentScene = LOAD;
			check = false;
		}
	}
	else
	{
		check = true;
	}
}


void GoTitle_Enter(scene* currentScene)
{
	static bool check = true;
	if (GetAsyncKeyState(VK_RETURN))
	{
		if (check)
		{
			*currentScene = TITLE;
			check = false;
		}
	}
	else
	{
		check = true;
	}
}

void End_Esc(scene* currentScene)
{
	static bool check = true;
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		if (check)
		{
			*currentScene = END;
			check = false;
		}
	}
	else
	{
		check = true;
	}
}

void UserLeft(int rangeX, int rangeY)
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		LeftMove(rangeX, rangeY);
	}
}

void UserRight(int rangeX, int rangeY)
{
	if (GetAsyncKeyState(VK_RIGHT))
	{
		RightMove(rangeX, rangeY);
	}
}

void UserUp(int rangeX, int rangeY)
{
	if (GetAsyncKeyState(VK_UP))
	{
		UpMove(rangeX, rangeY);
	}
}

void UserDown(int rangeX, int rangeY)
{
	if (GetAsyncKeyState(VK_DOWN))
	{
		DownMove(rangeX, rangeY);
	}
}



