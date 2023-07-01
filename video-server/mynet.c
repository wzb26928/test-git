#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "cam.h"
#include <signal.h>

int init_net()
{
	int ser_fd = -1;
	//socket
	ser_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(ser_fd < 0)
	{
		puts("socket error.");
		return ser_fd;
	}
	puts("socket success.");
	//bind
	struct sockaddr_in myser;
	memset(&myser, 0, sizeof(myser));
	myser.sin_family = AF_INET;
	myser.sin_port = htons(8888);
	myser.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(ser_fd, (struct sockaddr *)&myser, sizeof(myser));
	if(ret != 0)
	{
		puts("bind error.");
		close(ser_fd);
		ser_fd = -1;
		return ser_fd;
	}
	puts("bind success.");
	//listen
	ret = listen(ser_fd, 5);
	if(ret != 0)
	{
		puts("listen error.");
		close(ser_fd);
		ser_fd = -1;
		return ser_fd;
	}
	puts("listen success.");
	return ser_fd;
}

int get_file_name(char *filename, char *cmd)
{
	if(NULL == filename && NULL == cmd)
	{
		puts("get_file_name:error.");
		return -1;
	}

	char *p = cmd+4;
	int len = strlen(cmd);
	cmd[len-1] = 0;
	strcpy(filename, p);
	return 0;
}

int get_req(char *myreq, char *cmd)
{
	if(NULL == myreq && NULL == cmd)
	{
		puts("get_req:error.");
		return -1;
	}

	char *p = cmd;
	int i = 0;
	while(*p != ' ')
	{
		myreq[i] = *p;
		i++;
		p++;
	}
	myreq[i] = 0;
	return 0;
}

int get_file_len(int fd)
{
	if(fd < 0)
	{
		puts("get file len error.");
		return -1;
	}
	int len = lseek(fd, 0, SEEK_SET);
	len = lseek(fd, 0, SEEK_END);
	return len;
}

int do_work(int connfd)
{
	if(connfd < 0)
	{
		puts("connfd < 0");
		return -1;
	}
	puts("do_work");
	char cmd[50];
	memset(cmd, 0, sizeof(cmd));
	int ret = recv(connfd, cmd, sizeof(cmd), 0);
	if(ret > 0)
	{
		unsigned int width,height,size,ismjpeg,index;
		unsigned char *jpeg_ptr = NULL;
		width = 320;
		height = 240;
		int fd = camera_init("/dev/video0",&width, &height, &size, &ismjpeg);
		if(fd < 0)
		{
			return -1;
		}
		camera_start(fd);
		signal(SIGPIPE, SIG_IGN);

		//puts(cmd);
		char filename[40] = {0};
		get_file_name(filename,cmd);
		puts(filename);
		char req[5] = {0};
		get_req(req,cmd);
		puts(req);
		if(strcmp(req,"get") == 0)
		{
			while(1)
			{
				camera_dqbuf(fd,(void **) &jpeg_ptr, &size, &index);
				char filelen[20] = {0};
				sprintf(filelen,"len:%d",size);
				puts(filelen);
				send(connfd, filelen, 20, 0);
				send(connfd, jpeg_ptr, size, 0);
				camera_eqbuf(fd, index);
			}
			camera_stop(fd);
			camera_exit(fd);
		}
		else
		{
			camera_stop(fd);
			camera_exit(fd);
		}
	}
	return 0;
}
