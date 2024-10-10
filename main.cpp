#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <time.h>

#include "Console.h"
#include "Load.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Scene.h"
#include "InputKey.h"

void Buffer_Flip(void);
void Buffer_Clear(void);
void Sprite_Draw(int iX, int iY, char chSprite);


void UpdateTitle();
void UpdateLoad();
void UpdateGame();
void UpdateClear();
void UpdateGAMEOVER();
void UpdateEND();


char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];	//화면에 출력할 버퍼 생성
EnemyInfo EnemyArr[BUFSIZE];							//적 정보 구조체
Bullet BulletArr[BUFSIZE];								//총알 정보 구조체
Player player1;											//플레이어 정보에 대한 구조체 
EnemyInfo* EnemyCmp;									//적 정보들 비교 및 대입을 위한 구조체 생성
Bullet* BulletCmp;										//총알 정보들 비교 및 대입을 위한 구조체 생성

//랜더할 버퍼들
char* stageBuffer;
char* OverBuffer;
char* TitleBuffer;
char* Clear1Buffer;
char* Clear2Buffer;

//스테이지 정보 버퍼
char* StageInfo_src;
char* StageInfo_dest;
int StageTitleAmount;
char** StageTitleList;
//적 정보 버퍼
char* EnemyInfo_src;
char* EnemyInfo_dest;
int EnemyInfoAmount;
char** EnemyInfoList;
//총알 정보 버퍼
char* BulletInfo_src;
char* BulletInfo_dest;
int BulletInfoAmount;
char** BulletInfoList;
//패턴 정보 버퍼
char* PatternInfo_src;
char* PatternInfo_dest;
int PatternAmount;
char** PatternInfoList;
//패턴을 배열로 받기 위한 패턴 리스트
char** PatternList;
//적 정보를 배열로 받기 위한 리스트
char*** EnemyList;
char** EnemyData;
//총알 정보를 배열로 받기 위한 리스트
char*** BulletList;
char** BulletData;






scene currentScene = LOAD;
bool firstLoad = true;
bool end = false;
int stageIndex = 0;



int main(void)
{

	cs_Initial();
	while (!end)
	{
		switch (currentScene)
		{
		case TITLE:
			UpdateTitle();
			break;
		case LOAD:
			UpdateLoad();
			break;
		case GAME:
			UpdateGame();
			break;
		case CLEAR:
			UpdateClear();
			break;
		case GAMEOVER:
			UpdateGAMEOVER();
			break;
		case END:
			UpdateEND();
			break;
		}
		Sleep(50);
	}
}







