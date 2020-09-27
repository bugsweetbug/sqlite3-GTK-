#include "gtk_win.h"
#include <gtk/gtk.h>
#include "sqlite3.h"

/*


设置字体大小


*/
void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button)
{
	GtkWidget *labelChild;
	PangoFontDescription *font;
	gint fontSize = size;

	font = pango_font_description_from_string("Sans");          //"Sans"字体名
	pango_font_description_set_size(font, fontSize*PANGO_SCALE);//设置字体大小

	if(is_button)
		{
			labelChild = gtk_bin_get_child(GTK_BIN(widget));//取出GtkButton里的label
		}
	else
		{
			labelChild = widget;
		}

	//设置label的字体，这样这个GtkButton上面显示的字体就变了
	gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
	pango_font_description_free(font);
}

void load_image(GtkWidget *image, const char *img_path, const int width, const int height )
{
	/**
	*	void gtk_image_clear(GtkImage *image);
	*	清除控件里的图像数据
	*/
	gtk_image_clear( GTK_IMAGE(image) );        // 清除图像

	/**
	*	GdkPixbuf *gdk_pixbuf_new_from_file(const gchar *filename, GError **error);
	*	图片资源对象的创建
	*	filename:图片路径
	*	error: 	储存错误的指针
	*/
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(img_path, NULL); // 创建图片资源

	/**
	*	GdkPixbuf *gdk_pixbuf_scale_simple(const GdkPixbuf *src, int dest_width,
														int dest_height, GdkInterpType interp_type);
	*	设置图片的大小:
	*	通过gtk_image_new_from_file创建的图片控件，以图片默认大小来显示，不能修改其大小。
	*	如果要改变图片的大小，我们要借助图片资源对象GdkPixbuf，
	*	需要注意的是，GdkPixbuf不属于控件类，它以 Gdk 开头。
	*
	*	src:	gdk_pixbuf_new_from_file()的返回值
	*	dest_width:		需要修改成的图片的宽度
	*	dest_height:	需要修改成的图片的高度
	*	interp_type:	是一个枚举变量，标志图片的加载速度和质量，常用GDK_INTERP_BILINEAR
	****************************************************************************
	*	通过 gdk_pixbuf_new_from_file(), gdk_pixbuf_scale_simple()创建的pixbuf, *
	*	它会动态分配空间, 占用资源, 用完后, 需要人为释放资源。						*
	****************************************************************************
	*/
	GdkPixbuf *dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, width, height, GDK_INTERP_BILINEAR);

	/**
	*	void gtk_image_set_from_pixbuf( GtkImage *image, GdkPixbuf *pixbuf );
	*	图片控件重新设置一张图片
	*	image:	图片控件指针
	*	pixbuf:	指定好大小的pixbuf
	*/
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dest_pixbuf); 	//重设剪裁好的图片

	g_object_unref(src_pixbuf);											// 释放资源
	g_object_unref(dest_pixbuf);											// 释放资源
}


/*


主窗口(用户登录)


*/
void window_main_gtk(GtkButton *button, gpointer user_data)
{
	window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);//-----------主窗口
	gtk_widget_set_size_request(window_main,1080,600);
	gtk_window_set_title(GTK_WINDOW(window_main), "邦邦书店");// 设置标题
	gtk_container_set_border_width(GTK_CONTAINER(window_main), 10); 	// 设置窗口边框的宽度
	gtk_window_set_resizable(GTK_WINDOW(window_main), FALSE);
	gtk_window_set_position(GTK_WINDOW(window_main), GTK_WIN_POS_CENTER);

	GtkWidget* table_main = gtk_table_new(8,9,TRUE);
	gtk_container_add(GTK_CONTAINER(window_main),table_main);
	GtkWidget* lable_welcome = gtk_label_new("欢迎来到邦邦书店");
	set_widget_font_size(lable_welcome, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_welcome,0,3,0,1);

	label_sql = gtk_label_new("");
	set_widget_font_size(label_sql, 10, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_sql,7,9,2,4);


	GtkWidget* button_root = gtk_button_new_with_label("管理员入口");
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_root,7,9,0,1);
	set_widget_font_size(button_root, 18, TRUE);

	GtkWidget* label_title = gtk_label_new("请登录");
	set_widget_font_size(label_title, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_title,3,6,0,2);

	GtkWidget* label_id = gtk_label_new("账号：");
	set_widget_font_size(label_id, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_id,1,3,2,4);
	GtkWidget* label_passwd = gtk_label_new("密码：");
	set_widget_font_size(label_passwd, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_passwd,1,3,4,6);

	entry_user_id = gtk_entry_new();
	set_widget_font_size(entry_user_id, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_user_id,3,7,2,4);
	gtk_entry_set_max_length(GTK_ENTRY(entry_user_id), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_user_id), TRUE);

	entry_user_passwd = gtk_entry_new();
	set_widget_font_size(entry_user_passwd, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_user_passwd,3,7,4,6);
	gtk_entry_set_max_length(GTK_ENTRY(entry_user_passwd), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_user_passwd), FALSE);

	GtkWidget* button_find_passwd = gtk_button_new_with_label("忘记密码?");
	gtk_button_set_relief(GTK_BUTTON(button_find_passwd), GTK_RELIEF_NONE); // 按钮背景色透明
	set_widget_font_size(button_find_passwd, 15, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_find_passwd,7,8,4,6);

	GtkWidget* button_clear = gtk_button_new_with_label("清除");
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_clear,0,2,6,8);
	set_widget_font_size(button_clear, 25, TRUE);

	GtkWidget* button_login = gtk_button_new_with_label("登录");
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_login,3,6,6,8);
	set_widget_font_size(button_login, 25, TRUE);

	GtkWidget* button_register = gtk_button_new_with_label("注册");
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_register,7,9,6,8);
	set_widget_font_size(button_register, 25, TRUE);



	g_signal_connect(window_main,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(button_root,"pressed",G_CALLBACK(main_to_root),NULL);

	g_signal_connect(button_register,"pressed",G_CALLBACK(user_registered_gtk),NULL);
	g_signal_connect(button_login,"pressed",G_CALLBACK(do_user_log_in),NULL);

	g_signal_connect(button_clear,"pressed",G_CALLBACK(userClear),NULL);


	usleep(10000);
	show_only_one(NULL, "window_main");
}

/*


管理员登陆窗口


*/
void  main_to_root(GtkButton *button, gpointer user_data)
{

	root_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);//-----------主窗口
	gtk_widget_set_size_request(root_win,1080,600);
	gtk_window_set_title(GTK_WINDOW(root_win), "administrator");// 设置标题
	gtk_container_set_border_width(GTK_CONTAINER(root_win), 10); 	// 设置窗口边框的宽度
	gtk_window_set_resizable(GTK_WINDOW(root_win), FALSE);
	gtk_window_set_position(GTK_WINDOW(root_win), GTK_WIN_POS_CENTER);



	GtkWidget* table_main = gtk_table_new(8,9,TRUE);
	gtk_container_add(GTK_CONTAINER(root_win),table_main);
	GtkWidget* lable_welcome = gtk_label_new("管理员界面");
	set_widget_font_size(lable_welcome, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_welcome,0,3,0,1);

	GtkWidget* button_vip = gtk_button_new_with_label("会员入口");
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_vip,7,9,0,1);
	set_widget_font_size(button_vip, 18, TRUE);

	GtkWidget* label_title = gtk_label_new("请登录");
	set_widget_font_size(label_title, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_title,3,6,0,2);

	GtkWidget* label_id = gtk_label_new("账号：");
	set_widget_font_size(label_id, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_id,1,3,2,4);

	GtkWidget* label_passwd = gtk_label_new("密码：");
	set_widget_font_size(label_passwd, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_passwd,1,3,4,6);

	/*
	GtkWidget* label_sql = gtk_label_new("tips");
	set_widget_font_size(label_sql, 15, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(label_sql),label_passwd,3,6,6,7);
	*/

	entry_root_id = gtk_entry_new();
	set_widget_font_size(entry_root_id, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_root_id,3,7,2,4);
	gtk_entry_set_max_length(GTK_ENTRY(entry_root_id), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_root_id), TRUE);

	entry_root_passwd = gtk_entry_new();
	set_widget_font_size(entry_root_passwd, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_root_passwd,3,7,4,6);
	gtk_entry_set_max_length(GTK_ENTRY(entry_root_passwd), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_root_passwd), FALSE);

	GtkWidget* button_find_passwd = gtk_button_new_with_label("忘记密码?");
	gtk_button_set_relief(GTK_BUTTON(button_find_passwd), GTK_RELIEF_NONE); // 按钮背景色透明
	set_widget_font_size(button_find_passwd, 18, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_find_passwd,7,8,4,6);

	GtkWidget* button_clear = gtk_button_new_with_label("清除");
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_clear,0,2,6,8);
	set_widget_font_size(button_clear, 25, TRUE);

	GtkWidget* button_login = gtk_button_new_with_label("登录");
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_login,3,6,6,8);
	set_widget_font_size(button_login, 25, TRUE);

	GtkWidget* button_register = gtk_button_new_with_label("注册");
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_register,7,9,6,8);
	set_widget_font_size(button_register, 25, TRUE);

	g_signal_connect(root_win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(button_vip,"pressed",G_CALLBACK(show_only_one),"window_main");
	g_signal_connect(button_register,"pressed",G_CALLBACK(root_registered_gtk),NULL);
	g_signal_connect(button_login,"pressed",G_CALLBACK(do_admin_log_in),NULL);
	g_signal_connect(button_clear,"pressed",G_CALLBACK(rootClear),NULL);
	//g_signal_connect(button_login,"pressed",G_CALLBACK(get_username),entry_user_id);
	//g_signal_connect(button_login,"pressed",G_CALLBACK(get_password),entry_user_passwd);

	usleep(10000);
	show_only_one(NULL, "root_win");

}

