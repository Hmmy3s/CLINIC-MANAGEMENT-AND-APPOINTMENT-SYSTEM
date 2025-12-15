#include "database.h"
#include "ADMIN.h"

sql::Connection* db_test_con(const string& server, const string& username, const string& password)
{
	sql::Driver* driver;
	sql::Connection* con;

	try {
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException& e) {
		cout << "Could not connect to server. Error message" << e.what() << endl;
		system("pause");
		exit(1);
	}

	con->setSchema("clinic_db");
	cout << "Database connection successful!" << endl;
	cout << "Press Enter to continue and clear the screen..." << endl;
	cin.get();

	return con;
}

bool check_credentials(sql::Connection* con, const string& table, const string& username, const string& password) {
	try {
		string query = "SELECT COUNT(*) FROM `" + table + "` WHERE  `USERNAME` = ? AND `PASSWORD#` = ?";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setString(1, username);
		pstmt->setString(2, password);

		sql::ResultSet* res = pstmt->executeQuery();
		res->next();
		int count = res->getInt(1);

		delete res;
		delete pstmt;

		return count > 0;

	}
	catch (sql::SQLException& e) {
		get_error_message(e);
	}
}

//main menu options
void display_menu_options() {

	cout << "\n";
	set_padding(25);
	cout << "1. ADMIN LOGIN" << endl;

	set_padding(25);
	cout << "2. STAFF LOGIN" << endl;

	set_padding(25);
	cout << "3. NEW USER REGISTER" << endl;

	set_padding(25);
	cout << "4. USER LOGIN" << endl;

	set_padding(25);
	cout << "5. EXIT" << endl;
}

//use for title
void printCenteredTitle(const string& title, int boxWidth) {

	int consoleWidth = 80;
	int titlePadding = (boxWidth - title.length() - 2) / 2;
	int boxPadding = (consoleWidth - boxWidth) / 2;

	if (boxWidth <= title.length()) {
		boxWidth = title.length() + 2;
	}

	for (int i = 0; i < boxPadding; i++) cout << " ";
	cout << "+";
	for (int i = 0; i < boxWidth - 2; i++) cout << "-";
	cout << "+" << endl;
	cout << "";

	for (int i = 0; i < boxPadding; i++) cout << " ";
	cout << "|";

	for (int i = 0; i < titlePadding; i++) cout << " ";
	cout << title;
	for (int i = 0; i < (boxWidth - title.length() - titlePadding - 2); i++) cout << " ";

	cout << "|" << endl;
	cout << "";

	for (int i = 0; i < boxPadding; i++) cout << " ";
	cout << "+";
	for (int i = 0; i < boxWidth - 2; i++) cout << "-";
	cout << "+" << endl;

}

int get_options(const int max_options)
{
	int opt;

	do {
		cout << "\n";
		set_padding(25);
		cout << "Enter your option (1 to " << max_options << "): ";
		cin >> opt;

		if (opt < 1 || opt > max_options) {
			set_padding(25);
			cout << "Invalid option. Please enter a number between 1 and " << max_options << "." << endl;
		}
	} while (opt < 1 || opt > max_options);

	return opt;
}

void clear_screen()
{
	system("cls");
}

void set_padding(const int padding)
{
	for (int i = 0; i < padding; i++) cout << " ";
}

void set_line(const int width)
{
	for (int i = 0; i < width; i++) cout << "-";
	cout << "+";
}

void pause_screen()
{
	cin.ignore();
	cin.get();
}

void get_error_message(sql::SQLException& e)
{
	clear_screen();
	cout << "\n\n";
	set_padding(25);
	cout << "Error message: " << e.what() << endl;
	pause_screen();
	exit(1);
}


void main_menu(sql::Connection* con)
{
	while (true) {
		clear_screen();
		printCenteredTitle("MAIN MENU", 40);
		display_menu_options();
		int x = get_options(5);

		switch (x) {
		case 1: {
			clear_screen();
			// create object here
			Admin admin;

			for (int i = 0; i <= 3; i++) {
				bool log = admin.display_login_menu(con);
				if (log) {
					clear_screen();
					set_padding(25);
					cout << "Welcome, admin! Press Enter to continue...";
					pause_screen();

					bool h = admin.display_admin_dashboard(con);
					if (h == false) {
						break;
					}
				}

				clear_screen();
				cout << "\n\n";
				set_padding(25);
				cout << "U have " << (3 - i) << " login attempts left.";
				if (i == 2) {
					clear_screen();
					set_padding(25);
					cout << "Maximum login attempts reached. Returning to main menu...";
					pause_screen();
				}
				else {
					cout << "\n\n";
					set_padding(25);
					cout << "Press Enter to retry login...";
					pause_screen();
				}
			}
			break;
		}
		case 2:
			clear_screen();
			cout << "test_2";  // Placeholder for Staff
			break;
		case 3:
			clear_screen();
			cout << "test_3";  // Placeholder for Patient
			break;
		case 4:
			clear_screen();
			cout << "test_4";  // Placeholder for Patient
			break;
		default:
			clear_screen();
			cout << "Exiting program...";
			delete con;
			exit(0);
		}
	}
}

string get_password() {
	string password;
	char cha;

	while (true) {
		cha = _getch();  // Read a single character 

		if (cha == 13) { // Press Enter
			break;
		}
		else if (cha == 8) { // Handle backspace
			if (!password.empty()) {
				password.pop_back();
				cout << "\b \b"; // Remove character
			}
		}
		else {
			password.push_back(cha);
			cout << "#"; // Mask password 
		}
	}

	cout << "\n";
	return password;
}


string generate_id(sql::Connection* con, const string& table_name)
{
	//select the prefix based on table name
	string col, new_id, lastID;
	if (table_name == "staff") {
		col = "Staff_ID";
		new_id = "ST_";
	}
	else if (table_name == "admin") {
		col = "Admin_ID";			
		new_id = "AD_";
	}
	else {
		col = "Patient_ID";
		new_id = "PT_";
	}
		try {
			string query = "SELECT `" + col + "` FROM " + table_name + " ORDER BY " + col + " DESC LIMIT 1";
			sql::PreparedStatement* pstmt = con->prepareStatement(query);
			sql::ResultSet* res = pstmt->executeQuery();

			//normal case
			if (res->next()) {
				lastID = res->getString(col);
			}

			//if table empty
			else {
				new_id = new_id + "0000";
				delete res;
				delete pstmt;
				return new_id;
			}
			

			string num_part = lastID.substr(3);
			int num_id = stoi(num_part);
			int new_num_id = num_id + 1;
			string new_num_part = to_string(new_num_id);

			int add_zero = 4 - new_num_part.length();
			new_id.append(add_zero, '0');
			new_id.append(new_num_part);

			delete res;
			delete pstmt;

			return new_id;
		}
		catch (sql::SQLException& e) {
			clear_screen();
			cout << "\n\n";
			set_padding(25);
			cout << "Error message: " << e.what() << endl;
			pause_screen();
			exit(1);
		}
}