CC=
 
all:
	gcc  client.c  -o  client -static -L/usr/lib  -I/mnt/hgfs/comgitlab/softwarekit/1app/2socket
	arm-linux-gnueabi-gcc  client.c  -o  clientarm -static -L/usr/lib  -I/mnt/hgfs/comgitlab/softwarekit/1app/2socket
	gcc  server.c  -o  server -static -L/usr/lib  -I/mnt/hgfs/comgitlab/softwarekit/1app/2socket
	arm-linux-gnueabi-gcc  server.c  -o  serverarm -static -L/usr/lib  -I/mnt/hgfs/comgitlab/softwarekit/1app/2socket 
clear:
	rm  *.o
