#include <gtk/gtk.h>
#include "sqlite3.h"
#include "gtk_win.h"



int main(int argc, char  *argv[])
{
	//-------------------数据库准备工作
	sqlite3*db;
	char *errmsg = NULL;
	sqlite3_open("axiba.db",&db);
	sqlite3_exec(db,"create table admin (id int, passwd text);",NULL,NULL,&errmsg);
	sqlite3_exec(db,"create table user (id int, passwd text);",NULL,NULL,&errmsg);
		// id 名字 数量 价格 
	sqlite3_exec(db,"CREATE TABLE book(name varchar(32), number char(30),cost int(11));",NULL,NULL,&errmsg);
	sqlite3_exec(db,"create table recharge (id int, balance text);",NULL,NULL,&errmsg);
	sqlite3_exec(db,"create table user_book (id int, had_book text);",NULL,NULL,&errmsg);
	sqlite3_close(db);
	
	gtk_init(&argc,&argv);
	
	window_main_gtk(NULL, NULL);

	gtk_main();
	return 0;
}