/*


会员注册窗口


*/
void user_registered_gtk(GtkButton *button, gpointer user_data)
{

	window_user_registered = gtk_window_new(GTK_WINDOW_TOPLEVEL);//-----------主窗口
	gtk_widget_set_size_request(window_user_registered,1080,600);
	gtk_window_set_title(GTK_WINDOW(window_user_registered), "邦邦书店");// 设置标题
	gtk_container_set_border_width(GTK_CONTAINER(window_user_registered), 10); 	// 设置窗口边框的宽度
	gtk_window_set_resizable(GTK_WINDOW(window_user_registered), FALSE);
	gtk_window_set_position(GTK_WINDOW(window_user_registered), GTK_WIN_POS_CENTER);

	GtkWidget* table_main = gtk_table_new(6,5,TRUE);
	gtk_container_add(GTK_CONTAINER(window_user_registered),table_main);

	GtkWidget* button_return = gtk_button_new_with_label("返回");
	gtk_button_set_relief(GTK_BUTTON(button_return), GTK_RELIEF_NONE); // 按钮背景色透明
	set_widget_font_size(button_return, 15, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_return,0,1,0,1);

	GtkWidget* lable_title = gtk_label_new("会员注册");
	set_widget_font_size(lable_title, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_title,2,3,0,1);

	GtkWidget* label_id = gtk_label_new("账号：");
	set_widget_font_size(label_id, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_id,0,1,1,2);

	GtkWidget* label_user_tel = gtk_label_new("手机号：");
	set_widget_font_size(label_user_tel, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_user_tel,0,1,2,3);

	GtkWidget* label_passwd = gtk_label_new("密码：");
	set_widget_font_size(label_passwd, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_passwd,0,1,3,4);
	GtkWidget* label_passwd_again = gtk_label_new("确认密码：");
	set_widget_font_size(label_passwd_again, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_passwd_again,0,1,4,5);

	entry_user_reg_id = gtk_entry_new();
	set_widget_font_size(entry_user_reg_id, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_user_reg_id,1,4,1,2);
	gtk_entry_set_max_length(GTK_ENTRY(entry_user_reg_id), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_user_reg_id), TRUE);

	entry_user_tel = gtk_entry_new();
	set_widget_font_size(entry_user_tel, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_user_tel,1,4,2,3);
	gtk_entry_set_max_length(GTK_ENTRY(entry_user_tel), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_user_tel), TRUE);

	entry_user_reg_passwd = gtk_entry_new();
	set_widget_font_size(entry_user_reg_passwd, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_user_reg_passwd,1,4,3,4);
	gtk_entry_set_max_length(GTK_ENTRY(entry_user_reg_passwd), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_user_reg_passwd), FALSE);

	entry_user_passwd_again = gtk_entry_new();
	set_widget_font_size(entry_user_passwd_again, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_user_passwd_again,1,4,4,5);
	gtk_entry_set_max_length(GTK_ENTRY(entry_user_passwd_again), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_user_passwd_again), FALSE);

	GtkWidget* button_sign_in = gtk_button_new_with_label("注册");
	set_widget_font_size(button_sign_in, 22, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_sign_in,2,3,5,6);

	g_signal_connect(window_user_registered,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(button_return,"pressed",G_CALLBACK(show_only_one),"window_main");
	g_signal_connect(button_sign_in,"pressed",G_CALLBACK(do_user_register),NULL);

	usleep(10000);
	show_only_one(NULL, "window_user_registered");
}

/*


管理员注册窗口


*/
void root_registered_gtk(GtkButton *button, gpointer user_data)
{


	gtk_widget_hide_all(root_win);
	window_root_registered = gtk_window_new(GTK_WINDOW_TOPLEVEL);//-----------主窗口
	gtk_widget_set_size_request(window_root_registered,1080,600);
	gtk_window_set_title(GTK_WINDOW(window_root_registered), "邦邦书店");// 设置标题
	gtk_container_set_border_width(GTK_CONTAINER(window_root_registered), 10); 	// 设置窗口边框的宽度
	gtk_window_set_resizable(GTK_WINDOW(window_root_registered), FALSE);
	gtk_window_set_position(GTK_WINDOW(window_root_registered), GTK_WIN_POS_CENTER);

	GtkWidget* table_main = gtk_table_new(7,5,TRUE);
	gtk_container_add(GTK_CONTAINER(window_root_registered),table_main);

	GtkWidget* button_return = gtk_button_new_with_label("返回");
	gtk_button_set_relief(GTK_BUTTON(button_return), GTK_RELIEF_NONE); // 按钮背景色透明
	set_widget_font_size(button_return, 15, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_return,0,1,0,1);

	GtkWidget* lable_title = gtk_label_new("管理员注册");
	set_widget_font_size(lable_title, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_title,2,3,0,1);

	GtkWidget* label_id = gtk_label_new("账号：");
	set_widget_font_size(label_id, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_id,0,1,1,2);

	GtkWidget* label_tel = gtk_label_new("手机号：");
	set_widget_font_size(label_tel, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_tel,0,1,2,3);

	GtkWidget* label_invite = gtk_label_new("内推码：");
	set_widget_font_size(label_invite, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_invite,0,1,5,6);

	entry_invite = gtk_entry_new();
	set_widget_font_size(entry_invite, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_invite,1,4,5,6);
	gtk_entry_set_max_length(GTK_ENTRY(entry_invite), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_invite), TRUE);

	GtkWidget* label_passwd = gtk_label_new("密码：");
	set_widget_font_size(label_passwd, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_passwd,0,1,3,4);
	GtkWidget* label_passwd_again = gtk_label_new("确认密码：");
	set_widget_font_size(label_passwd_again, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),label_passwd_again,0,1,4,5);

	entry_root_reg_id = gtk_entry_new();
	set_widget_font_size(entry_root_reg_id, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_root_reg_id,1,4,1,2);
	gtk_entry_set_max_length(GTK_ENTRY(entry_root_reg_id), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_root_reg_id), TRUE);

	entry_root_tel = gtk_entry_new();
	set_widget_font_size(entry_root_tel, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_root_tel,1,4,2,3);
	gtk_entry_set_max_length(GTK_ENTRY(entry_root_tel), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_root_tel), TRUE);

	entry_root_reg_passwd = gtk_entry_new();
	set_widget_font_size(entry_root_reg_passwd, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_root_reg_passwd,1,4,3,4);
	gtk_entry_set_max_length(GTK_ENTRY(entry_root_reg_passwd), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_root_reg_passwd), FALSE);

	entry_root_passwd_again = gtk_entry_new();
	set_widget_font_size(entry_root_passwd_again, 20, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_root_passwd_again,1,4,4,5);
	gtk_entry_set_max_length(GTK_ENTRY(entry_root_passwd_again), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_root_passwd_again), FALSE);

	GtkWidget* button_root_sign_in = gtk_button_new_with_label("注册");
	set_widget_font_size(button_root_sign_in, 22, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_root_sign_in,2,3,6,7);

	g_signal_connect(window_root_registered,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(button_return,"pressed",G_CALLBACK(main_to_root),NULL);
	g_signal_connect(button_root_sign_in,"pressed",G_CALLBACK(do_admin_register),NULL);

	usleep(10000);

	show_only_one(NULL, "window_root_registered");
}


