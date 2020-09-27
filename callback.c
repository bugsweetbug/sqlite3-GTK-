#include "gtk_win.h"

void userClear( GtkWidget *button, gpointer entry )
{
	gtk_entry_set_text( GTK_ENTRY(entry_user_id), "");
	gtk_entry_set_text( GTK_ENTRY(entry_user_passwd), "");

}

void rootClear( GtkWidget *button, gpointer entry )
{
	gtk_entry_set_text( GTK_ENTRY(entry_root_id), "");
	gtk_entry_set_text( GTK_ENTRY(entry_root_passwd), "");

}

void show_only_one( GtkWidget *button, gpointer entry )
{
	char *str = (char *)entry;
	gtk_widget_hide_all(window_main);
	gtk_widget_hide_all(window_user_registered);
	gtk_widget_hide_all(root_win);
	gtk_widget_hide_all(root_logged_win);
	gtk_widget_hide_all(window_root_registered);
	gtk_widget_hide_all(user_logged_win);
	gtk_widget_hide_all(window_buy);
	gtk_widget_hide_all(window_recharge);
	gtk_widget_hide_all(window_recharge_success);
	gtk_widget_hide_all(root_logged_mem_win);
	gtk_widget_hide_all(user_logged_book_win);




	if(strcmp(str,"window_main") == 0)
		{
			gtk_widget_show_all(window_main);
		}
	else if(strcmp(str,"window_user_registered") == 0)
		{
			gtk_widget_show_all(window_user_registered);
		}
	else if(strcmp(str,"root_win") == 0)
		{
			gtk_widget_show_all(root_win);
		}
	else if(strcmp(str,"root_logged_win") == 0)
		{
			gtk_widget_show_all(root_logged_win);
		}
	else if(strcmp(str,"window_root_registered") == 0)
		{
			gtk_widget_show_all(window_root_registered);
		}
	else if(strcmp(str,"user_logged_win") == 0)
		{
			gtk_widget_show_all(user_logged_win);
		}
	else if(strcmp(str,"window_buy") == 0)
		{
			gtk_widget_show_all(window_buy);
		}
	else if(strcmp(str,"window_recharge") == 0)
		{
			gtk_widget_show_all(window_recharge);
		}
	else if(strcmp(str,"window_recharge_success") == 0)
		{
			gtk_widget_show_all(window_recharge_success);
		}
	else if(strcmp(str,"root_logged_mem_win") == 0)
		{
			gtk_widget_show_all(root_logged_mem_win);
		}
	else if(strcmp(str,"user_logged_book_win") == 0)
		{
			gtk_widget_show_all(user_logged_book_win);
		}

}
