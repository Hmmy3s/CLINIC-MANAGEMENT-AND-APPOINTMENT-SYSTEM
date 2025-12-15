#ifndef ADMIN_H
#define ADMIN_H

#include "database.h"
#include <mysql_connection.h>
using namespace std;

class Admin {
private:
	string table;
	string username;
	string admin_id;
	int dashboard_opt;
	int padding;

public:
	//constructor && destructor
	Admin();
	~Admin();

	//methods
	bool display_login_menu(sql::Connection* con);
	bool display_admin_dashboard(sql::Connection* con);
	void admin_dashboard_options();
	void create_staff_acc(sql::Connection* con, const string& admin_id);
	void delete_staff_acc(sql::Connection* con);
	void display_staff_table(sql::Connection* con);
	void update_staff_shift(sql::Connection* con);

};

#endif // !ADMIN_H