void UpdateTitle()
{
	stageIndex = 0;

	Buffer_Clear();

	for (int i = 0; i < BUFSIZE; i++)
	{
		if (TitleBuffer[i] != '-')
		{
			Sprite_Draw(i % (dfSCREEN_WIDTH - 1), i / dfSCREEN_WIDTH, TitleBuffer[i]);
		}
	}

	Buffer_Flip();


	End_Esc(&currentScene);
	Load_Enter(&currentScene);
}
void UpdateLoad()
{
	//첫 로드시 초기화, 스테이지와 적에 대한 정보 초기화
	if (firstLoad)
	{
		//스테이지 리스트 로드 및 원본 보존을 위한 복사본 생성
		Load_File(&StageInfo_src, "StageInfo.txt");
		strcpy_malloc(StageInfo_src, &StageInfo_dest);
		//스테이지 리스트 초기화
		StageTitleAmount = newLineAmount(StageInfo_dest);
		newLineChangeNull(&StageInfo_dest);
		Init_nullList(StageInfo_dest, StageTitleAmount, &StageTitleList);

		//적 정보 로드 및 원본 보존을 위한 복사본 생성
		Load_File(&EnemyInfo_src, "EnemyInfo.txt");
		strcpy_malloc(EnemyInfo_src, &EnemyInfo_dest);
		//적 정보 초기화
		EnemyInfoAmount = newLineAmount(EnemyInfo_dest);
		newLineChangeNull(&EnemyInfo_dest);
		Init_nullList(EnemyInfo_dest, EnemyInfoAmount, &EnemyInfoList);

		//패턴 파일 로드
		Load_File(&PatternInfo_src, "PatternInfo.txt");
		strcpy_malloc(PatternInfo_src, &PatternInfo_dest);
		//패턴 파일 초기화
		PatternAmount = newLineAmount(PatternInfo_dest);
		newLineChangeNull(&PatternInfo_dest);
		Init_nullList(PatternInfo_dest, PatternAmount, &PatternInfoList);

		//총알 정보 로드 및 원본 보존을 위한 복사본 생성
		Load_File(&BulletInfo_src, "bullet.txt");
		strcpy_malloc(BulletInfo_src, &BulletInfo_dest);
		//총알 정보 초기화
		BulletInfoAmount = newLineAmount(BulletInfo_dest);
		newLineChangeNull(&BulletInfo_dest);
		Init_nullList(BulletInfo_dest, BulletInfoAmount, &BulletInfoList);



		//랜더 버퍼 로드
		Load_File(&OverBuffer, "over.txt");
		Load_File(&TitleBuffer, "title.txt");
		Load_File(&Clear1Buffer, "clear1.txt");
		Load_File(&Clear2Buffer, "clear2.txt");



		//패턴을 초기화할 패턴 리스트 할당
		PatternList = (char**)malloc(sizeof(char*) * PatternAmount);
		if (PatternList == nullptr)
		{
			free(PatternList);
			printf("PatternList 할당 실패");
			return;
		}
		//패턴 파일을 패턴 리스트에 초기화
		for (int i = 0; i < PatternAmount; i++)
		{
			Load_File(&PatternList[i], PatternInfoList[i]);
		}





		//EnemyInfoList에 세부 정보를 인덱스로 접근하기 위해 배열 동적할당 후 초기화
		InfoList(EnemyInfoList, EnemyInfoAmount, &EnemyList, &EnemyData);
		//EnemyList에 정보를 받아 놓을 구조체 초기화
		EnemyCmp = (EnemyInfo*)malloc(sizeof(EnemyInfo) * EnemyInfoAmount);
		if (EnemyCmp == nullptr) {
			printf("EnemyCmp 할당 실패");
			return;
		}
		for (int i = 0; i < EnemyInfoAmount; i++)
		{
			EnemyCmp[i].attributes = EnemyList[i][0][0];
			EnemyCmp[i].life = strChangeInt(EnemyList[i][1]);
			EnemyCmp[i].textFileName = EnemyList[i][2];
			EnemyCmp[i].speed = strChangeInt(EnemyList[i][3]);
			EnemyCmp[i].creatSpeed1 = strChangeInt(EnemyList[i][4]);
			EnemyCmp[i].bulletImage = EnemyList[i][5][0];
		}


		//BulletInfoList에 세부 정보를 인덱스로 접근하기 위해 배열 동적할당 후 초기화
		InfoList(BulletInfoList, BulletInfoAmount, &BulletList, &BulletData);
		//BulletList에 정보를 받아 놓을 구조체 초기화
		BulletCmp = (Bullet*)malloc(sizeof(Bullet) * BulletInfoAmount);
		if (EnemyCmp == nullptr) {
			printf("BulletCmp 할당 실패");
			return;
		}
		for (int i = 0; i < BulletInfoAmount; i++)
		{
			BulletCmp[i].valid = BulletList[i][0][0];
			BulletCmp[i].damage = strChangeInt(BulletList[i][1]);
			BulletCmp[i].speed = strChangeInt(BulletList[i][2]);
		}
		






		firstLoad = false;
		currentScene = TITLE;
	}
	else
	{
		//현재 스테이지 버퍼 불러오기
		Load_File(&stageBuffer, StageTitleList[stageIndex]);

		//적 상태와 총알 상태 0으로 초기화
		memset(EnemyArr, 0, BUFSIZE*sizeof(EnemyInfo));
		memset(BulletArr, 0, BUFSIZE * sizeof(Bullet));


		//플레이어 정보 초기화
		PlayerInit();

		//stage에 맞게 Enemy 초기화 (EnemyList, PatternList정보를 바탕으로)
		for (int i = 0; i < BUFSIZE; i++)
		{
			for (int j = 0; j < EnemyInfoAmount; j++)
			{
				if (stageBuffer[i] == EnemyCmp[j].attributes)
				{
					EnemyArr[i].life = EnemyCmp[j].life;
					EnemyArr[i].attributes = EnemyCmp[j].attributes;
					EnemyArr[i].xpos = i % (dfSCREEN_WIDTH - 1);
					EnemyArr[i].ypos = i / dfSCREEN_WIDTH;
					EnemyArr[i].speed = EnemyCmp[j].speed;
					EnemyArr[i].previousTime = 0;
					EnemyArr[i].creatSpeed1 = EnemyCmp[j].creatSpeed1;
					EnemyArr[i].bulletImage = EnemyCmp[j].bulletImage;
					

					for (int pIndex = 0; pIndex < PatternAmount; pIndex++)
					{
						if (strcmp_A(PatternInfoList[pIndex], EnemyCmp[j].textFileName))
						{
							EnemyArr[i].PartternArray = PatternList[pIndex];
							EnemyArr[i].ArraySize = (int)strlen(PatternList[pIndex]);
							EnemyArr[i].PartternIndex = 0;
						}
					}
				}
			}
		}

		Free_File(stageBuffer);


		currentScene = GAME;
	}
}
void UpdateGame()
{
	//유저의 움직임 처리
	UserDown(dfSCREEN_WIDTH, dfSCREEN_HEIGHT);
	UserUp(dfSCREEN_WIDTH, dfSCREEN_HEIGHT);
	UserLeft(dfSCREEN_WIDTH, dfSCREEN_HEIGHT);
	UserRight(dfSCREEN_WIDTH, dfSCREEN_HEIGHT);


	//적 움직임 처리
	EnemyMove(stageIndex, EnemyArr);

	//총알 생성 처리
	BulletCreate(stageIndex, EnemyArr, BulletArr, &player1);
	//총알 충돌 처리
	BulletCollision(stageIndex, EnemyArr, BulletArr, &player1, BulletCmp, BulletInfoAmount);
	//총알 움직임 처리
	BulletMove(dfSCREEN_WIDTH, stageIndex, EnemyArr, BulletArr, BulletCmp, BulletInfoAmount);



	//랜더부
	Buffer_Clear();

	Sprite_Draw(player1.xpos, player1.ypos, player1.image);
	for (int i = 0; i < BUFSIZE; i++)
	{
		if (EnemyArr[i].life)
		{
			Sprite_Draw(EnemyArr[i].xpos, EnemyArr[i].ypos, EnemyArr[i].attributes);
		}
	}
	for (int i = 0; i < BUFSIZE; i++)
	{
		if (BulletArr[i].valid)
		{
			Sprite_Draw(BulletArr[i].xpos, BulletArr[i].ypos, BulletArr[i].valid);
		}
	}

	Buffer_Flip();

	



	//스테이지 클리어 여부 확인, 클리어시 currentScene전환
	int check = 0;
	for (int i = 0; i < BUFSIZE; i++)
	{
		if (EnemyArr[i].life)
		{
			check++;
		}
	}
	if (check == 0)
	{
		currentScene = CLEAR;
		stageIndex++;
	}

	//게임 오버 여부 확인
	if (player1.hp == 0)
	{
		currentScene = GAMEOVER;
	}

	//esc키 눌릴시 타이틀 화면으로
	End_Esc(&currentScene);
	//R키 눌릴시 스테이지 재시작
	RepeatLoad_R(&currentScene);



}
void UpdateClear()
{
	//스테이지 올 클리어 여부
	if (stageIndex == StageTitleAmount)
	{
		//엔터키 시 타이틀로 씬 전환
		GoTitle_Enter(&currentScene);

		//랜더
		Buffer_Clear();
		for (int i = 0; i < BUFSIZE; i++)
		{
			if (Clear2Buffer[i] != '-')
			{
				Sprite_Draw(i % (dfSCREEN_WIDTH), i / dfSCREEN_WIDTH, Clear2Buffer[i]);
			}
		}
		Buffer_Flip();
	}
	else
	{
		//엔터 키시 로드로 씬 전환
		Load_Enter(&currentScene);

		//랜더
		Buffer_Clear();
		for (int i = 0; i < BUFSIZE; i++)
		{
			if (Clear1Buffer[i] != '-')
			{
				Sprite_Draw(i % (dfSCREEN_WIDTH), i / dfSCREEN_WIDTH, Clear1Buffer[i]);
			}
		}
		Buffer_Flip();
	}

	//엔드로 씬 전환
	End_Esc(&currentScene);
}
void UpdateGAMEOVER()
{
	//게임 오버시 조작키 입력
	RepeatLoad_R(&currentScene);
	GoTitle_Enter(&currentScene);
	End_Esc(&currentScene);

	//OverBuffer 랜더
	Buffer_Clear();
	for (int i = 0; i < BUFSIZE; i++)
	{
		if (OverBuffer[i] != '-')
		{
			Sprite_Draw(i % (dfSCREEN_WIDTH ), i / dfSCREEN_WIDTH, OverBuffer[i]);
		}
	}
	Buffer_Flip();
}
void UpdateEND()
{
	if (!firstLoad)
	{
		Free_File(StageInfo_src);
		strcpy_free(StageInfo_dest);
		Free_nullList(StageTitleList);
		Free_File(EnemyInfo_src);
		strcpy_free(EnemyInfo_dest);
		Free_nullList(EnemyInfoList);
		Free_File(PatternInfo_src);
		strcpy_free(PatternInfo_dest);
		Free_nullList(PatternInfoList);
		Free_File(OverBuffer);
		Free_File(TitleBuffer);
		Free_File(Clear1Buffer);
		Free_File(Clear2Buffer);
		for (int i = 0; i < PatternAmount; i++)
		{
			Free_File(PatternList[i]);
		}
		free(PatternList);
		free(EnemyData);
		free(EnemyList);

		end = true;
	}
	end = true;
}







//--------------------------------------------------------------------
// 버퍼의 내용을 화면으로 찍어주는 함수.
//
// 적군,아군,총알 등을 szScreenBuffer 에 넣어주고, 
// 1 프레임이 끝나는 마지막에 본 함수를 호출하여 버퍼 -> 화면 으로 그린다.
//--------------------------------------------------------------------
void Buffer_Flip(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		cs_MoveCursor(0, iCnt);
		printf(szScreenBuffer[iCnt]);
	}
}


//--------------------------------------------------------------------
// 화면 버퍼를 지워주는 함수
//
// 매 프레임 그림을 그리기 직전에 버퍼를 지워 준다. 
// 안그러면 이전 프레임의 잔상이 남으니까
//--------------------------------------------------------------------
void Buffer_Clear(void)
{
	int iCnt;
	memset(szScreenBuffer, ' ', dfSCREEN_WIDTH * dfSCREEN_HEIGHT);

	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = '\0';
	}

}

//--------------------------------------------------------------------
// 버퍼의 특정 위치에 원하는 문자를 출력.
//
// 입력 받은 X,Y 좌표에 아스키코드 하나를 출력한다. (버퍼에 그림)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	szScreenBuffer[iY][iX] = chSprite;
}