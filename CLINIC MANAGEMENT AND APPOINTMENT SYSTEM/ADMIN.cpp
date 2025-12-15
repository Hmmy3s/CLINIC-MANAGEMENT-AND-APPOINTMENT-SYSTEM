#include "ADMIN.h"

Admin::Admin()
{
    table = "admin";
    dashboard_opt = 8;
    padding = 30;
}

Admin::~Admin(){}

bool Admin::display_login_menu(sql::Connection* con)
{
    clear_screen();
    printCenteredTitle("LOGIN MENU", 40);
    cout << "\n";

    set_padding(padding); //username
    cout << "USERNAME: ";
    getline(cin, username);
    cout << "\n";

    set_padding(padding); //password
    cout << "PASSWORD: ";
    bool x = check_credentials(con, table, username, get_password());

    if (x) {

		//get admin id
		string query = "SELECT Admin_ID FROM admin WHERE USERNAME = ?";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setString(1, username);
		sql::ResultSet* res = pstmt->executeQuery();
		res->next();
        admin_id = res->getString("Admin_ID");
		delete res;
		delete pstmt;

        clear_screen();
        cout << "\n\n";
        set_padding(padding);
        cout << "Login successful! Press Enter to continue...";
        pause_screen();
        return x;
    }
    else {
        clear_screen();
        cout << "\n\n";
        set_padding(padding);
        cout << "Invalid credentials. Press Enter to return...";
        pause_screen();
        return x;
    }
}

bool Admin::display_admin_dashboard(sql::Connection* con)
{
    bool stay = true;
    while (stay) {
        clear_screen();
        printCenteredTitle("ADMIN DASHBOARD", 40);
        admin_dashboard_options();
        int x = get_options(dashboard_opt);

        switch (x) {
        case 1:
            clear_screen();
            create_staff_acc(con, admin_id);
            pause_screen();
            break;

        case 2:
            clear_screen();
            cout << "\n";
            set_padding(padding);
			delete_staff_acc(con);
            pause_screen();
            break;

        case 3:
            clear_screen();
            cout << "\n";
            set_padding(padding);
			display_staff_table(con);
            cout << "\n\n";
            set_padding(padding);
            cout << "press enter to return to dashboard";
            pause_screen();
            break;

        case 4:
            clear_screen();
            cout << "\n";
            set_padding(padding);
            update_staff_shift(con);
            break;

        case 5:
            clear_screen();
            cout << "\n";
            set_padding(padding);
            cout << "repot 1";
            pause_screen();
            break;

        case 6:
            clear_screen();
            cout << "\n";
            set_padding(padding);
            cout << "report 2";
            pause_screen();
            break;

        case 7:
            clear_screen();
            cout << "\n";
            set_padding(padding);
            cout << "returning to main menu";
            pause_screen();
            stay = false;
            break;

        default:
            clear_screen();
            cout << "\n";
            set_padding(padding);
            cout << "BYE BYE";
            delete con;
			exit(0);
        }
    }
    return stay;
}

void Admin::admin_dashboard_options()
{
    cout << "\n";
	set_padding(padding);
    cout << "1. CREATE STAFF ACCOUNT\n" ;

    set_padding(padding);
    cout << "2. DELETE STAFF ACCOUNT\n";

    set_padding(padding);
    cout << "3 VIEW CURRENT STAFF\n";

    set_padding(padding);
    cout << "4. CHANGE STAFF SHIFT\n";

    set_padding(padding);
    cout << "5. GENERATE WEEKLY REPORT\n";

    set_padding(padding);
    cout << "6. GENERATE MONTHLY REPORT\n";

    set_padding(padding);
    cout << "7. RETURN TO MAIN MENU\n";

    set_padding(padding);
    cout << "8. EXIT\n";

}

void Admin::create_staff_acc(sql::Connection* con, const string& admin_id)
{
    string name, phone_number, email, username, query;
    string staff_id = generate_id(con, "staff");

    try {
        cout << endl;
        set_padding(padding);
        cout << "STAFF NAME: ";
        getline(cin, name);

        cout << endl;
        set_padding(padding);
        cout << "STAFF PHONE NO: ";
        getline(cin, phone_number);  

        cout << endl;
        set_padding(padding);
        cout << "STAFF EMAIL: ";
        getline(cin, email); 

        cout << endl;
        set_padding(padding);
        cout << "STAFF USERNAME: ";
        getline(cin, username);

        query = "INSERT INTO `staff`  (Staff_ID, Admin_ID, Name, Email, Username, PhoneNumber)VALUES(?,?,?,?,?,?)";
        sql::PreparedStatement* pstmt = con->prepareStatement(query);
        pstmt->setString(1, staff_id);
        pstmt->setString(2, admin_id);
        pstmt->setString(3, name);
        pstmt->setString(4, email);
        pstmt->setString(5, username);
        pstmt->setString(6, phone_number);
        pstmt->executeQuery();

        clear_screen();
        cout << endl;
        set_padding(padding);
        cout << "ACCOUNT SUCCESFULLY CREATED!!\n";
    }
    catch (sql::SQLException& e) {
        get_error_message(e);
    }
}

