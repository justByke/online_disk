#ifndef __HEAD_H
#define __HEAD_H

#include <stdio.h>
#include <string>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

#define FILE_PUSH 1;
#define FILE_PULL 2;
#define FILE_LIST 3;
#define FILE_SENDTO 4;
#define FILE_DELETE 5;

using namespace std;

struct User
{
	int uid;
	char user_name[20];
	char passwd[64];
};

struct Control
{
	int uid;
	int control;
};

void print_time(char *ch);
int mysql_check_login(struct User user);
void push_file(int connfd,struct sockaddr_in clientaddr,struct User user,char file[64]);
void list_file(int connfd,struct sockaddr_in clientaddr,struct User user);

#endif
