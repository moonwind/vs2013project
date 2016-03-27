#include <stdio.h>
#include "Datatype.h"
#include "stdafx.h"
static int GetSync_byte_fun()
{
    printf("GetSync_byte_fun run\n");
	return 0;
}

static int GetPID_fun()
{
	printf("GetPID_fun run\n");
   return 0;
}

static int GetCounter_fun()
{
	printf("GetCounter_fun run\n");
   return 0;
}
TSheaderOpreator NewheaderOpreator()
{
	TSheaderOpreator tempOpreator;
    tempOpreator.GetCounter   = GetCounter_fun;
    tempOpreator.GetPID       = GetPID_fun;
	tempOpreator.GetSync_byte = GetSync_byte_fun;
	return tempOpreator;
}