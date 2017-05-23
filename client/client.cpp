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

int file_push(int sockfd,char *filename)
{
	char s[32];
	FILE *fp;

	if((fp =fopen(filename,"rb")) == NULL) {
		perror("file open failed\n");
		exit(-1);
	}

	fseek(fp,0,SEEK_END);
	int file_len = ftell(fp);
	printf("%d\n",file_len);
	sprintf(s,"%d",file_len);
	send(sockfd,s,strlen(s),0);
	rewind(fp);

	char buf[BUFFER_SIZE];
	bzero(buf,BUFFER_SIZE);
	int len = 0;
	while((len = fread(buf,1,BUFFER_SIZE,fp)) > 0)
	{
		write(1, buf, BUFFER_SIZE);

		if(send(sockfd,buf,len,0) < 0)
		{
			perror("send file failed\n");
			exit(-1);
		}

		bzero(buf,BUFFER_SIZE);
		printf(".");
	}

	fclose(fp);

	//close(sockfd);
	return 0;
}

int check_login(int sockfd,struct User *user)
{

	if(send(sockfd,(char *)user,sizeof(struct User),0) < 0)
	{
		perror("发送数据失败\n");
		exit(-1);
	}

	bzero(user,sizeof(struct User));

	if(recv(sockfd,(char *)user,sizeof(struct User),0) < 0)
	{
		perror("接收数据失败\n");
		exit(-1);
	}

	printf("获取后用户名：%s,密码：%s,uid：%d\n",user->user_name,user->passwd,user->uid);


	return user->uid;
}

int socket_connect(struct Addr addr)
{
	struct sockaddr_in servaddr;
	struct hostent *host;
	int sockfd;

	host = gethostbyname(addr.host);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(addr.port);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(host == NULL)
	{
		perror("IP 获取失败\n");
		exit(-1);
	}

	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket 创建失败\n");
		exit(-1);
	}

	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
	{
		perror("connect 失败\n");
		exit(-1);
	}

	return sockfd;
}

int main(int argc,char **argv)
{
	struct User user;

	if(argc != 5)
	{
		printf("usage: %s  addr  port  username passwd\n",argv[0]);
		exit(-1);
	}

	char filename[BUFFER_SIZE];
	struct Addr addr;

	strcpy(addr.host,argv[1]);
	addr.port = atoi(argv[2]);
	strcpy(user.user_name,argv[3]);
	strcpy(user.passwd,argv[4]);

	int sockfd = socket_connect(addr);

	int uid = check_login(sockfd,&user);
	if(uid <= 0)
	{
		perror("验证失败\n");
		exit(-1);
	}
	
	char cmd[BUFFER_SIZE];
	struct Command command;
	while(fgets(cmd,BUFFER_SIZE,stdin) != NULL)
	{
		cmd[strlen(cmd)-1] = 0;
		if(strcmp(cmd,"quit") == 0)
			break;
		if(sscanf(cmd,"%s%s",&command.cmd,&command.file) != 2)
		{
			printf("input error\n");
			continue;
		}
	//	bzero(cmd,sizeof(BUFFER_SIZE));
	//	memcpy(cmd,&command,sizeof(struct Command));
		send(sockfd,cmd,strlen(cmd),0);
		printf("%s\t%s\n",command.cmd,command.file);
		if(strcmp(command.cmd,"push") == 0)
			file_push(sockfd,command.file);
		bzero(cmd,sizeof(BUFFER_SIZE));
	}	

	/*
	while(1)
	{
		printf("\n输入文件名:");
		fgets(filename,BUFFER_SIZE,stdin);
		filename[strlen(filename)-1] = 0;
		file_push(sockfd,filename);
	}
	*/
	return 0;
}