// fflush


void fflush_one(GtkButton *button, gpointer user_data)
{
	gtk_widget_hide_all(root_logged_win);
	gtk_widget_show_all(root_logged_win);
}


/*


管理员菜单界面


*/
void root_to_logged(GtkButton *button, gpointer user_data)
{

	show_only_one(NULL, "root_logged_win");
	root_logged_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title((GtkWindow*)root_logged_win,"邦邦书店");
	//显示窗口标题
	gtk_widget_set_size_request(root_logged_win,1080,600);
	//显示窗口大小
	gtk_window_set_position((GtkWindow*)root_logged_win,GTK_WIN_POS_CENTER_ALWAYS);
	//窗口位置设置：总是居中
	g_signal_connect(root_logged_win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//窗口销毁 即程序结束

	GtkWidget *fixed=gtk_fixed_new();
	//添加容器
	gtk_container_add(GTK_CONTAINER(root_logged_win),fixed);
	//把容器放入窗口


	/*

	书单仓库

	*/
	GtkWidget *label_store = gtk_label_new("书单仓库");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_store,250,15);
	set_widget_font_size(label_store, 20, FALSE);

	GtkWidget *label_name=gtk_label_new("名称:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_name,15,60);
	//把字符串放进容器中
	GtkWidget *label_num=gtk_label_new("数量:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_num,15,110);
	//把字符串放进容器中
	GtkWidget *label_money=gtk_label_new("金额:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_money,15,160);
	//把字符串放进容器中



	//提示信息：查找失败、搜索成功...
	label_book_store_tips = gtk_label_new("");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_book_store_tips,55,215);
	//把字符串放进容器中

	/*-------------------------------行编辑------------------------*/
	entry_add_book_name=gtk_entry_new();
	//设置行编辑控件
	gtk_fixed_put(GTK_FIXED(fixed),entry_add_book_name, 53, 55);
	//放入固定容器
	gtk_entry_set_text(GTK_ENTRY(entry_add_book_name),"");
	//设置控件初始文字为空
	gtk_editable_set_editable((GtkEditable *)entry_add_book_name,TRUE);
	//设置控件可编辑
	gtk_widget_set_size_request(entry_add_book_name,130,30);

	entry_add_book_num=gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed),entry_add_book_num,53,105);
	gtk_entry_set_text(GTK_ENTRY(entry_add_book_num),"");
	gtk_editable_set_editable((GtkEditable *)entry_add_book_num,TRUE);
	gtk_widget_set_size_request(entry_add_book_num,130,30);

	entry_add_book_money=gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed),entry_add_book_money,53,155);
	gtk_entry_set_text(GTK_ENTRY(entry_add_book_money),"");
	gtk_editable_set_editable((GtkEditable *)entry_add_book_money,TRUE);
	gtk_widget_set_size_request(entry_add_book_money,130,30);

	/*-----------------------------------按钮设置----------------------------*/
	GtkWidget *button_add=gtk_button_new_with_label("添加");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_add,60,240);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_add,50,50);
	//设置按钮大小


	GtkWidget *button_del=gtk_button_new_with_label("删除");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_del,60,320);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_del,50,50);
	//设置按钮大小


	GtkWidget *button_ret=gtk_button_new_with_label("返回");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_ret,60,400);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_ret,50,50);
	//设置按钮大小

	GtkWidget *button_to_mem = gtk_button_new_with_label("管理");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_to_mem,425,10);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_to_mem,60,40);
	//设置按钮大小

	GtkWidget *button_to_fflush = gtk_button_new_with_label("刷新");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_to_fflush,55,10);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_to_fflush,60,40);
	//设置按钮大小

	/*------------------------滚动窗口---------------------------------------*/
	GtkWidget *scroll_store=gtk_scrolled_window_new(NULL,NULL);

	gtk_fixed_put(GTK_FIXED(fixed),scroll_store, 190, 55);

	gtk_widget_set_size_request(scroll_store,299,480);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_store),
	                               GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	/*--------------------------------分栏列表----------------------------*/
	char *title[]= {"名称","数量","金额"};

	//gchar *str;

	GtkWidget *clist=gtk_clist_new_with_titles(3,title);//创建带标题的分栏列表

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll_store),clist);
	//gtk_clist_column_titles_hide(GTK_CLIST(clist));
	gtk_clist_set_column_width(GTK_CLIST(clist),0,80);//设置列的宽度
	gtk_clist_set_column_width(GTK_CLIST(clist),1,100);
	gtk_clist_set_column_width(GTK_CLIST(clist),2,80);


	gtk_clist_set_column_justification(GTK_CLIST(clist),1,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(GTK_CLIST(clist),2,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(GTK_CLIST(clist),3,GTK_JUSTIFY_CENTER);


//=========动图

	GtkWidget *image = gtk_image_new_from_pixbuf(NULL);
	gtk_image_clear(GTK_IMAGE(image));
	GtkWidget *button_image = gtk_button_new();
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_image,550,40);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_image,470,525);
	//设置按钮大小
	//gtk_widget_set_sensitive(button_image, FALSE);
	
	GdkPixbufAnimation *anim_pixbuf;
	anim_pixbuf = gdk_pixbuf_animation_new_from_file("./image/bang.gif", NULL);
	image = gtk_image_new_from_animation(anim_pixbuf);
	gtk_button_set_image(GTK_BUTTON(button_image), image);
	

/*
	GtkWidget *image = gtk_image_new_from_pixbuf(NULL);
	gtk_image_clear(GTK_IMAGE(image));
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./image/bang.gif", NULL);
	GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 470, 525, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dst_pixbuf);
	g_object_unref(src_pixbuf);
	g_object_unref(dst_pixbuf);
	gtk_fixed_put(GTK_FIXED(fixed),image,550,40);
*/

	sqlite3 *db;
	sqlite3_open("axiba.db",&db);
	char sql1[100] = "";
	char *errmsg = NULL;
	char **table = NULL;
	int r = 0,c = 0;
	sprintf(sql1,"select name, number, cost from book;");
	sqlite3_get_table(db,sql1,&table,&r,&c,&errmsg);
	if (!errmsg)
		{
			//gtk_label_set_text(GTK_LABEL(label_sql_vip),"获取失败");

		}
	gtk_clist_clear(GTK_CLIST(clist));
	int m=3, i = 0, j = 0;
	char **p = (char *)malloc(m*sizeof(char *));
	for(i=0; i<m; i++)
		{
			p[i]=(char *)malloc(sizeof(char *));
		}
	int k=0;
	if (r!=0)
		{
			for (i = 1; i < r+1; ++i)
				{
					for (j = 0; j < c; ++j)
						{

							//printf("-%s-", table[i*c+j]);
							strcpy(p[k],table[i*c+j]);
							k++;
							if(k==3)
								{
									gtk_clist_append(GTK_CLIST(clist),p);
									k=0;
								}
						}
					//printf("\n");
				}
		}
	for(i=0; i<m; i++)
		{
			free(p[i]);
		}
	free(p);
	sqlite3_close(db);

	//返回主界面
	g_signal_connect(button_ret,"pressed",G_CALLBACK(main_to_root),NULL);
	g_signal_connect(root_logged_win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(button_to_mem,"pressed",G_CALLBACK(root_to_logged_mem), NULL);
	//g_signal_connect(button_to_fflush,"pressed",G_CALLBACK(root_to_logged), NULL);
	g_signal_connect(button_to_fflush,"pressed",G_CALLBACK(fflush_one), NULL);
	g_signal_connect(button_add,"pressed",G_CALLBACK(book_add), NULL);
	g_signal_connect(button_del,"pressed",G_CALLBACK(book_del), NULL);

	show_only_one(NULL, "root_logged_win");
}

// fflush