void Admin::delete_staff_acc(sql::Connection* con)
{
	string staff_id, query;

	cout << endl;
	set_padding(padding);
	cout << "ENTER STAFF ID TO DELETE: ";
	getline(cin, staff_id);

    try {
        query = "DELETE FROM `queue` WHERE Staff_ID = ?";
        sql::PreparedStatement* pstmt = con->prepareStatement(query);
        pstmt->setString(1, staff_id);
        pstmt->executeUpdate();
        delete pstmt;

        query = "DELETE FROM `staff` WHERE Staff_ID = ?";
        pstmt = con->prepareStatement(query);
        pstmt->setString(1, staff_id);

        int affected_row = pstmt->executeUpdate();
        delete pstmt;

        if (affected_row > 0) {
            clear_screen();
            cout << endl;
            set_padding(padding);
            cout << "ACCOUNT SUCCESSFULLY DELETED!!\n";
        }
        else {
            clear_screen();
            cout << endl;
            set_padding(padding);
            cout << "NO ACCOUNT FOUND WITH THE GIVEN STAFF ID.\n";
        }
    }
    catch (sql::SQLException& e) {
        get_error_message(e);
	}

}

void Admin::display_staff_table(sql::Connection* con)
{
    try {
        //vars related
        const int col = 6;
        string info, query = "SELECT * FROM staff";
        string col_title[col] = { "Staff_ID", "Name", "Email", "PhoneNumber", "ShiftStartTime", "ShiftEndTime" };
        int col_width[col] = { 0 };

        sql::PreparedStatement* pstmt = con->prepareStatement(query);
        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next()) {

            //get col length 
            while (res->next()) {
                for (int i = 0; i < col; i++) {
                    info = res->getString(col_title[i]);
                    int info_length = info.length();
					col_width[i] = col_title[i].length();
                    if (col_width[i] < info_length) {
                        col_width[i] = info.length() + 2;
                    }
                }
            }

			clear_screen();

            //display table colum title  
            cout << "+";
            for (int i = 0; i < col; i++) set_line(col_width[i] + 4);
			cout << "\n";

            cout << "|";
            for (int i = 0; i < col; i++) {
                set_padding(2);
                cout << col_title[i];
                set_padding(col_width[i] - col_title[i].length() + 2);
                cout << "|";
            }

            cout << "\n";
			cout << "+";
            for (int i = 0; i < col; i++) set_line(col_width[i] + 4);
            cout << "\n";

            delete res;
            res = pstmt->executeQuery();

            //display each roe
            while (res->next()) {
                cout << "|";
                for (int i = 0; i < col; i++) {
                    info = res->getString(col_title[i]);
                    set_padding(2);
                    if (info == "00:00:00") {
                        info = "N/A";
                        cout << info;
                    }
                    else {
                        cout << info;
                    }
                    set_padding(col_width[i] - info.length() + 2);
                    cout << "|";
                }
				cout << "\n";
            }

			cout << "+";
            for (int i = 0; i < col; i++) set_line(col_width[i] + 4);

            delete res;
            delete pstmt;

        }
        //if table empty
        else {
            clear_screen();
            cout << "\n\n";
            set_padding(padding);
            cout << "NO STAFF ACCOUNT AVAILABLE TO DISPLAY.\n";
            set_padding(padding);
            cout << "press enter to return...";
        }
    }
    catch (sql::SQLException& e) {
        get_error_message(e);
	}
}

void Admin::update_staff_shift(sql::Connection* con)
{
    try {
        string staff_id, shift_start, shift_end, query;
        display_staff_table(con);

        cout << "\n\n\n";
        set_padding(padding);
        cout << "Enter the staff_ID of the staff u want to change: ";
        cin >> staff_id;

        cout << "\n";
        set_padding(padding);
        cout << "Enter new shift start time (hh:mm:ss) : ";
        cin >> shift_start;

        cout << "\n";
        set_padding(padding);
        cout << "Enter new shift end time (hh:mm:ss) : ";
        cin >> shift_end;

        query = "SELECT COUNT(*) FROM staff WHERE Staff_ID = ?";
        sql::PreparedStatement* pstmt = con->prepareStatement(query);
        pstmt->setString(1, staff_id);
        sql::ResultSet* res = pstmt->executeQuery();
        res->next();
        int count = res->getInt(1);
        delete res;
        delete pstmt;

        if (count > 0) {
            query = "UPDATE staff SET ShiftStartTime = ?, ShiftEndTime = ? WHERE Staff_ID = ?";
            pstmt = con->prepareStatement(query);
            pstmt->setString(1, shift_start);
            pstmt->setString(2, shift_end);
            pstmt->setString(3, staff_id);
            pstmt->executeUpdate();
            clear_screen();

            cout << "\n\n\n";
            set_padding(padding);
            cout << "UPDATED STAFF SUCCESFULLY!!!\n";

            set_padding(padding);
            cout << "Press enter to return to main menu ......";

            pause_screen();
        }
        else {
            clear_screen();
            cout << "\n\n\n";
            set_padding(padding);
            cout << "STAFF ID DOESNT EXIST. UPDATE FAILED!!!\n";

            set_padding(padding);
            cout << "Press enter to return to main menu ......";
        }
    }
    catch (sql::SQLException& e) {
        get_error_message(e);
    }
}
