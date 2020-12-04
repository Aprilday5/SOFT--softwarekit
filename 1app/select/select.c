
//文档说明：使用select监测多个设备文件，比如串口，input按键，。。-----timerfd
//将它们集合到


#include "select.h"


//获取input设备符号
void getInputEvent(const char *driname_,char * devname_)
{
    char          name[64];           /* RATS: Use ok, but could be better */
    char          buf[256] = { 0, };  /* RATS: Use ok */
    int           fd = 0;
    int           i;
	
    for (i = 0; i < 32; i++)  
    {
        sprintf(name, "/dev/input/event%d", i);
        if ((fd = open(name, O_RDONLY, 0)) >= 0)
        {
            ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
            if(strstr(buf, driname_))
            {
                close(fd);
				sprintf(devname_,"/dev/input/event%d",i);
				dbg_mc("path:%s\n",devname_);
				break ;
            }
            close(fd);
        }
    }
}

//创建1s的定时器
int sys_create_timerfd(int tv_sec,int tv_nsec)
{
    struct itimerspec new_value;
    int timerfd;


    /*创建定时噿*/
    timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timerfd == -1)
    {
        return -1;
    }   

    /*设置定时器的起始时刻*/
    new_value.it_value.tv_sec  = tv_sec;
    new_value.it_value.tv_nsec = tv_nsec;
    /*设置定时器的间隔*/
    new_value.it_interval.tv_sec  = tv_sec;
    new_value.it_interval.tv_nsec = tv_nsec; 
   
    /*启动定时噿*/
    if (timerfd_settime(timerfd, 0, &new_value, NULL))
    {
        close(timerfd);
        return -1;
    }
    
    return timerfd;
}
int TimerTaskInit(int sec_)
{

	int timerfd = -1;
	timerfd = sys_create_timerfd(sec_,0);
	if (timerfd < 0)
	{
		printf("timer init err: 1s\n");
	}
	else
	{
		//printf("time_fd=%d\n",gp->timerfd_1s);
	}
	return timerfd;
}
void writeToFile(int value_,char *filepath_)
{
	char _cmdbuf[30]={0};
	sprintf(_cmdbuf,"echo %d > %s",value_,filepath_);
	printf("cmd:%s\n",_cmdbuf);
	system(_cmdbuf);		
}


//fds初始化
void initfd(G_DATA *dp)
{
	int ret = -1;
	int i = 0;
	
	dp->g_struDevFd.nNum = 0;	//约定若干fd在数组中的排序
	strcpy(dp->g_struDevFd.nFdString[dp->g_struDevFd.nNum],"timerfd1");
	dp->g_struDevFd.nFd[dp->g_struDevFd.nNum] = TimerTaskInit(1);  
	dp->g_struDevFd.nNum++;

	strcpy(dp->g_struDevFd.nFdString[dp->g_struDevFd.nNum],"/dev/ttyRS485_1");
	dp->g_struDevFd.nFd[dp->g_struDevFd.nNum] = UART0_Open(dp->g_struDevFd.nFd[dp->g_struDevFd.nNum],dp->g_struDevFd.nFdString[dp->g_struDevFd.nNum]);//串口初始化：打开串口，设置波特率
	do  
	{	ret = UART0_Init(dp->g_struDevFd.nFd[dp->g_struDevFd.nNum],9600,0,8,1,'N');  
		sleep(1);
	}while(FALSE == ret || FALSE == dp->g_struDevFd.nFd[dp->g_struDevFd.nNum]); 
	dp->g_struDevFd.nNum++;
	
	getInputEvent("gpiokey",dp->g_struDevFd.nFdString[dp->g_struDevFd.nNum]); //input按键
	dp->g_struDevFd.nFd[dp->g_struDevFd.nNum] = open(dp->g_struDevFd.nFdString[dp->g_struDevFd.nNum], O_RDONLY);//按键初始化
	if (dp->g_struDevFd.nFd[dp->g_struDevFd.nNum] < 0)
	{
		perror("open:");
		return ;
	}
	dp->g_struDevFd.nNum++;

	   // 获得最大串口句柄
   for (i = 0; i < dp->g_struDevFd.nNum; i++)
   {
      if (dp->g_struDevFd.nFd[i] > dp->nMaxFd)
      {
         dp->nMaxFd = dp->g_struDevFd.nFd[i];
      }
   }
   dp->nMaxFd += 1;
}

void selectProcess(G_DATA *dp)
{

	int ret = -1;
	fd_set fds;
	int i;

	unsigned long long expir_times;
	
	int len=0;
	char msg_buf[300]={0};
	
	struct input_event event;
	int _pressCounter=0;
	
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	
		// 处理myset
	while(1){

		FD_ZERO(&fds);
		
		for(i=0;i<dp->g_struDevFd.nNum;i++){
			if (dp->g_struDevFd.nFd[i] >= 0){ //如果中间有设备fd失效，则跳过，最初的对应关系不会改变
				FD_SET(dp->g_struDevFd.nFd[i], &fds);
			}
		}
		
		ret = select(dp->nMaxFd, &fds, NULL, NULL, &timeout);
		if(ret < 0)
		{
			//usleep(10000);
			perror("select: "); 
			continue;
		}		
		else
		{
			if(dp->g_struDevFd.nFd[0]>0){
				if (FD_ISSET(dp->g_struDevFd.nFd[0], &fds)){
					
					read(dp->g_struDevFd.nFd[0], &expir_times, sizeof(expir_times));
					_pressCounter++;
				    dbg_mc("counter:%d\n",_pressCounter);	
					if(_pressCounter==3){
						_pressCounter = 0;									
					}
				}
			}
			if(dp->g_struDevFd.nFd[1] > 0){
				if(FD_ISSET(dp->g_struDevFd.nFd[1],&fds)){
					
					len = read(dp->g_struDevFd.nFd[1], msg_buf, 300);					
					if(len > 0){
						dbg_mc("msg_buf:%s\n",msg_buf); 
						UART0_Send(dp->g_struDevFd.nFd[1],msg_buf,strlen(msg_buf)); 
					}					
				}
			}
			if(dp->g_struDevFd.nFd[2] > 0){
				if (FD_ISSET(dp->g_struDevFd.nFd[2], &fds))
				{
					memset(&event,0,sizeof(event));
					ret = read(dp->g_struDevFd.nFd[2], &event, sizeof(event));
					
					if(ret == -1) {
						perror("Failed to read.\n");
						exit(1);
					}
					if(event.type == EV_KEY) 
					{
						dbg_mc("type:%d, code:%d, value:%d\n", event.type, event.code, event.value);					
					}
				}

			}	

		}
	}

}
int main(void)
{
	

	G_DATA *dp;
	dp = (G_DATA *) malloc(sizeof(G_DATA));
	if (dp == NULL)
	{
		dbg_mc("dp init fail\n");
		return -1;
	}
	
	initfd(dp);
	dbg_mc("select1:%d\n",dp->g_struDevFd.nNum);

	selectProcess(dp);

	


	return 0;
}