void fflush_two(GtkButton *button, gpointer user_data)
{
	gtk_widget_hide_all(root_logged_mem_win);
	gtk_widget_show_all(root_logged_mem_win);
}


/*

	会员管理

*/
void root_to_logged_mem(GtkButton *button, gpointer user_data)
{

	root_logged_mem_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title((GtkWindow*)root_logged_mem_win,"邦邦书店");
	//显示窗口标题
	gtk_widget_set_size_request(root_logged_mem_win,1080,600);
	//显示窗口大小
	gtk_window_set_position((GtkWindow*)root_logged_mem_win,GTK_WIN_POS_CENTER_ALWAYS);
	//窗口位置设置：总是居中
	g_signal_connect(root_logged_mem_win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//窗口销毁 即程序结束

	GtkWidget *fixed=gtk_fixed_new();
	//添加容器
	gtk_container_add(GTK_CONTAINER(root_logged_mem_win),fixed);
	//把容器放入窗口

	GtkWidget *label_member = gtk_label_new("会员管理");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_member,790,15);
	set_widget_font_size(label_member, 20, FALSE);

	GtkWidget *label_vip_name=gtk_label_new("ID:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_vip_name,555,60);
	//把字符串放进容器中
	GtkWidget *label_vip_num=gtk_label_new("密码:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_vip_num,555,110);
	//把字符串放进容器中
	GtkWidget *label_vip_money=gtk_label_new("余额:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_vip_money,555,160);
	//把字符串放进容器中



	//提示信息：查找失败、搜索成功...
	GtkWidget *label_tips = gtk_label_new("");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_tips,755,290);
	//把字符串放进容器中

	/*-------------------------------行编辑------------------------*/
	entry_vip_input_id=gtk_entry_new();
	//设置行编辑控件
	gtk_fixed_put(GTK_FIXED(fixed),entry_vip_input_id, 593, 55);
	//放入固定容器
	gtk_entry_set_text(GTK_ENTRY(entry_vip_input_id),"");
	//设置控件初始文字为空
	gtk_editable_set_editable((GtkEditable *)entry_vip_input_id,TRUE);
	//设置控件可编辑
	gtk_widget_set_size_request(entry_vip_input_id,130,30);

	GtkWidget *entry_vip_passwd=gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed),entry_vip_passwd,593,105);
	gtk_entry_set_text(GTK_ENTRY(entry_vip_passwd),"");
	gtk_editable_set_editable((GtkEditable *)entry_vip_passwd,TRUE);
	gtk_widget_set_size_request(entry_vip_passwd,130,30);

	GtkWidget *entry_vip_money=gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed),entry_vip_money,593,155);
	gtk_entry_set_text(GTK_ENTRY(entry_vip_money),"");
	gtk_editable_set_editable((GtkEditable *)entry_vip_money,TRUE);
	gtk_widget_set_size_request(entry_vip_money,130,30);

	/*-----------------------------------按钮设置----------------------------*/
	GtkWidget *button_vip_add=gtk_button_new_with_label("添加");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_vip_add,600,240);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_vip_add,50,50);
	//设置按钮大小
	gtk_widget_set_sensitive(button_vip_add, FALSE);

	GtkWidget *button_vip_del=gtk_button_new_with_label("删除");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_vip_del,600,320);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_vip_del,50,50);
	//设置按钮大小

	GtkWidget *button_vip_ret=gtk_button_new_with_label("返回");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_vip_ret,600,400);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_vip_ret,50,50);
	//设置按钮大小

	GtkWidget *button_to_fflush = gtk_button_new_with_label("刷新");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_to_fflush,595,10);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_to_fflush,60,40);
	//设置按钮大小


	/*------------------------滚动窗口---------------------------------------*/
	GtkWidget *scroll_vip_mem=gtk_scrolled_window_new(NULL,NULL);

	gtk_fixed_put(GTK_FIXED(fixed),scroll_vip_mem, 730, 55);

	gtk_widget_set_size_request(scroll_vip_mem,299,480);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_vip_mem),
	                               GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	/*--------------------------------分栏列表----------------------------*/
	char *vip_title[]= {"会员ID","密码","余额"};

	//gchar *str;

	GtkWidget *vip_clist=gtk_clist_new_with_titles(2,vip_title);//创建带标题的分栏列表

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll_vip_mem),vip_clist);
	//gtk_clist_column_titles_hide(GTK_CLIST(clist));
	gtk_clist_set_column_width(GTK_CLIST(vip_clist),0,150);//设置列的宽度
	gtk_clist_set_column_width(GTK_CLIST(vip_clist),1,150);
	//gtk_clist_set_column_width(GTK_CLIST(vip_clist),2,80);


	gtk_clist_set_column_justification(GTK_CLIST(vip_clist),1,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(GTK_CLIST(vip_clist),2,GTK_JUSTIFY_CENTER);
	//gtk_clist_set_column_justification(GTK_CLIST(vip_clist),3,GTK_JUSTIFY_CENTER);

	//gtk_clist_append(GTK_CLIST(vip_clist),vip_text1);

	g_signal_connect(root_logged_mem_win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(button_vip_ret, "pressed", G_CALLBACK(show_only_one), "root_logged_win");
	g_signal_connect(button_to_fflush, "pressed", G_CALLBACK(fflush_two), NULL);
	g_signal_connect(button_vip_del, "pressed", G_CALLBACK(user_del), NULL);


//==========动图

	GtkWidget *image = gtk_image_new_from_pixbuf(NULL);
	gtk_image_clear(GTK_IMAGE(image));
	GtkWidget *button_image = gtk_button_new();
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_image,50,40);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_image,470,525);
	//设置按钮大小
	//gtk_widget_set_sensitive(button_image, FALSE);
	
	GdkPixbufAnimation *anim_pixbuf;
	anim_pixbuf = gdk_pixbuf_animation_new_from_file("./image/dog.gif", NULL);
	image = gtk_image_new_from_animation(anim_pixbuf);
	gtk_button_set_image(GTK_BUTTON(button_image), image);



	sqlite3 *db;
	sqlite3_open("axiba.db",&db);
	char sql1[100] = "";
	char *errmsg = NULL;
	char **table = NULL;
	int r = 0,c = 0;
	sprintf(sql1,"select id, passwd from user;");
	sqlite3_get_table(db,sql1,&table,&r,&c,&errmsg);
	if (!errmsg)
		{
			//gtk_label_set_text(GTK_LABEL(label_sql_vip),"获取失败");

		}
	gtk_clist_clear(GTK_CLIST(vip_clist));
	int m=2, i = 0, j = 0;
	char **p = (char *)malloc(m*sizeof(char *));
	for(i=0; i<m; i++)
		{
			p[i]=(char *)malloc(sizeof(char *));
		}
	int k=0;
	if (r!=0)
		{
			for (i = 1; i < r+1; ++i)
				{
					for (j = 0; j < c; ++j)
						{

							//printf("-%s-", table[i*c+j]);
							strcpy(p[k],table[i*c+j]);
							k++;
							if(k==2)
								{
									gtk_clist_append(GTK_CLIST(vip_clist),p);
									k=0;
								}
						}
					//printf("\n");
				}
		}
	for(i=0; i<m; i++)
		{
			free(p[i]);
		}
	free(p);
	sqlite3_close(db);

	show_only_one(NULL, "root_logged_mem_win");
}


