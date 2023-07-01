#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "cam.h"
#include <signal.h>

int main()
{
	//camera_init
	unsigned int width = 320;
	unsigned int height = 240;
	unsigned int size = 0;
	unsigned int ismjpeg;
	unsigned int index;
	int fd = camera_init("/dev/video0",&width,&height,&size,&ismjpeg);
	if(fd < 0)
	{
		puts("open camera error.");
		return -1;
	}
	printf("width:%u\n",width);
	printf("height:%u\n",height);
	printf("size:%u\n",size);
	printf("ismjpeg:%u\n",ismjpeg);
	//camera_start
	camera_start(fd);
	//cappic
	char *jpeg_ptr = NULL;
	int i = 0;
	for(i=1;i<11;i++)
	{
		int ret = camera_dqbuf(fd,(void**)&jpeg_ptr,&size,&index);
		if(ret == -1)
		{
			exit(EXIT_FAILURE);
		}

		ret = camera_eqbuf(fd,index);
		if(ret == -1)
		{
			exit(EXIT_FAILURE);
		}
	}
	signal(SIGPIPE,SIG_IGN);
	fprintf(stdout,"init camera success.\n");
			while(1)
			{
				ret = camera_dqbuf(fd, (void **)&jpeg_ptr, &size, &index);
				if(ret == -1)
				{
					break;
				}

				memset(buf, 0, sizeof(buf));
				sprintf(buf,"%d",size);
				puts(buf);
				ret = camera_eqbuf(fd, index);
				if(ret == -1)
				{
					break;
				}

			}
	//camera_stop
	camera_stop(fd);
	//camera_exit
	camera_exit(fd);
	sleep(5);
	close(connfd);
	//close
	close(sockfd);
	return 0;
}
