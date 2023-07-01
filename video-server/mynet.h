#ifndef __MYNET_H__
#define __MYNET_H__
	int init_net();
	int do_work(int connfd);
	int get_file_name(char *filename, char *cmd);
	int get_req(char *myreq, char *cmd);
	int get_file_len(int fd);
#endif
