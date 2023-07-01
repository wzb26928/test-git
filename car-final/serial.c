#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "serial.h"
#include <poll.h>
#include <sys/select.h>

//int uart_set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity);
//int init_uart();
//int writecmd(const char *str);

int writecmd(int fd, const unsigned char *str)
{
	if(fd < 0)
	{
		puts("fd < 0");
		return -1;
	}

	if(NULL == str)
	{
		puts("str is null pointer.");
		return -1;
	}
/*
	unsigned char buf[5] = {0xff, 0x00, 0x00, 0x00, 0xff};
	if(strstr(str, "go_starght"))
	{
		buf[2] = 0x01;
	}
	else if(strstr(str, "go_back"))
	{
		buf[2] = 0x02;
	}
	else if(strstr(str, "go_left"))
	{
		buf[2] = 0x03;
	}
	else if(strstr(str, "go_right"))
	{
		buf[2] = 0x04;
	}
	else if(strstr(str, "stop"))
	{
		buf[2] = 0x00;
	}
*/
	static struct pollfd fds;
	fds.fd = fd;
	fds.events = POLLOUT;
	fds.revents = 0;

	/*60 second timeout*/
	int ret = poll(&fds, 1, 600);
	if(0 >= ret)
	{
		perror("[write_serial]poll");
		return -1;
	}

	if(fds.revents & POLLOUT)
	{
		ret = write(fd, str, 2);
		int i = 0;
		for(i = 0; i < 2; i++)
		{
			printf("%x ",str[i]);
		}
		printf("\nret = %d\n",ret);
		return ret;
	}
	else
	{
		return -1;
	}
}

int init_uart()
{
	//serial_init
	//open ttyUSB0
	int fd_ser = -1;
	fd_ser = open("/dev/ttyATH0", O_RDWR | O_NOCTTY);
	if(fd_ser < 0)
	{
		perror("open serial");
		return -1;
	}
	
	//int ret_s = uart_set(fd_ser, 115200, 0, 8, 1, 'N');
	int ret_s = uart_set(fd_ser, 9600, 0, 8, 1, 'N');
	printf("ret_s:%d\n",ret_s);
	printf("serial->init:init serial success.\n");
	
	return fd_ser;
}

int uart_set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity)
{
	int i;
	int status;
	int speed_arr[] = {B115200, B19200, B9600, B4800, B2400, B1200, B300};
	int name_arr[] = {115200, 19200, 9600, 4800, 2400, 1200, 300};
	
	struct termios options;
	if(tcgetattr(fd, &options) != 0)
	{
		perror("SetupSerial 1");
		return 0;
	}

	//set baud
	for(i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
	{
		if(speed == name_arr[i])
		{
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
		}
	}
	
	options.c_cflag |= CLOCAL;
	options.c_cflag |= CREAD;
	
	switch(flow_ctrl)
	{
		case 0:
			options.c_cflag &= ~CRTSCTS;
			break;
		case 1:
			options.c_cflag &= CRTSCTS;
			break;
		case 2:
			options.c_cflag |= IXON | IXOFF | IXANY;
			break;
	}

	options.c_cflag &= ~CSIZE;
	switch(databits)
	{
		case 5:
			options.c_cflag |= CS5;
			break;
		case 6:
			options.c_cflag |= CS6;
			break;
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			puts("Unsupported data size");
			return 0;
	}

	switch(parity)
	{
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;
			options.c_iflag &= ~INPCK;
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB);
			options.c_iflag |= INPCK;
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;
			options.c_cflag |= ~PARODD;
			options.c_iflag |= INPCK;
			break;
		case 's':
		case 'S':
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			puts("Unsupport parity");
			return 0;
	}

	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		default:
			puts("Unsupported stop bits");
			return 0;
	}

	options.c_oflag &= ~OPOST;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	
	options.c_cc[VTIME] = 1;
	options.c_cc[VMIN] = 1;

	tcflush(fd, TCIFLUSH);
	
	if(tcsetattr(fd, TCSANOW, &options) != 0)
	{
		perror("com set error");
		return 0;
	}
	return 1;
}

int myrecvmsg(int fd, char *buf, int data_len)
{
	if(fd < 0)
	{
		puts("fd < 0");
		return -1;
	}

	if(NULL == buf)
	{
		puts("buf is null.");
		return -1;
	}
	
	//select serial port
	int len, fs_sel;
	fd_set fs_read;
	
	struct timeval time;
	
	FD_ZERO(&fs_read);
	FD_SET(fd, &fs_read);

	time.tv_sec = 10;
	time.tv_usec = 0;

	fs_sel = select(fd+1, &fs_read, NULL, NULL, &time);
	if(fs_sel)
	{
		len = read(fd, buf, data_len);
		return len;
	}
	else
	{
		return -1;
	}
}