/*



用户菜单界面



*/
void user_to_logged(GtkButton *button, gpointer user_data)
{

	g_source_remove(timer);//跳转之后取消定时器
	user_logged_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title((GtkWindow*)user_logged_win,"邦邦书店");
	//显示窗口标题
	gtk_widget_set_size_request(user_logged_win,1080,600);
	//显示窗口大小
	gtk_window_set_position((GtkWindow*)user_logged_win,GTK_WIN_POS_CENTER_ALWAYS);
	//窗口位置设置：总是居中
	g_signal_connect(user_logged_win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//窗口销毁 即程序结束

	GtkWidget *fixed=gtk_fixed_new();
	//添加容器
	gtk_container_add(GTK_CONTAINER(user_logged_win),fixed);
	//把容器放入窗口





	//===============书单仓库



	GtkWidget *label_store = gtk_label_new("书单商城");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_store,250,15);
	set_widget_font_size(label_store, 20, FALSE);

	GtkWidget *label_name=gtk_label_new("名称:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_name,15,60);
	//把字符串放进容器中
	GtkWidget *label_num=gtk_label_new("数量:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_num,15,110);
	//把字符串放进容器中
	GtkWidget *label_money=gtk_label_new("金额:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_money,15,160);
	//把字符串放进容器中

	label_left_money=gtk_label_new("");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_left_money,130,235);
	//把字符串放进容器中



	//提示信息：...
	GtkWidget *label_tips = gtk_label_new("剩余金额:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_tips,53,235);
	//把字符串放进容器中



	//==============图片


	GtkWidget *image = gtk_image_new_from_pixbuf(NULL);
	gtk_image_clear(GTK_IMAGE(image));
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./image/si.png", NULL);
	GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 470, 525, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dst_pixbuf);
	g_object_unref(src_pixbuf);
	g_object_unref(dst_pixbuf);
	gtk_fixed_put(GTK_FIXED(fixed),image,550,40);

	//-------------------------------行编辑------------------------
	entry_user_book_name_id=gtk_entry_new();
	//设置行编辑控件
	gtk_fixed_put(GTK_FIXED(fixed),entry_user_book_name_id, 53, 55);
	//放入固定容器
	gtk_entry_set_text(GTK_ENTRY(entry_user_book_name_id),"");
	//设置控件初始文字为空
	gtk_editable_set_editable((GtkEditable *)entry_user_book_name_id,TRUE);
	//设置控件可编辑
	gtk_widget_set_size_request(entry_user_book_name_id,130,30);

	GtkWidget *entry_passwd=gtk_entry_new(); //number
	gtk_fixed_put(GTK_FIXED(fixed),entry_passwd,53,105);
	gtk_entry_set_text(GTK_ENTRY(entry_passwd),"");
	gtk_editable_set_editable((GtkEditable *)entry_passwd,TRUE);
	gtk_widget_set_size_request(entry_passwd,130,30);

	GtkWidget *entry_money=gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed),entry_money,53,155);
	gtk_entry_set_text(GTK_ENTRY(entry_money),"");
	gtk_editable_set_editable((GtkEditable *)entry_money,TRUE);
	gtk_widget_set_size_request(entry_money,130,30);

	//-----------------------------------按钮设置----------------------------
	GtkWidget *button_add=gtk_button_new_with_label("购买");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_add,60,260);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_add,50,50);
	//设置按钮大小


	GtkWidget *button_charge=gtk_button_new_with_label("充值");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_charge,60,340);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_charge,50,50);
	//设置按钮大小

	GtkWidget *button_ret=gtk_button_new_with_label("注销");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_ret,60,420);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_ret,50,50);
	//设置按钮大小

	GtkWidget *button_to_mem = gtk_button_new_with_label("管理");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_to_mem,425,10);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_to_mem,60,40);
	//设置按钮大小

	//------------------------滚动窗口---------------------------------------
	GtkWidget *scroll_store=gtk_scrolled_window_new(NULL,NULL);

	gtk_fixed_put(GTK_FIXED(fixed),scroll_store, 190, 55);

	gtk_widget_set_size_request(scroll_store,299,480);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_store),
	                               GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	//--------------------------------分栏列表----------------------------

	//gchar *str;
	char *title[] = {"书名", "数量", "金额"};
	GtkWidget *clist=gtk_clist_new_with_titles(3,title);//创建带标题的分栏列表

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll_store),clist);
	//gtk_clist_column_titles_hide(GTK_CLIST(clist));
	gtk_clist_set_column_width(GTK_CLIST(clist),0,80);//设置列的宽度
	gtk_clist_set_column_width(GTK_CLIST(clist),1,100);
	gtk_clist_set_column_width(GTK_CLIST(clist),2,80);


	gtk_clist_set_column_justification(GTK_CLIST(clist),1,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(GTK_CLIST(clist),2,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(GTK_CLIST(clist),3,GTK_JUSTIFY_CENTER);

//刷新余额

	sqlite3 *db;

	int ret = sqlite3_open("axiba.db",&db);
	if(ret == SQLITE_OK)
		{
			printf("open database suecess\n");
		}
	else
		printf("open database fail\n");

	char sql[100]="";
	char sql_cmd[100] = "";
	char *errmsg=NULL;
	sprintf(sql,"select balance from recharge  where id = %d;",atoi(name_id));
	char  **table = NULL;//用来存放你数据库查到的结果
	int r,c;//r行数 c列数
	sqlite3_get_table(db, sql,&table,&r,&c,&errmsg);
	gtk_label_set_text(GTK_LABEL(label_left_money),table[1]);

	//==========================之后加的
	sprintf(sql_cmd,"select name, number, cost from book;");
	sqlite3_get_table(db,sql_cmd,&table,&r,&c,&errmsg);
	if (!errmsg)
		{
			gtk_label_set_text(GTK_LABEL(label_sql_vip),"获取失败");

		}
	gtk_clist_clear(GTK_CLIST(clist));
	int m=3, i = 0, j = 0;
	char **p = (char *)malloc(m*sizeof(char *));
	for(i=0; i<m; i++)
		{
			p[i]=(char *)malloc(sizeof(char *));
		}
	int k=0;
	if (r!=0)
		{
			for (i = 1; i < r+1; ++i)
				{
					for (j = 0; j < c; ++j)
						{

							//printf("-%s-", table[i*c+j]);
							strcpy(p[k],table[i*c+j]);
							k++;
							if(k==3)
								{
									gtk_clist_append(GTK_CLIST(clist),p);
									k=0;
								}
						}
					//printf("\n");
				}
		}
	for(i=0; i<m; i++)
		{
			free(p[i]);
		}
	free(p);
	sqlite3_close(db);
	//=======================end

	//返回主界面
	g_signal_connect(button_ret,"pressed",G_CALLBACK(window_main_gtk),NULL);
	g_signal_connect(button_charge,"pressed",G_CALLBACK(window_recharge_gtk),NULL);
	g_signal_connect(button_add,"pressed",G_CALLBACK(user_pay_book),NULL);
	g_signal_connect(user_logged_win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(button_to_mem,"pressed",G_CALLBACK(user_to_logged_book),NULL);
	show_only_one(NULL, "user_logged_win");

}

void fflush_three(GtkButton *button, gpointer user_data)
{
	gtk_widget_hide_all(user_logged_book_win);
	gtk_widget_show_all(user_logged_book_win);
}
/*

已购书目

*/

void user_to_logged_book(GtkButton *button, gpointer user_data)
{
	user_logged_book_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title((GtkWindow*)user_logged_book_win,"邦邦书店");
	//显示窗口标题
	gtk_widget_set_size_request(user_logged_book_win,1080,600);
	//显示窗口大小
	gtk_window_set_position((GtkWindow*)user_logged_book_win,GTK_WIN_POS_CENTER_ALWAYS);
	//窗口位置设置：总是居中

	GtkWidget *fixed=gtk_fixed_new();
	//添加容器
	gtk_container_add(GTK_CONTAINER(user_logged_book_win),fixed);
	//把容器放入窗口

	GtkWidget *label_member = gtk_label_new("已购书目");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_member,790,15);
	set_widget_font_size(label_member, 20, FALSE);

	GtkWidget *label_vip_name=gtk_label_new("ID:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_vip_name,555,60);
	//把字符串放进容器中
	GtkWidget *label_vip_num=gtk_label_new("密码:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_vip_num,555,110);
	//把字符串放进容器中
	GtkWidget *label_vip_money=gtk_label_new("余额:");
	//定义字符串
	gtk_fixed_put(GTK_FIXED(fixed),label_vip_money,555,160);
	//把字符串放进容器中

	/*
		//提示信息：查找失败、搜索成功...
		GtkWidget *label_tips = gtk_label_new("");
		//定义字符串
		gtk_fixed_put(GTK_FIXED(fixed),label_tips,755,290);
		//把字符串放进容器中
	*/

	/*-------------------------------行编辑------------------------*/

	GtkWidget *entry_vip_id=gtk_entry_new();
	//设置行编辑控件
	gtk_fixed_put(GTK_FIXED(fixed),entry_vip_id, 593, 55);
	//放入固定容器
	gtk_entry_set_text(GTK_ENTRY(entry_vip_id),"");
	//设置控件初始文字为空
	gtk_editable_set_editable((GtkEditable *)entry_vip_id,TRUE);
	//设置控件可编辑
	gtk_widget_set_size_request(entry_vip_id,130,30);

	GtkWidget *entry_vip_passwd=gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed),entry_vip_passwd,593,105);
	gtk_entry_set_text(GTK_ENTRY(entry_vip_passwd),"");
	gtk_editable_set_editable((GtkEditable *)entry_vip_passwd,TRUE);
	gtk_widget_set_size_request(entry_vip_passwd,130,30);

	GtkWidget *entry_vip_money=gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed),entry_vip_money,593,155);
	gtk_entry_set_text(GTK_ENTRY(entry_vip_money),"");
	gtk_editable_set_editable((GtkEditable *)entry_vip_money,TRUE);
	gtk_widget_set_size_request(entry_vip_money,130,30);


	/*-----------------------------------按钮设置----------------------------*/
	GtkWidget *button_vip_add=gtk_button_new_with_label("添加");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_vip_add,600,240);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_vip_add,50,50);
	//设置按钮大小


	GtkWidget *button_vip_del=gtk_button_new_with_label("删除");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_vip_del,600,320);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_vip_del,50,50);
	//设置按钮大小

	GtkWidget *button_vip_ret=gtk_button_new_with_label("返回");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_vip_ret,600,400);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_vip_ret,50,50);
	//设置按钮大小

	GtkWidget *button_to_fflush = gtk_button_new_with_label("刷新");
	//设置按钮
	gtk_fixed_put(GTK_FIXED(fixed),button_to_fflush,595,10);
	//把按钮放入容器中
	gtk_widget_set_size_request(button_to_fflush,60,40);
	//设置按钮大小



	/*------------------------滚动窗口---------------------------------------*/
	GtkWidget *scroll_vip_mem=gtk_scrolled_window_new(NULL,NULL);

	gtk_fixed_put(GTK_FIXED(fixed),scroll_vip_mem, 730, 55);

	gtk_widget_set_size_request(scroll_vip_mem,299,480);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_vip_mem),
	                               GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	/*--------------------------------分栏列表----------------------------*/

	char *vip_title[]		= 	{"书名"};
	char sql_fflush[100]	=	"";
	char *errmsg=NULL;



	GtkWidget *vip_clist=gtk_clist_new_with_titles(1,vip_title);//创建带标题的分栏列表

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll_vip_mem),vip_clist);

	gtk_clist_set_column_width(GTK_CLIST(vip_clist),0,80);//设置列的宽度
	//gtk_clist_set_column_width(GTK_CLIST(vip_clist),1,100);
	//gtk_clist_set_column_width(GTK_CLIST(vip_clist),2,80);

	gtk_clist_set_column_justification(GTK_CLIST(vip_clist),1,GTK_JUSTIFY_CENTER);
	//gtk_clist_set_column_justification(GTK_CLIST(vip_clist),2,GTK_JUSTIFY_CENTER);
	//gtk_clist_set_column_justification(GTK_CLIST(vip_clist),3,GTK_JUSTIFY_CENTER);


