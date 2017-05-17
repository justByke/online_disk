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

#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

struct Addr
{
	char host[64];
	int port;
};

int file_push(struct Addr addr,char *filename)
{
	struct sockaddr_in servAddr;
	struct hostent* host;
	int sockfd;
	FILE *fp;

	host = gethostbyname(addr.host);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(SERVER_PORT);
	//servAddr.sin_addr = *((struct in_addr *)host->h_addr);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(host == NULL) {
		perror("获取IP地址失败\n");
		exit(-1);
	}

	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("socket 失败\n");
		exit(-1);
	}

	if(connect(sockfd,(struct sockaddr *)&servAddr,sizeof(servAddr)) == -1) {
		perror("connect failed\n");
		exit(-1);
	}

	if((fp =fopen(filename,"rb")) == NULL) {
		perror("file open failed\n");
		exit(-1);
	}

	char buf[BUFFER_SIZE];
	bzero(buf,BUFFER_SIZE);
	int len = 0;
	while((len = fread(buf,1,BUFFER_SIZE,fp)) > 0)
	{
		if(send(sockfd,buf,len,0) < 0)
		{
			perror("send file failed\n");
			exit(-1);
		}

		bzero(buf,BUFFER_SIZE);
		printf(".");
	}

	fclose(fp);
	close(sockfd);

	return 0;
}

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("usage: %s  addr  port\n",argv[0]);
		exit(-1);
	}

	char filename[BUFFER_SIZE];
	struct Addr addr;

	strcpy(addr.host,argv[1]);
	addr.port = atoi(argv[2]);

	while(1)
	{
		printf("\n输入文件名:");
		fgets(filename,BUFFER_SIZE,stdin);
		filename[strlen(filename)-1] = 0;
		file_push(addr,filename);
	}

	return 0;
}

