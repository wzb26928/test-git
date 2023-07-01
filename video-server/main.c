#include "mynet.h"
#include "serial.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int ser_fd = init_net();
	if(ser_fd < 0)
	{
		puts("init_net error.");
		return -1;
	}
	int connfd = -1;
	struct sockaddr_in mycli;
	int len = sizeof(mycli);
	connfd = accept(ser_fd, (struct sockaddr *)&mycli, &len);
	if(connfd < 0)
	{
		puts("accept error.");
		close(ser_fd);
		return -1;
	}
	puts("accept success.");
	do_work(connfd);
	close(ser_fd);
	return 0;
}
