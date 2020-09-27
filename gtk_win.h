#ifndef __GTK_WIN_H_
#define __GTK_WIN_H_

#include <gtk/gtk.h>
#include <string.h>
#include "sqlite3.h"
#include <stdlib.h>


//全局变量
GtkWidget* window_main;                                                               	//用户登录窗口
GtkWidget* root_win;                                                                  	//管理员登录窗口
GtkWidget* window_user_registered;                                                    	//用户注册窗口
GtkWidget* window_root_registered;                                                    	//管理员注册窗口
GtkWidget* root_logged_win;                                                           	//管理员登录界面
GtkWidget* user_logged_win;                                                           	//用户登录界面
GtkWidget* entry_user_id;                                                             	//用户id
GtkWidget* entry_user_passwd;                                                         	//用户密码
GtkWidget* entry_root_id;                                                             	//管理员id
GtkWidget* entry_root_passwd;                                                         	//管理员密码
GtkWidget* window_buy;                                                                	//支付窗口
GtkWidget* window_recharge;																				//充值窗口
GtkWidget *entry_charge_amount;		///																	//充值金额
GtkWidget *entry_charge_passwd;																			//充值密码
GtkWidget* window_recharge_success;																		//充值成功界面
GtkWidget* root_logged_mem_win;																			//会员管理窗口
GtkWidget* user_logged_book_win;																			//用户已购书单窗口
guint timer;																									//定时器
GtkWidget* label_sql;
GtkWidget *entry_user_reg_id;
GtkWidget *entry_user_reg_passwd;
GtkWidget *entry_root_reg_passwd;
GtkWidget *entry_root_reg_id;
GtkWidget *entry_add_book_name;
GtkWidget *entry_add_book_num;
GtkWidget *entry_add_book_money;
const char *name_id;
GtkWidget *label_left_money; //剩余金额
GtkWidget *entry_vip_input_id;
GtkWidget *label_sql_vip;
GtkWidget *entry_user_book_name_id;
GtkWidget *label_book_store_tips;
GtkWidget *entry_user_tel; //用户手机
GtkWidget *entry_user_passwd_again; //用户确认密码
GtkWidget *entry_root_tel; //root手机
GtkWidget *entry_root_passwd_again; //root确认密码
GtkWidget *entry_invite; //内推码




//声明外部可用
extern void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button);
extern void window_main_gtk(GtkButton *button, gpointer user_data);							//用户登录
extern void main_to_root(GtkButton *button, gpointer user_data);								//管理员登录
extern void user_registered_gtk(GtkButton *button, gpointer user_data);						//用户注册界面
extern void root_registered_gtk(GtkButton *button, gpointer user_data);						//管理员注册
extern void root_to_logged(GtkButton *button, gpointer user_data);							//管理员登录界面
extern void user_to_logged(GtkButton *button, gpointer user_data);							//用户登录界面
extern void rootClear( GtkWidget *button, gpointer entry );                            //行编辑清除
extern void userClear( GtkWidget *button, gpointer entry );										//行编辑清除
//extern void window_buy_gtk(GtkButton *button, gpointer user_data);							//购买界面
extern void window_recharge_gtk(GtkButton *button, gpointer user_data);						//充值界面
extern void window_pay_success_gtk(GtkButton *button, gpointer user_data);					//支付成功
extern void show_only_one( GtkWidget *button, gpointer entry );								//
extern void get_id(GtkButton *button,gpointer user_data);										//获取账号id
extern void get_username(GtkButton *button,gpointer user_data);							   //获取会员名
extern void get_password(GtkButton *button,gpointer user_data);                        //获取密码
extern void get_confirm(GtkButton *button,gpointer user_data);                         //获取确认密码
extern void get_code(GtkButton *button,gpointer user_data);                            //
extern void auth_insert(GtkButton *button,gpointer user_data);                         //管理员数据库添加
extern void auth_login(GtkButton *button,gpointer user_data);                          //数据库管理员登录
extern void root_to_logged_mem(GtkButton *button, gpointer user_data);                 //会员管理界面
extern void user_to_logged_book(GtkButton *button, gpointer user_data);                 //用户已购书单界面
extern void do_user_register(GtkButton *button,gpointer user_data);
extern void do_admin_register(GtkButton *button,gpointer user_data);
extern void do_user_log_in(GtkButton* button,gpointer user_data);
extern void do_admin_log_in(GtkButton* button,gpointer user_data);
extern void book_add(GtkButton *button,gpointer user_data);
extern void book_del(GtkButton *button,gpointer user_data);
extern void book_recharg(GtkButton *button,gpointer user_data);
extern void user_del(GtkButton *button,gpointer user_data);
extern void user_pay_book(GtkButton *button,gpointer user_data);
extern void load_image(GtkWidget *image, const char *img_path, const int width, const int height );
extern void fflush_one(GtkButton *button, gpointer user_data);
extern void fflush_two(GtkButton *button, gpointer user_data);
extern void fflush_three(GtkButton *button, gpointer user_data);



#endif