//============图片


	GtkWidget *image = gtk_image_new_from_pixbuf(NULL);
	gtk_image_clear(GTK_IMAGE(image));
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./image/linux.jpg", NULL);
	GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 470, 525, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dst_pixbuf);
	g_object_unref(src_pixbuf);
	g_object_unref(dst_pixbuf);
	gtk_fixed_put(GTK_FIXED(fixed),image,50,50);
	
	
	show_only_one(NULL, "user_logged_book_win");
	g_signal_connect(button_vip_ret,"pressed",G_CALLBACK(show_only_one),"user_logged_win");
	g_signal_connect(user_logged_book_win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(button_to_fflush,"pressed",G_CALLBACK(fflush_three),NULL);


	sqlite3 *db;

	int ret = sqlite3_open("axiba.db",&db);
	if(ret == SQLITE_OK)
		{
			printf("open database suecess\n");
		}
	else
		printf("open database fail\n");

	const char *id_id = gtk_entry_get_text(GTK_ENTRY(entry_user_id));
	sprintf(sql_fflush,"select had_book from user_book where id = %d;",atoi(id_id));
	char  **table = NULL;//用来存放你数据库查到的结果
	int r,c;//r行数 c列数

	sqlite3_get_table(db, sql_fflush,&table,&r,&c,&errmsg);
	
	if (errmsg != NULL)
		{
			printf("sqlite3_get_table fail\n");
			return ;
		}
	printf("program on 1220...\n");
	printf("r =%d\n",r);
	if(r == 1)
	{	printf("没有值\n");
	return;
	}
	else if(r > 1)
		{
			gtk_clist_clear(GTK_CLIST(vip_clist));
			int m=1, i = 0, j = 0;
			char **p = (char *)malloc(m*sizeof(char *));
			for(i=0; i<m; i++)
				{
					p[i]=(char *)malloc(sizeof(char *));
				}
			int k=0;
			if (r!=0)
				{
					for (i = 2; i < r+1; ++i)
						{
							for (j = 0; j < c; ++j)
								{
									//printf("-%s-", table[i*c+j]);
									strcpy(p[k],table[i*c+j]);
									k++;
									if(k==1)
										{
											gtk_clist_append(GTK_CLIST(vip_clist),p);
											k=0;
										}
								}
							//printf("\n");
						}
				}
			for(i=0; i<m; i++)
				{
					free(p[i]);
				}
			free(p);
		}
	sqlite3_close(db);
	
	

}

/*


==========================购买界面


*/

/*
void window_buy_gtk(GtkButton *button, gpointer user_data)
{
	window_buy= gtk_window_new(GTK_WINDOW_TOPLEVEL);//-----------主窗口
	gtk_widget_set_size_request(window_buy,500,300);
	gtk_window_set_title(GTK_WINDOW(window_buy), "支付界面");// 设置标题
	gtk_container_set_border_width(GTK_CONTAINER(window_buy), 10); 	// 设置窗口边框的宽度
	gtk_window_set_resizable(GTK_WINDOW(window_buy), FALSE);
	gtk_window_set_position(GTK_WINDOW(window_buy), GTK_WIN_POS_CENTER);

	GtkWidget* table_main = gtk_table_new(3,3,TRUE);
	gtk_container_add(GTK_CONTAINER(window_buy),table_main);
	GtkWidget* lable_book_name = gtk_label_new("书籍名称");
	set_widget_font_size(lable_book_name, 18, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_book_name,0,2,0,1);

	GtkWidget* entry_charge_money = gtk_entry_new();
	set_widget_font_size(entry_charge_money, 18, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_charge_money,0,2,1,2);
	//设置行编辑控件
	//gtk_fixed_put(GTK_FIXED(fixed),entry_charge_money, 593, 55);
	//放入固定容器
	gtk_entry_set_text(GTK_ENTRY(entry_charge_money),"");
	//设置控件初始文字为空
	gtk_editable_set_editable((GtkEditable *)entry_charge_money,TRUE);
	//设置控件可编辑
	//gtk_widget_set_size_request(entry_charge_money,130,30);


	GtkWidget* button_pay = gtk_button_new_with_label("支付");
	set_widget_font_size(button_pay, 18, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_pay,2,3,1,2);

	show_only_one(NULL, "window_buy");

	g_signal_connect(window_buy,"destroy",G_CALLBACK(user_to_logged),NULL);
	g_signal_connect(button_pay,"pressed",G_CALLBACK(show_only_one),"user_logged_win");

}

*/
/*

=================充值界面

*/



