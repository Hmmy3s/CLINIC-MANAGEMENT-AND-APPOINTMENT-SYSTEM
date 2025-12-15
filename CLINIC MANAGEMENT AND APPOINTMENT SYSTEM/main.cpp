#include "database.h"
#include"ADMIN.h"

//global vars
int padding = 25;
string username;

int main() {

	//create object
    

    //create connection
    sql::Connection* con = db_test_con("tcp://127.0.0.1:3306", "root", "");

    //start
	main_menu(con);

	/**Admin admin;
	admin.display_staff_table(con); **/

    delete con;

    return 0;
}