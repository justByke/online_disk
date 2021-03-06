#ifndef HEAD_H_
#define HEAD_H_

#include <iostream>
#include <fstream>
#include <exception>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

static const char* path = "/root/online_disk.1.1/server/user_file";

int socket_connect();
void child_process(int connfd);

using namespace std;

class SDisk
{
	public:
		SDisk();

		~SDisk();

		int user_register();//用户注册

		int user_login();//user login online_disk

		int disk_ls();//显示当前目录

		int disk_cd(const char* dir);//更改目录

		int disk_cdup();//返回上层目录

		int disk_mkdir();//创建目录

		int disk_rmdir(const char* dir);//删除目录

		int disk_rmfile(const char* file);//删除文件

		int disk_upload();//上传文件

		int disk_download();//下载文件

		int disk_quit();//退出登录

		int disk_sendcmd();
		int disk_recvcmd();//接收指令 

		void set_socket(int socket);

		char* get_resp();

		char* get_recmd();
	private:
		char user_name[32];//用户名

		char user_pass[32];//用户名密码

		int mysocket;//socket id

		char m_cmd[BUFFER_SIZE];//存放指令

		char m_recmd[32];//存放返回命令

		char m_resp[BUFFER_SIZE];//存放返回语句
};





















#endif
