#ifndef _select_h
#define _select_h

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

#include <linux/input.h>    
#include <stdlib.h>
#include <sys/timerfd.h>
#include "usart.h"


#define DEBUG1
#ifdef DEBUG1
#define dbg_mc(fmt, args...) printf(" DBG(%s, %s(), %d): " fmt, __FILE__, __FUNCTION__, __LINE__, ## args)
#else
#define dbg_mc(fmt, args...)
#endif
// 文件描述符集合
typedef struct{
   int nNum; // 文件描述数量
   int nFd[10]; // 文件描述符集合   
   char nFdString[10][30]; //设备类文件全名
} struFdDef;

typedef struct{
	
	struFdDef g_struDevFd; //设备符集合
	int nMaxFd;

}G_DATA;


#endif

