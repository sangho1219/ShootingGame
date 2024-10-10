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


char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];	//ȭ�鿡 ����� ���� ����
EnemyInfo EnemyArr[BUFSIZE];							//�� ���� ����ü
Bullet BulletArr[BUFSIZE];								//�Ѿ� ���� ����ü
Player player1;											//�÷��̾� ������ ���� ����ü 
EnemyInfo* EnemyCmp;									//�� ������ �� �� ������ ���� ����ü ����
Bullet* BulletCmp;										//�Ѿ� ������ �� �� ������ ���� ����ü ����

//������ ���۵�
char* stageBuffer;
char* OverBuffer;
char* TitleBuffer;
char* Clear1Buffer;
char* Clear2Buffer;

//�������� ���� ����
char* StageInfo_src;
char* StageInfo_dest;
int StageTitleAmount;
char** StageTitleList;
//�� ���� ����
char* EnemyInfo_src;
char* EnemyInfo_dest;
int EnemyInfoAmount;
char** EnemyInfoList;
//�Ѿ� ���� ����
char* BulletInfo_src;
char* BulletInfo_dest;
int BulletInfoAmount;
char** BulletInfoList;
//���� ���� ����
char* PatternInfo_src;
char* PatternInfo_dest;
int PatternAmount;
char** PatternInfoList;
//������ �迭�� �ޱ� ���� ���� ����Ʈ
char** PatternList;
//�� ������ �迭�� �ޱ� ���� ����Ʈ
char*** EnemyList;
char** EnemyData;
//�Ѿ� ������ �迭�� �ޱ� ���� ����Ʈ
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
	//ù �ε�� �ʱ�ȭ, ���������� ���� ���� ���� �ʱ�ȭ
	if (firstLoad)
	{
		//�������� ����Ʈ �ε� �� ���� ������ ���� ���纻 ����
		Load_File(&StageInfo_src, "StageInfo.txt");
		strcpy_malloc(StageInfo_src, &StageInfo_dest);
		//�������� ����Ʈ �ʱ�ȭ
		StageTitleAmount = newLineAmount(StageInfo_dest);
		newLineChangeNull(&StageInfo_dest);
		Init_nullList(StageInfo_dest, StageTitleAmount, &StageTitleList);

		//�� ���� �ε� �� ���� ������ ���� ���纻 ����
		Load_File(&EnemyInfo_src, "EnemyInfo.txt");
		strcpy_malloc(EnemyInfo_src, &EnemyInfo_dest);
		//�� ���� �ʱ�ȭ
		EnemyInfoAmount = newLineAmount(EnemyInfo_dest);
		newLineChangeNull(&EnemyInfo_dest);
		Init_nullList(EnemyInfo_dest, EnemyInfoAmount, &EnemyInfoList);

		//���� ���� �ε�
		Load_File(&PatternInfo_src, "PatternInfo.txt");
		strcpy_malloc(PatternInfo_src, &PatternInfo_dest);
		//���� ���� �ʱ�ȭ
		PatternAmount = newLineAmount(PatternInfo_dest);
		newLineChangeNull(&PatternInfo_dest);
		Init_nullList(PatternInfo_dest, PatternAmount, &PatternInfoList);

		//�Ѿ� ���� �ε� �� ���� ������ ���� ���纻 ����
		Load_File(&BulletInfo_src, "bullet.txt");
		strcpy_malloc(BulletInfo_src, &BulletInfo_dest);
		//�Ѿ� ���� �ʱ�ȭ
		BulletInfoAmount = newLineAmount(BulletInfo_dest);
		newLineChangeNull(&BulletInfo_dest);
		Init_nullList(BulletInfo_dest, BulletInfoAmount, &BulletInfoList);



		//���� ���� �ε�
		Load_File(&OverBuffer, "over.txt");
		Load_File(&TitleBuffer, "title.txt");
		Load_File(&Clear1Buffer, "clear1.txt");
		Load_File(&Clear2Buffer, "clear2.txt");



		//������ �ʱ�ȭ�� ���� ����Ʈ �Ҵ�
		PatternList = (char**)malloc(sizeof(char*) * PatternAmount);
		if (PatternList == nullptr)
		{
			free(PatternList);
			printf("PatternList �Ҵ� ����");
			return;
		}
		//���� ������ ���� ����Ʈ�� �ʱ�ȭ
		for (int i = 0; i < PatternAmount; i++)
		{
			Load_File(&PatternList[i], PatternInfoList[i]);
		}





		//EnemyInfoList�� ���� ������ �ε����� �����ϱ� ���� �迭 �����Ҵ� �� �ʱ�ȭ
		InfoList(EnemyInfoList, EnemyInfoAmount, &EnemyList, &EnemyData);
		//EnemyList�� ������ �޾� ���� ����ü �ʱ�ȭ
		EnemyCmp = (EnemyInfo*)malloc(sizeof(EnemyInfo) * EnemyInfoAmount);
		if (EnemyCmp == nullptr) {
			printf("EnemyCmp �Ҵ� ����");
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


		//BulletInfoList�� ���� ������ �ε����� �����ϱ� ���� �迭 �����Ҵ� �� �ʱ�ȭ
		InfoList(BulletInfoList, BulletInfoAmount, &BulletList, &BulletData);
		//BulletList�� ������ �޾� ���� ����ü �ʱ�ȭ
		BulletCmp = (Bullet*)malloc(sizeof(Bullet) * BulletInfoAmount);
		if (EnemyCmp == nullptr) {
			printf("BulletCmp �Ҵ� ����");
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
		//���� �������� ���� �ҷ�����
		Load_File(&stageBuffer, StageTitleList[stageIndex]);

		//�� ���¿� �Ѿ� ���� 0���� �ʱ�ȭ
		memset(EnemyArr, 0, BUFSIZE*sizeof(EnemyInfo));
		memset(BulletArr, 0, BUFSIZE * sizeof(Bullet));


		//�÷��̾� ���� �ʱ�ȭ
		PlayerInit();

		//stage�� �°� Enemy �ʱ�ȭ (EnemyList, PatternList������ ��������)
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
	//������ ������ ó��
	UserDown(dfSCREEN_WIDTH, dfSCREEN_HEIGHT);
	UserUp(dfSCREEN_WIDTH, dfSCREEN_HEIGHT);
	UserLeft(dfSCREEN_WIDTH, dfSCREEN_HEIGHT);
	UserRight(dfSCREEN_WIDTH, dfSCREEN_HEIGHT);


	//�� ������ ó��
	EnemyMove(stageIndex, EnemyArr);

	//�Ѿ� ���� ó��
	BulletCreate(stageIndex, EnemyArr, BulletArr, &player1);
	//�Ѿ� �浹 ó��
	BulletCollision(stageIndex, EnemyArr, BulletArr, &player1, BulletCmp, BulletInfoAmount);
	//�Ѿ� ������ ó��
	BulletMove(dfSCREEN_WIDTH, stageIndex, EnemyArr, BulletArr, BulletCmp, BulletInfoAmount);



	//������
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

	



	//�������� Ŭ���� ���� Ȯ��, Ŭ����� currentScene��ȯ
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

	//���� ���� ���� Ȯ��
	if (player1.hp == 0)
	{
		currentScene = GAMEOVER;
	}

	//escŰ ������ Ÿ��Ʋ ȭ������
	End_Esc(&currentScene);
	//RŰ ������ �������� �����
	RepeatLoad_R(&currentScene);



}
void UpdateClear()
{
	//�������� �� Ŭ���� ����
	if (stageIndex == StageTitleAmount)
	{
		//����Ű �� Ÿ��Ʋ�� �� ��ȯ
		GoTitle_Enter(&currentScene);

		//����
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
		//���� Ű�� �ε�� �� ��ȯ
		Load_Enter(&currentScene);

		//����
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

	//����� �� ��ȯ
	End_Esc(&currentScene);
}
void UpdateGAMEOVER()
{
	//���� ������ ����Ű �Է�
	RepeatLoad_R(&currentScene);
	GoTitle_Enter(&currentScene);
	End_Esc(&currentScene);

	//OverBuffer ����
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
// ������ ������ ȭ������ ����ִ� �Լ�.
//
// ����,�Ʊ�,�Ѿ� ���� szScreenBuffer �� �־��ְ�, 
// 1 �������� ������ �������� �� �Լ��� ȣ���Ͽ� ���� -> ȭ�� ���� �׸���.
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
// ȭ�� ���۸� �����ִ� �Լ�
//
// �� ������ �׸��� �׸��� ������ ���۸� ���� �ش�. 
// �ȱ׷��� ���� �������� �ܻ��� �����ϱ�
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
// ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���.
//
// �Է� ���� X,Y ��ǥ�� �ƽ�Ű�ڵ� �ϳ��� ����Ѵ�. (���ۿ� �׸�)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	szScreenBuffer[iY][iX] = chSprite;
}