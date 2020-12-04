
#include "server.h"

int main(void)
{
	int sockfd,clifd = -1;
	socklen_t len = 0;
	struct sockaddr_un seraddr = {0};	
	struct sockaddr_un cliaddr = {0};
	
	int ret = -1;
	
	// 第1步：先socket打开文件描述符
	sockfd = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket");
		return -1;
	}
	printf("socketfd = %d.\n", sockfd);
	
	// 第2步：bind绑定sockefd和当前电脑的ip地址&端口号
	seraddr.sun_family = AF_LOCAL;
	strcpy(seraddr.sun_path, "/tmp/test1.socket");
	ret = bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	if (ret < 0)
	{
		perror("bind");
		return -1;
	}
	printf("bind success.\n");

	// 第三步：listen监听端口
	ret = listen(sockfd, BACKLOG);		// 阻塞等待客户端来连接服务器
	if (ret < 0)
	{
		perror("listen");
		return -1;
	}
	
	// 第四步：accept阻塞等待客户端接入
	clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
	printf("连接已经建立，client fd = %d.\n", clifd);
	

	while(1)
	{
		info st;
		// 回合中第1步：服务器收
		ret = recv(clifd, &st, sizeof(info), 0);

		// 回合中第2步：服务器解析客户端数据包，然后干活，
		if (st.cmd == CMD_REGISTER)  //clietn强制退出的时候，这里多执行了2次
		{
			printf("用户要注册学生信息\n");
			printf("学生姓名：%s，学生年龄：%d\n", st.name, st.age);
			// 在这里服务器要进行真正的注册动作，一般是插入数据库一条信息
			
			// 回合中第3步：回复客户端
			st.stat = STAT_OK;
			ret = send(clifd, &st, sizeof(info), 0);
		}
		
		if (st.cmd == CMD_CHECK)
		{
			
		}
		
		if (st.cmd == CMD_GETINFO)
		{
			
		}

	}
}


/*	
//sockaddr_in
char recvbuf[100];

int main(void)
{
	// 第1步：先socket打开文件描述符
	int sockfd = -1, ret = -1, clifd = -1;
	socklen_t len = 0;
	struct sockaddr_in seraddr = {0};
	struct sockaddr_in cliaddr = {0};
	
	char ipbuf[30] = {0};
	
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket");
		return -1;
	}
	printf("socketfd = %d.\n", sockfd);
	
	// 第2步：bind绑定sockefd和当前电脑的ip地址&端口号
	seraddr.sin_family = AF_INET;		// 设置地址族为IPv4
	seraddr.sin_port = htons(SERPORT);	// 设置地址的端口号信息
	seraddr.sin_addr.s_addr = inet_addr(SERADDR);	//　设置IP地址
	ret = bind(sockfd, (const struct sockaddr *)&seraddr, sizeof(seraddr));
	if (ret < 0)
	{
		perror("bind");
		return -1;
	}
	printf("bind success.\n");
	
	// 第三步：listen监听端口
	ret = listen(sockfd, BACKLOG);		// 阻塞等待客户端来连接服务器
	if (ret < 0)
	{
		perror("listen");
		return -1;
	}
	
	// 第四步：accept阻塞等待客户端接入
	clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
	printf("连接已经建立，client fd = %d.\n", clifd);
	


	// 客户端反复给服务器发
	while (1)
	{
		info st;
		// 回合中第1步：服务器收
		ret = recv(clifd, &st, sizeof(info), 0);

		// 回合中第2步：服务器解析客户端数据包，然后干活，
		if (st.cmd == CMD_REGISTER)  //clietn强制退出的时候，这里多执行了2次
		{
			printf("用户要注册学生信息\n");
			printf("学生姓名：%s，学生年龄：%d\n", st.name, st.age);
			// 在这里服务器要进行真正的注册动作，一般是插入数据库一条信息
			
			// 回合中第3步：回复客户端
			st.stat = STAT_OK;
			ret = send(clifd, &st, sizeof(info), 0);
		}
		
		if (st.cmd == CMD_CHECK)
		{
			
		}
		
		if (st.cmd == CMD_GETINFO)
		{
			
		}

	}
	return 0;
}*/	