void window_recharge_gtk(GtkButton *button, gpointer user_data)
{
	
	window_recharge= gtk_window_new(GTK_WINDOW_TOPLEVEL);//-----------主窗口
	gtk_widget_set_size_request(window_recharge,500,300);
	gtk_window_set_title(GTK_WINDOW(window_recharge), "充值界面");// 设置标题
	gtk_container_set_border_width(GTK_CONTAINER(window_recharge), 10); 	// 设置窗口边框的宽度
	gtk_window_set_resizable(GTK_WINDOW(window_recharge), FALSE);
	gtk_window_set_position(GTK_WINDOW(window_recharge), GTK_WIN_POS_CENTER);

	GtkWidget* table_main = gtk_table_new(8,6,TRUE);
	gtk_container_add(GTK_CONTAINER(window_recharge),table_main);

	GtkWidget* lable_amount = gtk_label_new("充值金额");
	set_widget_font_size(lable_amount, 18, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_amount,1,3,1,3);

	GtkWidget* lable_passwd = gtk_label_new("请输入密码");
	set_widget_font_size(lable_passwd, 18, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_passwd,1,3,3,5);

	entry_charge_amount = gtk_entry_new();
	set_widget_font_size(entry_charge_amount, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_charge_amount,3,5,1,3);
	gtk_entry_set_max_length(GTK_ENTRY(entry_charge_amount), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_charge_amount), TRUE);

	entry_charge_passwd = gtk_entry_new();
	set_widget_font_size(entry_charge_passwd, 25, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),entry_charge_passwd,3,5,3,5);
	gtk_entry_set_max_length(GTK_ENTRY(entry_charge_passwd), 100);
	gtk_entry_set_visibility(GTK_ENTRY(entry_charge_passwd), FALSE);

	GtkWidget* button_rccharge = gtk_button_new_with_label("充值");
	set_widget_font_size(button_rccharge, 18, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),button_rccharge,2,4,5,7);


	g_signal_connect(window_recharge,"destroy",G_CALLBACK(user_to_logged),NULL);
	g_signal_connect(button_rccharge,"pressed",G_CALLBACK(book_recharg),NULL);

	show_only_one(NULL, "window_recharge");
}
/*


==============充值成功


*/


void window_pay_success_gtk(GtkButton *button, gpointer user_data)
{
	timer = g_timeout_add(3000, (GSourceFunc)user_to_logged, NULL);
	window_recharge_success = gtk_window_new(GTK_WINDOW_TOPLEVEL);//-----------主窗口
	gtk_widget_set_size_request(window_recharge_success,500,300);
	gtk_window_set_title(GTK_WINDOW(window_recharge_success), "支付界面");// 设置标题
	gtk_container_set_border_width(GTK_CONTAINER(window_recharge_success), 10); 	// 设置窗口边框的宽度
	gtk_window_set_resizable(GTK_WINDOW(window_recharge_success), FALSE);
	gtk_window_set_position(GTK_WINDOW(window_recharge_success), GTK_WIN_POS_CENTER);

	GtkWidget* table_main = gtk_table_new(8,6,TRUE);
	gtk_container_add(GTK_CONTAINER(window_recharge_success),table_main);

	GtkWidget* lable_pay_ok = gtk_label_new("充值完成");
	set_widget_font_size(lable_pay_ok, 35, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_pay_ok,1,5,1,5);

	GtkWidget* lable_return = gtk_label_new("3s后返回主页");
	set_widget_font_size(lable_return, 16, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table_main),lable_return,2,4,5,6);

	//sleep(3);
	//g_signal_connect(NULL,NULL,G_CALLBACK(user_to_logged),NULL);

	g_signal_connect(window_recharge_success,"destroy",G_CALLBACK(user_to_logged),NULL);



	show_only_one(NULL, "window_recharge_success");

}

//管理员注册
void do_admin_register(GtkButton* button,gpointer user_data)
{
	sqlite3* db;
	sqlite3_open("axiba.db",&db);
	const char *id = gtk_entry_get_text(GTK_ENTRY(entry_root_reg_id));
	const char *passwd = gtk_entry_get_text(GTK_ENTRY(entry_root_reg_passwd));
	//先判断一下你注册过没---查id在admin表中是否有
	char cmd1[100] = "";
	sprintf(cmd1,"select *from admin where id = %d;",atoi(id));
	char **table = NULL;
	int r,c ;
	char *errmsg = NULL;
	sqlite3_get_table(db,cmd1,&table,&r,&c,&errmsg);
	if (r != 0)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_root_reg_id),"该账号已存在");
			gtk_entry_set_text(GTK_ENTRY(entry_root_reg_passwd),"");
			gtk_entry_set_text( GTK_ENTRY(entry_root_tel), "");
			gtk_entry_set_text( GTK_ENTRY(entry_root_passwd_again), "");
			gtk_entry_set_text( GTK_ENTRY(entry_invite), "");
			return;
		}
	char cmd2[100] = "";
	sprintf(cmd2,"insert into admin  values ( %d,\'%s\');",atoi(id),passwd);
	sqlite3_exec(db,cmd2,NULL,NULL,&errmsg);
	if (errmsg == NULL)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_root_reg_id),"注册成功");
			gtk_entry_set_text(GTK_ENTRY(entry_root_reg_passwd),"");
			gtk_entry_set_text( GTK_ENTRY(entry_root_tel), "");
			gtk_entry_set_text( GTK_ENTRY(entry_root_passwd_again), "");
			gtk_entry_set_text( GTK_ENTRY(entry_invite), "");
		}
	sqlite3_close(db);
}


//会员注册


void do_user_register(GtkButton* button,gpointer user_data)
{
	sqlite3* db;
	sqlite3_open("axiba.db",&db);
	const char *id = gtk_entry_get_text(GTK_ENTRY(entry_user_reg_id));
	const char *passwd = gtk_entry_get_text(GTK_ENTRY(entry_user_reg_passwd));
	//先判断一下你注册过没---查id在user表中是否有
	char cmd1[100] = "";
	sprintf(cmd1,"select *from user where id = %d;",atoi(id));
	char **table = NULL;
	int r,c ;
	char *errmsg = NULL;
	sqlite3_get_table(db,cmd1,&table,&r,&c,&errmsg);
	if (r != 0)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_user_reg_id),"该账号已存在");
			gtk_entry_set_text(GTK_ENTRY(entry_user_reg_passwd),"");
			gtk_entry_set_text( GTK_ENTRY(entry_user_tel), "");
			gtk_entry_set_text( GTK_ENTRY(entry_user_passwd_again), "");
			return;
		}

	char cmd3[100] = "";
	sprintf(cmd3,"insert into recharge  values ( %d,\'3\');",atoi(id));
	sqlite3_exec(db,cmd3,NULL,NULL,&errmsg);

	char cmd4[100] = "";
	sprintf(cmd4,"insert into user_book (id) values (%d);",atoi(id));
	sqlite3_exec(db,cmd4,NULL,NULL,&errmsg);

	char cmd2[100] = "";
	sprintf(cmd2,"insert into user  values ( %d,\'%s\');",atoi(id),passwd);
	sqlite3_exec(db,cmd2,NULL,NULL,&errmsg);
	if (errmsg == NULL)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_user_reg_id),"注册成功");
			gtk_entry_set_text(GTK_ENTRY(entry_user_reg_passwd),"");
			gtk_entry_set_text( GTK_ENTRY(entry_user_tel), "");
			gtk_entry_set_text( GTK_ENTRY(entry_user_passwd_again), "");

		}

	sqlite3_close(db);
}

//会员登录
void do_user_log_in(GtkButton* button,gpointer user_data)
{
	name_id = gtk_entry_get_text(GTK_ENTRY(entry_user_id));

	sqlite3* db;
	sqlite3_open("axiba.db",&db);
	const char *id = gtk_entry_get_text(GTK_ENTRY(entry_user_id));
	const char *passwd = gtk_entry_get_text(GTK_ENTRY(entry_user_passwd));

	char cmd2[100] = "";
	char **table = NULL;
	int r,c ;
	char *errmsg = NULL;
	sprintf(cmd2,"select *from user where id = %d and passwd = \'%s\';",atoi(id),passwd);
	sqlite3_get_table(db,cmd2,&table,&r,&c,&errmsg);
	//sqlite3_exec(db,cmd2,NULL,NULL,&errmsg);
	sqlite3_close(db);
	if (r == 0)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_user_id),"账号或密码错误");
			gtk_entry_set_text(GTK_ENTRY(entry_user_passwd),"");

		}
	else
		{
			user_to_logged(NULL, NULL);
		}


}


//管理员登录


