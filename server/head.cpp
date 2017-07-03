#include "head.h"

using namespace std;

SDisk::SDisk()
{
	
}

SDisk::~SDisk()
{

}

int SDisk::user_login()
{
	disk_recvcmd();
	sscanf(m_resp,"%s%s",&user_name,&user_pass);
	const char* db = "online_disk",*server = "127.0.0.1",
	      		*user = "root",*password = "sujian"; 

	MYSQL connection;
	mysql_init(&connection);
	char sql[256] = {0};
	strcpy(sql,"select uid from user where username=\"");
	strcat(sql,user_name);
	strcat(sql,"\" and passwd=\"");
	strcat(sql,user_pass);
	strcat(sql,"\";");

	if(mysql_real_connect(&connection,server,user,password,db,0,NULL,0))
	{
		int res = mysql_query(&connection,sql);
		if(res)
		{
			cout<<"Select Error."<<endl;
			return -1;
		}
		else
		{
			MYSQL_RES *res_ptr = mysql_store_result(&connection);
			if(res_ptr)
			{
				int column = mysql_num_fields(res_ptr);//获取列数
				int row = mysql_num_rows(res_ptr)+1;//获取行数，+1表示还有第一行字段名
				if(row <= 1)
				{
					memset(m_cmd,0,BUFFER_SIZE);
					strcpy(m_cmd,"-1");
					send(mysocket,m_cmd,BUFFER_SIZE,0);

				}
				else
				{
					MYSQL_ROW result_row = mysql_fetch_row(res_ptr);
						cout<<"获取到UID是: "<<result_row[0]<<endl;
					int uid = atoi(result_row[0]);
					memset(m_cmd,0,BUFFER_SIZE);
					strcpy(m_cmd,"获取的UID是:");
					strcat(m_cmd,result_row[0]);
					send(mysocket,m_cmd,strlen(m_cmd),0);
				}
			}
		}

	}
	return 0;
}

int socket_connect()
{
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		return -1;
	
	if((bind(sockfd,(struct sockaddr *)&addr,sizeof(addr))) == -1)
		return -1;

	if(listen(sockfd,20))
		return -1;

	return sockfd;
}
void SDisk::set_socket(int socket)
{
	mysocket = socket;
}
int SDisk::disk_recvcmd()
{
	memset(m_resp,0,BUFFER_SIZE);
	try
	{
		recv(mysocket,m_resp,BUFFER_SIZE,0);
	}
	catch (exception& e)
	{
		cout<<e.what()<<endl;
		return -1;
	}
	return 0;
}

char* SDisk::get_resp()
{
	return m_resp;
}

int main()
{
	int sockfd = socket_connect();

	while(1)
	{
		pid_t pid;
		struct sockaddr_in clientaddr;
		socklen_t len = sizeof(clientaddr);
		int connfd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
		if(connfd == -1)
		{
			perror("accept failed\n");
			continue;
		}
		else
		{
			cout<<"客户端连接成功"<<endl;

			pid = fork();
			if(pid < 0)
			{
				perror("fork failed\n");
			}
			else if(pid == 0)
			{
				SDisk disk_server;
				disk_server.set_socket(connfd);
				disk_server.user_login();

				disk_server.disk_recvcmd();
				while(strcmp(disk_server.get_resp(),"quit") != 0)
				{
					
				}
			}
			else
			{
				close(connfd);
			}

		}
	}

	return 0;
}










