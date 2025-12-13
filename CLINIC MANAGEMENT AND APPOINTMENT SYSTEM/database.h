#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <conio.h>
#include <cstdlib>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

sql::Connection* db_test_con(const string& server, const string& username, const string& password);
bool check_credentials(sql::Connection* con, const string& table, const string& username, const string& password);


void display_menu_options();
void main_menu(sql::Connection* con);

void printCenteredTitle(const string& title, int boxWidth);
int get_options(const int max_options);
void clear_screen();
void set_padding(const int padding);
void set_line(const int width);
void pause_screen();

string get_password();
string generate_id(sql::Connection* con, const string& table_name);

#endif // DATABASE_H