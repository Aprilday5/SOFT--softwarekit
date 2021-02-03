/*	#include <stdio.h>

int main(int argc,char ** argv)
{
	printf("hello hi~  %s %s\n",argv[0],argv[1]);
	
}*/	

#include<stdio.h>
#include<syslog.h>
 
int main(int argc,char*argv[])
{
        openlog("xxxx",LOG_CONS | LOG_PID,0);
        syslog(LOG_INFO|LOG_MAIL,"this is  the test!xxxx,info");/*这里LOG_INFO和LOG_MAIL不分先后 */
        closelog();
        return 0;
}

