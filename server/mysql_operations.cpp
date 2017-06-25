#include "head.h"

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



