#include<stdio.h>
#include<stdlib.h>
#include "stdafx.h"
//任意数据块交换
int Swap(void *arg1, void *arg2, int swapMemSize)
{
	unsigned char *temp = NULL;
	unsigned char *p1, *p2;
	int index = 0;
	p1 = (unsigned char *)arg1;
	p2 = (unsigned char *)arg2;
	temp = (unsigned char*)malloc(swapMemSize);
	if (temp == NULL)
	{
		return 1;
	}
	for (index = 0; index < swapMemSize; index++)
	{
		temp[index] = p1[index];
	}
	for (index = 0; index < swapMemSize; index++)
	{
		p1[index] = p2[index];
	}
	for (index = 0; index < swapMemSize; index++)
	{
		p2[index] = temp[index];
	}
	free(temp);
	return 0;
}
