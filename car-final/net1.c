#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include "serial.h"

int init_net1()
{
	int ser_fd1 = -1;
	//socket
	ser_fd1 = socket(AF_INET, SOCK_STREAM, 0);
	if(ser_fd1 < 0)
	{
		puts("socket1 error.");
		return ser_fd1;
	}
	puts("socket1 success.");
	//bind
	struct sockaddr_in myser;
	memset(&myser, 0, sizeof(myser));
	myser.sin_family = AF_INET;
	myser.sin_port = htons(7777);
	myser.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(ser_fd1, (struct sockaddr *)&myser, sizeof(myser));
	if(ret != 0)
	{
		puts("bind1 error.");
		close(ser_fd1);
		ser_fd1 = -1;
		return ser_fd1;
	}
	puts("bind1 success.");
	//listen
	ret = listen(ser_fd1, 5);
	if(ret != 0)
	{
		puts("listen1 error.");
		close(ser_fd1);
		ser_fd1 = -1;
		return ser_fd1;
	}
	puts("listen1 success.");
	return ser_fd1;
}


int do_work1(int connfd1)
{
	if(connfd1 < 0)
	{
		puts("connfd1 < 0");
		return -1;
	}
	puts("do work1");
	int fd_uart = init_uart();
	char cmd[50];
	memset(cmd, 0, sizeof(cmd));
	int ret;
	while(1)
	{
		memset(cmd, 0, sizeof(cmd));
		ret = recv(connfd1, cmd, sizeof(cmd), 0);
		if(ret > 0)
		{
			if(strcmp(cmd,"qq") == 0)
			{
				writecmd(fd_uart,"qq");
			}
			else if(strcmp(cmd,"hh") == 0)
			{
				writecmd(fd_uart,"hh");
			}
			else if(strcmp(cmd,"ll") == 0)
			{
				writecmd(fd_uart,"ll");
			}
			else if(strcmp(cmd,"rr") == 0)
			{
				writecmd(fd_uart,"rr");
			}
			else if(strcmp(cmd,"ss") == 0)
			{
				writecmd(fd_uart,"ss");
			}
			puts(cmd);
		}
	}
	close(fd_uart);
	return 0;
}
