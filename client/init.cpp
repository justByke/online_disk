#include "head.h"

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


