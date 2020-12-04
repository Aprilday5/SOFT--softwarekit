#ifndef __SERVER__H
#define __SERVER__H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/un.h>

#define SERPORT		9003
#define SERADDR		"192.168.198.200"		// ifconfig看到的
#define BACKLOG		100

#define CMD_REGISTER	1001	// 注册学生信息
#define CMD_CHECK		1002	// 检验学生信息
#define CMD_GETINFO		1003	// 获取学生信息

#define STAT_OK			30		// 回复ok
#define STAT_ERR		31		// 回复出错了

typedef struct commu
{
	char name[20];		// 学生姓名
	int age;			// 学生年龄
	int cmd;			// 命令码
	int stat;			// 状态信息，用来回复
}info;

#endif

