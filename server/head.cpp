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

int SDisk::disk_ls()
{
	DIR * dir;
	struct dirent *ptr;
	//FILE *dp = fopen("pwd.txt","w");//该文件指针指向保存目录中的文件名

	char buffer[32];
	if(strcmp(getcwd(buffer,32),path) == 0)
	{
		if(access(user_name,0) < 0)
		{
			if(mkdir(user_name,0700) <0)
			{
				cout<<"mkdir error"<<endl;
				return -1;
			}
			
			dir = opendir(user_name);
		}
	}
	ofstream ofile;
	ofile.open("pwd.txt");
	while((ptr = readdir(dir)) != NULL)
	{
		ofile<<ptr->d_name<<endl;			
	}
	ofile<<flush;
	ofile.close();

	return 0;
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
		sscanf(m_resp,"%[^ ]",m_recmd);//  取m_resp中遇到空格为止的字符串
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

char* SDisk::get_recmd()
{
	return m_recmd;
}








