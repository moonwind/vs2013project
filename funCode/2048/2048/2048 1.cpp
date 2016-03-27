// 2048.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h> 
enum MoveDir{MoveUp = 0,MoveDown,MoveLeft,MoveRight,none};
int Game2048[4][4];
int *PGame2048 = (int *)Game2048;
void Game2048PutOut();
void ArrMemcpy(int * DesArray, int *SrcArray, int Row_Column, enum MoveDir Direction, int flag);
int CheckAdd(int *arr, int Row_Column, enum MoveDir Direction);// Row_Column;
int GameRun(enum MoveDir);
int SetNewNum();
int _tmain(int argc, _TCHAR* argv[])
{
	memset(Game2048,0, 16*sizeof(int));
	int _value;
	enum MoveDir direction = none;
	SetNewNum();
	SetNewNum();
	Game2048PutOut();
	while(1)
	{
		_value = GetAsyncKeyState(VK_LEFT);
		if (_value & 0x01)
			direction = MoveLeft;
		_value = GetAsyncKeyState(VK_RIGHT);
		if (_value & 0x01)
			direction = MoveRight;
		_value = GetAsyncKeyState(VK_UP);
		if (_value & 0x01)
			direction = MoveUp;
		_value = GetAsyncKeyState(VK_DOWN);
		if (_value & 0x01)
			direction = MoveDown;
		if (direction != none)
		{
			int ret = GameRun(direction);
			if (!ret)
			{
				printf("game over!!\n");
				break;
			}
			Game2048PutOut();
		}

		direction = none;
	}
	getchar();
	return 0;
}
int GameRun(enum MoveDir direction)
{
	int retValue=0;
	for (int index = 0; index < 4;index++)
		retValue+=CheckAdd(PGame2048, index, direction)<<index;
	retValue+=SetNewNum();
	return retValue;
}

int SetNewNum()
{
	int randNum = 0,index=0;
	int numNeedSet[16], indexNum = 0;;

	memset(numNeedSet,-1,16*sizeof(int));
	for (index = 0; index < 16; index++)
	{
		if (PGame2048[index] == 0)
			numNeedSet[indexNum++] = index;
	}
	if (indexNum == 0)
		return 0;//full
	randNum = rand() % indexNum;
	PGame2048[numNeedSet[randNum]] = 2;
	return 1;
}

void Game2048PutOut()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%-4d",Game2048[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int CheckAdd(int *arr, int Row_Column, enum MoveDir Direction)// Row_Column
{
	int DesArray[4], SrcArray[4] ,* PArr;
	int index = 0,retValue=0;
	memset(DesArray,0,4*sizeof(int));
	PArr = DesArray;
	ArrMemcpy(SrcArray, arr, Row_Column,Direction, 1);
	for (index = 0; index < 4; index++)
	{
		if (SrcArray[index] == 0)
			continue;
		if (*PArr == SrcArray[index])
		{
			*PArr += SrcArray[index];//合并
			PArr++;
			retValue = 1;
		}
		else
		{
			PArr += *PArr ? 1 : 0;
			*PArr = SrcArray[index];
		}
	}
	ArrMemcpy(DesArray, arr, Row_Column, Direction, 0);
	return retValue;
}
void ArrMemcpy(int * DesArray, int *SrcArray, int Row_Column, enum MoveDir Direction,int flag)
{
	int step = 0,index = 0;
	step = (Direction == MoveUp || Direction == MoveDown) ? 4 : 1;
	switch (Direction)
	{
	case MoveUp:
		SrcArray = SrcArray + Row_Column;
		break;
	case MoveLeft:
		SrcArray = SrcArray + Row_Column*4;
		break;
	case MoveDown:
		SrcArray = SrcArray + Row_Column  + 3 * step;
		step = 0 - step;
		break;
	case MoveRight:
		SrcArray = SrcArray + Row_Column*4+3*step;
		step = 0 - step;
		break;
	}
	if (flag)
	{
		for (index = 0; index < 4; index++)
		{
			*DesArray++ = *SrcArray;
			SrcArray += step;
		}
	}
	else
	{
		for (index = 0; index < 4; index++)
		{
			*SrcArray = *DesArray++;
			SrcArray += step;
		}
	}
}