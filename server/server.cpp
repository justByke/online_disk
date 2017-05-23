#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define BUFFER_SIZE 8888

#define FILE_PUSH 1;
#define FILE_PULL 2;
#define FILE_LIST 3;
#define FILE_SENDTO 4;
#define FILE_DELETE 5;

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
void push_file(int connfd,struct sockaddr_in clientaddr);
int read_line(int fd, char *vptr, ssize_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++)
	{
		if ((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break;
		}
		else if (rc == 0)
		{
			*ptr = 0;
			return(n - 1);
		} 
		else
		{
			return(-1);
		}
	}
	*ptr = 0;
	return(n);
}


int main(int argc,char **argv)
{
	struct User user;
	struct Control control;
	struct sockaddr_in servaddr,client_addr;
	char ch[64];
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket falied!\n");
		exit(-1);
	}

	if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1) {
		perror("bind failed!\n");
		exit(-1);
	}

	if(listen(sockfd,20)) {
		perror("listen failed!\n");
		exit(-1);
	}

	while(1) {
		pid_t pid;
		struct sockaddr_in clientaddr;
		socklen_t len = sizeof(clientaddr);
		int connfd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
		if(connfd == -1) {
			perror("accept failed!\n");
			continue;
		}

		else {
			printf("client %s : %d connect successful\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			print_time(ch);
			printf("加入的时间是:%s\n",ch);


			pid = fork();
			if(pid < 0) {
				perror("fork failed\n");
			}
			else if(pid == 0) {
				if(recv(connfd,(char *)&user,sizeof(struct User),0) < 0)
				{
					perror("shuju jie shou cuo wu\n");
					exit(-1);
				}
				printf("客户端发送过来的用户名是:%s,密码是:%s\n",user.user_name,user.passwd);
				if((user.uid = mysql_check_login(user)) <= 0)
				{
					printf("身份验证失败\n");
					exit(-1);
				}
				printf("身份验证成功\n");
				send(connfd,(char *)&user,sizeof(struct User),0);
			//在验证成功之后将这个客户的目录发送过去显示.	
				

				char cmd[20];
				char file[64];
				char line[BUFFER_SIZE];
				recv(connfd,line,BUFFER_SIZE,0);
				printf("%s\n",line);
				if(sscanf(line,"%s%s",cmd,file) == 2)
				{
					printf("%s%s\n",cmd,file);
					if(strcmp(cmd,"push") == 0)
						push_file(connfd,clientaddr);
				}
				/*
				while(1)	
				{
					push_file(connfd,clientaddr);
				}
				exit(0);
				*/
			}
			else {
				close(connfd);
			}

		}
	}

	return 0;
}


void print_time(char *ch)
{
	time_t now;
	struct tm * stm;
	time(&now);
	stm = localtime(&now);
	sprintf(ch,"%02d:%02d:%02d\n",stm->tm_hour,stm->tm_min,stm->tm_sec);
	return ;
}

int mysql_check_login(struct User user)
{
	MYSQL conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW result_row;
	int res;
	int row;
	int column;
	int uid;
	char sql[256] = {0};
	strcpy(sql,"select uid from user where username=\"");
	strcat(sql,user.user_name);
	strcat(sql,"\" and passwd=\"");
	strcat(sql,user.passwd);
	strcat(sql,"\";");
	printf("查询的sql : %s\n",sql);

	uid = -1;
	mysql_init(&conn);
	if(mysql_real_connect(&conn,"127.0.0.1","root","sujian","online_disk",0,NULL,CLIENT_FOUND_ROWS))
	{
		res = mysql_query(&conn,sql);
		if(res)
		{
			perror("SELECT SQL ERROR\n");
			exit(-1);
		}
		else
		{
			res_ptr = mysql_store_result(&conn);
			if(res_ptr)
			{
				column = mysql_num_fields(res_ptr);//获取列数
				row = mysql_num_rows(res_ptr)+1;//获取行数，+1表示还有第一行字段名
				if(row <= 1)
					;
				else
				{
					result_row = mysql_fetch_row(res_ptr);
					printf("获取到UID是: %s\n",result_row[0]);
					uid = atoi(result_row[0]);
				}
			}
			else
			{
				printf("没有查询到匹配的数据\n");
			}
		}
	}
	else
	{
		perror("connect error\n");
		exit(-1);
	}

	mysql_close(&conn);
	return uid;
}


void push_file(int connfd,struct sockaddr_in clientaddr)
{
	char s[32];
	bzero(s,32);
	int file_len = 0;
	int fd = read_line(connfd,s,32);
	if(fd == 0)
	{
		printf("客户端断开连接\n");
		exit(0);
	}
	file_len = atoi(s);
	//printf("%d\n",file_len);
	char buf[BUFFER_SIZE];
	int data_len;
	FILE *fp = NULL;
	if((fp = fopen("data","wb"))==NULL)
	{
		perror("FILE open failed\n");
		exit(-1);
	}
	int size = 0;// block

	int len = 0;
	bzero(buf,BUFFER_SIZE);
	while(data_len = recv(connfd,buf,BUFFER_SIZE,0)) {
	
			write(1, buf, data_len);
			if(data_len < 0) {
				perror("data recv mistake\n");
				exit(-1);
			}
			size++;
			if(size == 1)
			{
				printf("正在接收来自%s : %d 的文件\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			}					
			else
				printf(".");
			//向文件中写入
			int write_len = fwrite(buf,sizeof(char),data_len,fp);
		
			if(write_len > data_len) {
				printf("写入错误\n");
				exit(-1);
			}
			bzero(buf,BUFFER_SIZE);
			len = len + data_len;
			printf("%d\n",len);
			if(len >= file_len)
				break;
	}

	
	if(size > 0)
		printf("%s : %d 的文件传送成功\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
	else
		printf("%s : %d 的文件传送失败\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
	fclose(fp);
//	exit(0);

}












