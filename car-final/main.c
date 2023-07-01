
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "serial.h"
#include "net1.h"


#include <stdio.h>

int main()
{
	int ser_fd1 = init_net1();
	if(ser_fd1 < 0)
	{
		puts("init_net1 error.");
		return -1;
	}
	int connfd1 = -1;
	struct sockaddr_in mycli;
	int len = sizeof(mycli);
	connfd1 = accept(ser_fd1, (struct sockaddr *)&mycli, &len);
	if(connfd1 < 0)
	{
		puts("accept1 error.");
		close(ser_fd1);
		return -1;
	}
	puts("accept1 success.");
	do_work1(connfd1);
	close(ser_fd1);
	return 0;
}
