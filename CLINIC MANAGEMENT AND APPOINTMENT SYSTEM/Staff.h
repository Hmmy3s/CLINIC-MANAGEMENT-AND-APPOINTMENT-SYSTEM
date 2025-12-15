#ifndef STAFF_H
#define STAFF_H

#include "database.h"

class Staff
{
	private:
		string staff_id;
		string table;
		string username;
		int dashboard_opt;
		int padding;

    public:

		//constructor && destructor
		Staff();
		~Staff();

		//methods
		bool display_login_menu(sql::Connection* con);
		bool display_staff_dashboard(sql::Connection* con);
		void staff_dashboard_options();
		void view_queue(sql::Connection* con);
		void mark_patient_done(sql::Connection* con);


};

#endif // !STAFF_H