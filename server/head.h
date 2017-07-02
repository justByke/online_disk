#ifndef HEAD_H_
#define HEAD_H_

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>



using namespace std;

class SDisk
{
	public:
		SDisk();

		~SDisk();

		int user_register();//用户注册

		int user_login();//user login online_disk

		int disk_pwd();//显示当前目录

		int disk_cd();//更改目录

		int disk_cdup();//返回上层目录

		int disk_mkdir();//创建目录

		int disk_rmdir();//删除目录

		int disk_upload();//上传文件

		int disk_download();//下载文件

		int disk_quit();//退出登录

		int disk_recvcmd();//接收指令 
	private:
		char user_name[32];//用户名

		char user_pass[32];//用户名密码

		int mysocket;//socket id

		char m_cmd[256];//存放指令

		char m_resp[256];//存放返回语句
};





















#endif
