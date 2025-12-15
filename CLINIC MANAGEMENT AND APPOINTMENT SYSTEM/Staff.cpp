#include "Staff.h"

Staff::Staff()
{
	table = "staff";
	dashboard_opt = 5;
	padding = 30;
}

Staff::~Staff(){}

bool Staff::display_login_menu(sql::Connection* con)
{
	cout << "Enter username: ";
	getline(cin, username);

	cout << "Enter password: ";
	bool x = check_credentials(con, table, username, get_password());

	if (x) {

		//get staff id
		string query = "SELECT Staff_ID FROM staff WHERE USERNAME = ?";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setString(1, username);
		sql::ResultSet* res = pstmt->executeQuery();
		res->next();
		staff_id = res->getString("Staff_ID");
		delete res;
		delete pstmt;

		//login succesful
		clear_screen();
		cout << "\n\n";
		set_padding(padding);
		cout << "Login successful! Press Enter to continue...";
		pause_screen();

		return x;
	}
	else {
		//login failed
		clear_screen();
		cout << "\n\n";
		set_padding(padding);
		cout << "Invalid credentials. Press Enter to return...";
		pause_screen();

		return x;
	}
}

void Staff::staff_dashboard_options()
{
	cout << "\n";
	set_padding(padding);
	cout << "1. VIEW PATIENT QUEUE\n";
	set_padding(padding);
	cout << "2. MARK PATIENT AS DONE\n";
	set_padding(padding);
	cout << "3. MARK PATIENT AS LATE\n";
	set_padding(padding);
	cout << "4. UPDATE PASSWORD\n";
	
}