void do_admin_log_in(GtkButton* button,gpointer user_data)
{
	sqlite3* db;
	sqlite3_open("axiba.db",&db);
	const char *id = gtk_entry_get_text(GTK_ENTRY(entry_root_id));
	const char *passwd = gtk_entry_get_text(GTK_ENTRY(entry_root_passwd));

	char cmd2[100] = "";
	char **table = NULL;
	int r,c ;
	char *errmsg = NULL;
	sprintf(cmd2,"select *from admin where id = %d and passwd = \'%s\';",atoi(id),passwd);
	sqlite3_get_table(db,cmd2,&table,&r,&c,&errmsg);
	//sqlite3_exec(db,cmd2,NULL,NULL,&errmsg);
	sqlite3_close(db);
	if (r == 0)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_root_id),"账号或密码错误");
			gtk_entry_set_text(GTK_ENTRY(entry_root_passwd),"");

		}
	else
		{
			root_to_logged(NULL, NULL);
		}
}

//==============管理员删除书籍

void book_del(GtkButton *button,gpointer user_data)
{
	sqlite3 *db;

	const char *name = gtk_entry_get_text(GTK_ENTRY(entry_add_book_name));
	//const char *cost = gtk_entry_get_text(GTK_ENTRY(entry_add_book_num));
	//const char *number = gtk_entry_get_text(GTK_ENTRY(entry_add_book_money));

	int ret = sqlite3_open("axiba.db",&db);
	if(ret == SQLITE_OK)
		printf("open database sucess !\n");
	else
		printf("open database fail !\n");
	//const gchar *text=gtk_button_get_label(GTK_BUTTON(button));

	char sql[100]="";
	char *errmsg=NULL;
	sprintf(sql,"delete from book where name = \'%s\' ;",name);

	ret =sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if (ret != SQLITE_OK)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_add_book_name),"删除失败");
		}
	else
		gtk_entry_set_text(GTK_ENTRY(entry_add_book_name),"删除成功");
	sqlite3_close(db);
}

//=========== 管理员增加书籍


void book_add(GtkButton *button,gpointer user_data)
{
	sqlite3 *db;

	const char *name = gtk_entry_get_text(GTK_ENTRY(entry_add_book_name));
	const char *cost = gtk_entry_get_text(GTK_ENTRY(entry_add_book_num));
	const char *number = gtk_entry_get_text(GTK_ENTRY(entry_add_book_money));
	int ret=sqlite3_open("axiba.db",&db);
	//const gchar *text=gtk_button_get_label(GTK_BUTTON(button));

	int r,c;
	char sql[100]="";
	char **table = "";
	char *errmsg=NULL;
	char sql_read[100] = "";
	sprintf(sql_read,"select cost from book where name = \'%s\';",name);


	sqlite3_get_table(db, sql_read,&table,&r,&c,&errmsg);
	if(r != 0)
		{
			gtk_label_set_text(GTK_LABEL(label_book_store_tips),"已存在此书籍");
			return;
		}



	sprintf(sql,"insert into book values(\'%s\',\'%s\',\'%s\');",name,cost,number);
	ret =sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if (ret != SQLITE_OK)
		{
			gtk_label_set_text(GTK_LABEL(label_book_store_tips),"添加失败");
		}
	else
		gtk_label_set_text(GTK_LABEL(label_book_store_tips),"添加成功");

	//char  **table = NULL;//用来存放你数据库查到的结果
	//int r,c;//r行数 c列数
	//sqlite3_get_table(db, "select * from book;",&table,&r,&c,&errmsg);
	/* if (errmsg != NULL)//如果我们出现错误就会打印errmsg
	{
	    printf("%s\n", errmsg);
	}*/

	sqlite3_close(db);
}




//=========== 会员充值


void book_recharg(GtkButton *button,gpointer user_data)
{
	sqlite3 *db;

	const char *money = gtk_entry_get_text(GTK_ENTRY(entry_charge_amount));
	//const char *number = gtk_entry_get_text(GTK_ENTRY(entry_add_book_money));
	int ret = sqlite3_open("axiba.db",&db);
	if(ret == SQLITE_OK)
		{
			printf("open database suecess\n");
		}
	else
		printf("open database fail\n");
	//printf("id : %s\n", name_id);

	char sql[100]="";
	char sql1[100]="";
	char *errmsg=NULL;
	char *errmsg1=NULL;
	sprintf(sql,"select balance from recharge  where id = %d;",atoi(name_id));
	char  **table = NULL;//用来存放你数据库查到的结果
	int r,c;//r行数 c列数
	sqlite3_get_table(db, sql,&table,&r,&c,&errmsg);

	//printf("table : %s\n", table[1]);

	int ba=atoi(table[1])+atoi(money);

	//printf("ba : %d\n", ba);
	//char *str = NULL;
	//sprintf(str, "%d \n", ba);
	//printf("str : %s\n", str);


	sprintf(sql1,"update recharge set balance = \"%d\"  where id = %d;",ba,atoi(name_id));
	sqlite3_exec(db,sql1,NULL,NULL,&errmsg1);
	if (errmsg != 0)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_charge_amount),"充值失败");
		}
	else
		{
			gtk_entry_set_text(GTK_ENTRY(entry_charge_amount),"充值成功");

		}
	sqlite3_close(db);
}


//管理员删除用户


void user_del(GtkButton *button,gpointer user_data)
{
	sqlite3 *db;

	const char *id = gtk_entry_get_text(GTK_ENTRY(entry_vip_input_id));

	int ret=sqlite3_open("axiba.db",&db);
	if(ret == SQLITE_OK)
		printf("open database sucess !\n");
	else
		printf("open database fail !\n");
	//const gchar *text=gtk_button_get_label(GTK_BUTTON(button));
	int int_id = atoi(id);
	char sql[100]="";
	char *errmsg=NULL;

	sprintf(sql,"delete from user where id = %d;",int_id);
	ret =sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if (ret != SQLITE_OK)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_vip_input_id),"删除失败");
		}
	else
		gtk_entry_set_text(GTK_ENTRY(entry_vip_input_id),"删除成功");
	ret = sqlite3_close(db);
	if(ret == SQLITE_OK)
		printf("close database sucess !\n");
	else
		printf("close database fail !\n");
}


//=========== 会员购买


void user_pay_book(GtkButton *button,gpointer user_data)
{
	sqlite3 *db;

	const char *pay_book_name = gtk_entry_get_text(GTK_ENTRY(entry_user_book_name_id));
	const char *id = gtk_entry_get_text(GTK_ENTRY(entry_user_id));
	int ret = sqlite3_open("axiba.db",&db);
	if(ret == SQLITE_OK)
		{
			printf("open database suecess\n");
		}
	else
		printf("open database fail\n");






	//查余额
	char sql1[100]="";
	sprintf(sql1,"select balance from recharge  where id = %d;",atoi(id));
	char  **table = NULL;//用来存放你数据库查到的结果
	int r,c;//r行数 c列数
	sqlite3_get_table(db, sql1,&table,&r,&c,NULL);
	printf("table: %d\n", atoi(table[1]));
	printf("pay_book_name: %s\n", pay_book_name);

	//查书本单价
	char *errmsg1=NULL;
	char sql2[100]="";
	sprintf(sql2,"select cost from book  where name = \"%s\";",pay_book_name);
	char  **table2 = NULL;//用来存放你数据库查到的结果
	sqlite3_get_table(db, sql2,&table2,&r,&c,&errmsg1);
	if(r == 0)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_user_book_name_id),"没有该书籍");

			return;
		}
	printf("table2: %d\n", atoi(table2[1]));

	//最新余额
	int ba=atoi(table[1])-atoi(table2[1]);

	char *errmsg2=NULL;
	char sql3[100]="";
	sprintf(sql3,"update recharge set balance = \"%d\"  where id = %d;",ba,atoi(id));
	sqlite3_exec(db,sql3,NULL,NULL,&errmsg2);



	char sql[100]="";
	char *errmsg=NULL;
	sprintf(sql,"insert into user_book values (%d,\'%s\');",atoi(id),pay_book_name);
	sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if (errmsg != 0)
		{
			gtk_entry_set_text(GTK_ENTRY(entry_user_book_name_id),"购买失败");
		}
	else
		{
			gtk_entry_set_text(GTK_ENTRY(entry_user_book_name_id),"购买成功");

		}


	char sql4[100]="";
	char *errmsg4=NULL;
	sprintf(sql4,"select balance from recharge  where id = %d;",atoi(name_id));
	char  **table3 = NULL;//用来存放你数据库查到的结果
	sqlite3_get_table(db, sql4,&table3,&r,&c,&errmsg4);
	gtk_label_set_text(GTK_LABEL(label_left_money),table3[1]);

	sqlite3_close(db);
}