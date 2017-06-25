#ifndef __HEAD_H
#define __HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>//gethostbyname
#include <unistd.h>//close
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

struct Command
{
	char cmd[20];
	char file[64];
};

struct Addr
{
	char host[64];
	int port;
};

struct User 
{
	int uid;
	char user_name[20];
	char passwd[64];
};

int file_push(int sockfd,char *filename);
int check_login(int sockfd,struct User *user);
int socket_connect(struct Addr addr);



#endif
