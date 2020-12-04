
#include "server.h"

//sockaddr_un

int main(void)
{
	int sockfd;
	struct sockaddr_un seraddr;
	int ret = -1;
	
	// 第1步：先socket打开文件描述符
	sockfd = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket");
		return -1;
	}
	printf("socketfd = %d.\n", sockfd);
	// 第2步：connect链接服务器
	seraddr.sun_family = AF_LOCAL;
	strcpy(seraddr.sun_path, "/tmp/test1.socket");
	ret = connect(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	if (ret < 0) 
	{
		perror("listen");
		return -1;
	}
	printf("成功建立连接\n");


	while(1)
	{
		// 回合中第1步：客户端给服务器发送信息
		info st1;
		printf("请输入学生姓名\n");
		scanf("%s", st1.name);
		printf("请输入学生年龄");
		scanf("%d", &st1.age);
		st1.cmd = CMD_REGISTER;
		//printf("刚才输入的是：%s\n", sendbuf);
		ret = send(sockfd, &st1, sizeof(info), 0);
		printf("发送了1个学生信息\n");
		
		// 回合中第2步：客户端接收服务器的回复
		memset(&st1, 0, sizeof(st1));
		ret = recv(sockfd, &st1, sizeof(st1), 0);
		
		// 回合中第3步：客户端解析服务器的回复，再做下一步定夺
		if (st1.stat == STAT_OK)
		{
			printf("注册学生信息成功\n");
		}
		else
		{
			printf("注册学生信息失败\n");
		}

	}
}


/*	
//sockaddr_in
char sendbuf[100];
char recvbuf[100];

int main(void)
{
	// 第1步：先socket打开文件描述符
	int sockfd = -1, ret = -1;
	struct sockaddr_in seraddr = {0};
	struct sockaddr_in cliaddr = {0};
	
	// 第1步：socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket");
		return -1;
	}
	printf("socketfd = %d.\n", sockfd);
	
	// 第2步：connect链接服务器
	seraddr.sin_family = AF_INET;		// 设置地址族为IPv4
	seraddr.sin_port = htons(SERPORT);	// 设置地址的端口号信息
	seraddr.sin_addr.s_addr = inet_addr(SERADDR);	//　设置IP地址
	ret = connect(sockfd, (const struct sockaddr *)&seraddr, sizeof(seraddr));
	if (ret < 0)
	{
		perror("listen");
		return -1;
	}
	printf("成功建立连接\n");

	while (1)
	{
		// 回合中第1步：客户端给服务器发送信息
		info st1;
		printf("请输入学生姓名\n");
		scanf("%s", st1.name);
		printf("请输入学生年龄");
		scanf("%d", &st1.age);
		st1.cmd = CMD_REGISTER;
		//printf("刚才输入的是：%s\n", sendbuf);
		ret = send(sockfd, &st1, sizeof(info), 0);
		printf("发送了1个学生信息\n");
		
		// 回合中第2步：客户端接收服务器的回复
		memset(&st1, 0, sizeof(st1));
		ret = recv(sockfd, &st1, sizeof(st1), 0);
		
		// 回合中第3步：客户端解析服务器的回复，再做下一步定夺
		if (st1.stat == STAT_OK)
		{
			printf("注册学生信息成功\n");
		}
		else
		{
			printf("注册学生信息失败\n");
		}

	}



	return 0;
}*/	





