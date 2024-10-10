#include "Load.h"
#include <stdio.h>
#include <memory.h>
#include <Windows.h>

bool strcmp_A(char* arg1, char* arg2)
{
	int len1 = strlen_null(arg1);
	int len2 = strlen_null(arg2);

	if (len1 - len2)
	{
		return false;
	}

	for (int i = 0; i < len1; i++)
	{
		if (arg1[i] - arg2[i])
		{
			return false;
		}
	}
	return true;
}


int TenSquare(int a)
{
	int ret = 1;
	for (int i = 0; i < a; i++)
	{
		ret = ret * 10;
	}
	return ret;
}


//문자열 길이
int strlen_null(const char* src)
{
	int srcSize = 0;

	while (*src != '\0')
	{
		srcSize++;
		src = src + 1;
	}

	return srcSize;
}
int strlen_space(const char* src)
{
	int srcSize = 0;

	while (*src != ' ')
	{
		srcSize++;
		src = src + 1;
	}

	return srcSize;
}




//문자열에서 개행 개수
int newLineAmount(const char* Buffer)
{
	int ListSize = 0;
	int BufferSize = strlen_null(Buffer);

	for (int i = 0; i < BufferSize; i++)
	{
		if (Buffer[i] == '\n')
		{
			ListSize++;
		}
	}

	return ListSize;
}
//문자열에서 공백 개수
int spaceAmount(const char* Buffer)
{
	int ListSize = 0;
	int BufferSize = strlen_null(Buffer);

	for (int i = 0; i < BufferSize; i++)
	{
		if (Buffer[i] == ' ')
		{
			ListSize++;
		}
	}

	return ListSize;
}

//숫자 문자열을 숫자로 전환해주어 반환하는 함수
int strChangeInt(char* str)
{
	int ret = 0;
	int index = strlen_null(str);
	char* temp = str;

	for (int j = index; j > 0; j--)
	{
		ret += (*temp - '0') * TenSquare(j - 1);
		temp = temp + 1;
	}

	return ret;
}





//개행을 기준으로 문자열 나누기
void newLineChangeNull(char** Buffer)
{
	char* Buffer_ = *Buffer;
	int BufferSize = strlen_null(Buffer_);

	for (int i = 0; i < BufferSize; i++)
	{
		if (Buffer_[i] == '\n')
		{
			Buffer_[i] = '\0';
		}
	}

	return;
}
//공백을 기준으로 문자열 나누기
void spaceChangeNull(char** Buffer)
{
	char* Buffer_ = *Buffer;
	int BufferSize = strlen_null(Buffer_);

	for (int i = 0; i < BufferSize; i++)
	{
		if (Buffer_[i] == ' ')
		{
			Buffer_[i] = '\0';
		}
	}

	return;
}


//원본 데이터 복사(동적 할당을 통해 같은 크기를 가지는 데이터를 만든다)
bool strcpy_malloc(const char* src, char** dest)
{
	int srcSize = 0;
	const char* temp = src;

	while (*src != '\0')
	{
		srcSize++;
		src = src + 1;
	}
	src = temp;

	char* a = (char*)malloc(srcSize+1);
	if (a == nullptr)
	{
		printf("할당 실패");
		return false;
	}
	char* temp2 = a;



	while (*src != '\0')
	{
		*a = *src;
		a = a + 1;
		src = src + 1;
	}
	*a = '\0';
	a = temp2;

	*dest = a;

	return true;
}

void strcpy_free(char* dest)
{
	free(dest);
}


//중간 중간 널 값이 있는 동적할당된 공간에서 널 값을 기준으로 문자열로 보고, 
//해당 문자열들의 주소 배열을 동적할당 해주는 함수
bool Init_nullList(char* Buffer, int size ,char*** List_arg)
{
	char** List = (char**)malloc(sizeof(char*) * size);
	if (List == NULL)
	{
		free(List);
		printf("List 할당 실패");
		return false;
	}

	size_t BufferSize = _msize(Buffer);

	int bufIndex = 0;
	List[0] = Buffer;
	int ListIndex = 1;
	while (1)
	{
		if (bufIndex >= BufferSize)
		{
			break;
		}
		if (ListIndex >= size)
		{
			break;
		}
		
		Buffer = Buffer + strlen_null(Buffer) + 1;


		List[ListIndex] = Buffer;
		ListIndex++;
		bufIndex++;
	}


	*List_arg = List;
	return true;
}

void Free_nullList(char** List_arg)
{
	free(List_arg);
}



bool Load_File(char** Buffer, const char* name)
{
	FILE* fp;

	fopen_s(&fp, name, "rt");
	if (fp == NULL)
	{
		printf("%s 파일 열기 실패", name);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);


	char* StageInfoBuffer = (char*)malloc(size+1);
	if (StageInfoBuffer == nullptr)
	{
		printf("%s 할당 실패", name);
		return false;
	}

	memset(StageInfoBuffer, 0, size+1);

	fread(StageInfoBuffer, sizeof(char), size+1, fp);
	
	StageInfoBuffer[size] = '\0';

	fclose(fp);

	*Buffer = StageInfoBuffer;

	return true;
}

void Free_File(char* Buffer)
{
	free(Buffer);
}


//공백이 포함된 문자열 배열 편집
//공백을 기준으로 문자열로 자르고, 해당 문자열들을 배열로 정리하기 위해
//배열을 동적할당 하는 구간
void InfoList(char** InfoList, int typeAmount, char**** List, char*** ListData)
{
	int infoAmount = spaceAmount(InfoList[0]) + 1;
	*List = (char***)malloc(sizeof(char**) * typeAmount);
	if (List == nullptr) {
		printf("List 할당 실패");
		return;
	}
	*ListData = (char**)malloc(sizeof(char*) * typeAmount * infoAmount);
	if (ListData == nullptr) {
		printf("ListData 할당 실패");
		return;
	}
	for (int i = 0; i < typeAmount; ++i)
	{
		(* List)[i] = (*ListData) + i * infoAmount;
	}
	//공백으로 구분되어 있는 데이터를 배열로 초기화
	for (int i = 0; i < typeAmount; ++i)
	{
		for (int j = 0; j < infoAmount; j++)
		{
			if (j == 0)
			{
				(*List)[i][j] = InfoList[i];
				continue;
			}

			(*List)[i][j] = (*List)[i][j - 1] + strlen_space((*List)[i][j - 1]) + 1;
		}
		spaceChangeNull((*List)[i]);
	}

}