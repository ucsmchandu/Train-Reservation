#include <iostream>
#include <sstream>
#include <mysql.h>
#include <mysqld_error.h>
#include <conio.h>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <unordered_map>
#include <thread> // For std::this_thread::sleep_for
#include <chrono>
const char *host = "localhost";
const char *user = "root";
const char *pass = "Chandu123!@#";
using namespace std;
// class
class train
{
private:
	MYSQL *obj;
	MYSQL *conn;
	int bank_number;
	// these are inside a password function
	void change_password();
	void close_acc1();
	void close_acc2();

public:
	train()
	{
	}
	// these are main functions
	void create_account();
	void login();
	void forget_password();
	void booking_ticket();
	void cancel_tickets();
	void tickect_histroy();
	void close_account();
};

// closing account
// deleteing all transaction
void train::close_account()
{
	string phone, acc_pin, from_pin, from_phone;
	bool found = false;
	bool found_acc = false;
	obj = mysql_init(nullptr);
	system("cls");
	if (!mysql_real_connect(obj, host, user, pass, "trainapp", 3306, NULL, 0))
	{
		cout << "Error :" << mysql_error(obj) << endl;
	}
	else
	{
		do
		{
			cout << "Enter phone number :";
			cin >> phone;
			if (phone.length() != 10)
			{
				cout << "Inavlid phone number!" << endl;
				found = false;
				continue;
			}
			for (size_t i = 0; i < phone.length(); i++)
			{
				if (!isdigit(phone[i]))
				{
					cout << "Inavalid phone number!" << endl;
					found = false;
				}
				else
				{
					found = true;
				}
			}
		} while (!found);
		cout << "Enter account pin :";
		cin >> acc_pin;
		string line = "select phone,password from appaccount where phone='" + phone + "' and password='" + acc_pin + "'";
		if (mysql_query(obj, line.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res = mysql_store_result(obj);
			MYSQL_ROW row;
			if (res)
			{
				while (row = mysql_fetch_row(res))
				{
					from_phone = row[0];
					from_pin = row[1];
					found_acc = true;
				}
			}
			else
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
			if (found_acc == true)
			{
				if (from_phone == phone && from_pin == acc_pin)
				{
					string word = "delete from appaccount where phone='" + phone + "' and password='" + acc_pin + "'";
					if (mysql_query(obj, word.c_str()))
					{
						cout << "Error :" << mysql_error(obj) << endl;
					}
					else
					{
						if (mysql_affected_rows(obj) > 0)
						{
							cout << "Account deleted successfull" << endl;
							this_thread::sleep_for(chrono::milliseconds(4000));
						}
					}
				}
			}
			else
			{
				cout << "Account not founded!" << endl;
				this_thread::sleep_for(chrono::milliseconds(4000));
			}
		}
	}
	mysql_close(obj);
}

// chnaging pass
void train::change_password()
{
	string acc_num, phone, aadhar, new_pin, new_pinn;
	bool mobile = false;
	bool found_aadhar = false;
	bool found = false;
	char ch;
	do
	{
		cout << "Enter phone number :";
		cin >> phone;
		if (phone.length() != 10)
		{
			cout << "Invalid phone number!" << endl;
			mobile = false;
			continue;
		}
		for (size_t i = 0; i < phone.length(); i++)
		{
			if (!isdigit(phone[i]))
			{
				cout << "Invalid phone number!" << endl;
				mobile = false;
			}
			else
			{
				mobile = true;
			}
		}
	} while (!mobile);
	do
	{
		cout << "Enter aadhar number :";
		cin >> aadhar;
		if (aadhar.length() != 12)
		{
			cout << "Invalid aadhar number!" << endl;
			found_aadhar = false;
			continue;
		}
		for (size_t i = 0; i < aadhar.length(); i++)
		{
			if (!isdigit(aadhar[i]))
			{
				cout << "Invalid aadhar number!" << endl;
				found_aadhar = false;
			}
			else
			{
				found_aadhar = true;
			}
		}
	} while (!found_aadhar);
	cout << "Enter account number :";
	cin >> acc_num;
	obj = mysql_init(nullptr);
	if (!mysql_real_connect(obj, host, user, pass, "trainapp", 3306, NULL, 0))
	{
		cout << "Error :" << mysql_error(obj) << endl;
	}
	else
	{
		string found_pin = "select password from appaccount where phone='" + phone + "' and aadhar='" + aadhar + "' and bank_number='" + acc_num + "' ";
		if (mysql_query(obj, found_pin.c_str()))
		{
			cout << "Error:" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res = mysql_store_result(obj);
			MYSQL_ROW row;
			if (res)
			{
				while (row = mysql_fetch_row(res))
				{
					found = true;
				}
			}
		}
		if (found != true)
		{
			cout << "Incorrect details (or) Account doesn't exist !" << endl;
		}
		else
		{
			do
			{
				cout << "Enter new pin :";
				while (true)
				{
					ch = _getch();
					if (ch == '\r')
					{
						break;
					}
					else if (ch == '\b')
					{
						if (!new_pin.empty())
						{
							new_pin.pop_back();
							cout << "\b \b";
						}
					}
					else
					{
						new_pin += ch;
						cout << "*";
					}
				}
				cout << "Enter again:";
				while (true)
				{
					ch = _getch();
					if (ch == '\r')
					{
						break;
					}
					else if (ch == '\b')
					{
						if (!new_pinn.empty())
						{
							new_pinn.pop_back();
							cout << "\b \b";
						}
					}
					else
					{
						new_pinn += ch;
						cout << "*";
					}
				}
			} while (new_pin != new_pinn);
			cout << endl;
			string update_pin = "update appaccount set password='" + new_pin + "' where bank_number='" + acc_num + "' and phone='" + phone + "' and aadhar='" + aadhar + "' ";
			if (mysql_query(obj, update_pin.c_str()))
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
			else
			{
				cout << "Pin changed successfully" << endl;
			}
		}
	}
}
// cancel tickets
void train::cancel_tickets()
{
	string phone, acc_num, date;
	bool mobile = false;
	bool found_phone = false;
	bool reciver1 = false;
	bool reciver2 = false;
	bool found1 = false;
	bool found2 = false;
	bool found_bill = false;
	bool found_acec = false;
	bool found_res = false;
	double bill_t = 0;
	system("cls");
	cout << "                                               Cancelling tickets" << endl;
	cout << "   NOTICE :if you cancelled the tickets half of the amount is debited in your account" << endl;
	do
	{
		cout << "Enter phone number :";
		cin >> phone;
		if (phone.length() != 10)
		{
			cout << "Invalid phone number!" << endl;
			mobile = false;
			continue;
		}
		for (size_t i = 0; i < phone.length(); i++)
		{
			if (!isdigit(phone[i]))
			{
				cout << "Invalid phone number!" << endl;
				mobile = false;
			}
			else
			{
				mobile = true;
			}
		}
	} while (!mobile);
	cout << "Enter account number :";
	cin >> acc_num;
	cout << "Enter date on which you to cancel tickets (y/m/d) enter carefully :";
	cin >> date;
	obj = mysql_init(nullptr);
	if (!mysql_real_connect(obj, host, user, pass, "trainapp", 3306, NULL, 0))
	{
		cout << "Error :" << mysql_error(obj) << endl;
	}
	else
	{
		string get_acc = "select bank_number from appaccount where bank_number='" + acc_num + "' and phone='" + phone + "' ";
		if (mysql_query(obj, get_acc.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res = mysql_store_result(obj);
			MYSQL_ROW row;
			if (res)
			{
				while (row = mysql_fetch_row(res))
				{
					found_acec = true;
				}
			}
			else
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
		}
		if (found_acec != true)
		{
			cout << "Account number not found!" << endl;
			this_thread::sleep_for(chrono::milliseconds(4000));
			login();
		}
		/////////////////////////////////////////////////////////////////////////////////////
		string get_det = "select phone from reservation where phone='" + phone + "' and date='" + date + "'";
		if (mysql_query(obj, get_det.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res = mysql_store_result(obj);
			MYSQL_ROW row;
			if (res)
			{
				while (row = mysql_fetch_row(res))
				{
					found_phone = true;
				}
			}
			else
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
		}
		if (found_phone != true)
		{
			cout << "No tickets found on this number!" << endl;
			this_thread::sleep_for(chrono::milliseconds(4000));
			login();
		}
		string get_bal = "select bill from reservation where phone='" + phone + "' and date='" + date + "'";
		if (mysql_query(obj, get_bal.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res = mysql_store_result(obj);
			MYSQL_ROW row;
			if (res)
			{
				while (row = mysql_fetch_row(res))
				{
					for (unsigned int i = 0; i < mysql_num_fields(res); i++)
					{
						found_bill = true;
						bill_t += stod(row[i]);
					}
				}
			}
			else
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
		}
	} // for tickets verrifiaction
	if (found_phone == true && found_bill == true)
	{
		string update_acc = "delete from reservation where phone='" + phone + "' and date='" + date + "'";
		if (mysql_query(obj, update_acc.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			if (mysql_affected_rows(obj) > 0)
			{
				found_res = true;
			}
		}
		if (found_res == true)
		{
			conn = mysql_init(nullptr);
			if (!mysql_real_connect(conn, host, user, pass, "bank", 3306, NULL, 0))
			{
				cout << "Error :" << mysql_error(conn) << endl;
			}
			else
			{
				string account1 = "select account_number from account where account_number='" + acc_num + "' and phone='" + phone + "'";
				if (mysql_query(conn, account1.c_str()))
				{
					cout << "Error :" << mysql_error(conn) << endl;
				}
				else
				{
					MYSQL_RES *res = mysql_store_result(conn);
					MYSQL_ROW row;
					if (res)
					{
						while (row = mysql_fetch_row(res))
						{
							found1 = true;
							reciver1 = true;
						}
					}
					else
					{
						cout << "Error :" << mysql_error(conn) << endl;
					}
				}
				string account2 = "select account_number from nmlaccount where account_number='" + acc_num + "' and phone='" + phone + "'";
				if (mysql_query(conn, account2.c_str()))
				{
					cout << "Error :" << mysql_error(conn) << endl;
				}
				else
				{
					MYSQL_RES *res = mysql_store_result(conn);
					MYSQL_ROW row;
					if (res)
					{
						while (row = mysql_fetch_row(res))
						{
							found1 = true;
							reciver1 = true;
						}
					}
					else
					{
						cout << "Error :" << mysql_error(conn) << endl;
					}
				}
				if (reciver1 == true && reciver2 == true)
				{
					if (found1 = true)
					{
						string update_acc1 = "update account set balance='" + to_string(bill_t / 2) + "' where account_number='" + acc_num + "' and phone='" + phone + "'";
						if (mysql_query(conn, update_acc1.c_str()))
						{
							if (mysql_affected_rows(conn) > 0)
							{
								cout << bill_t / 2 << " credited in your account" << endl;
							}
						}
						else
						{
							cout << "Error :" << mysql_error(conn) << endl;
						}
					}
					if (found2 = true)
					{
						string update_acc2 = "update nmlaccount set balance='" + to_string(bill_t / 2) + "' where account_number='" + acc_num + "' and phone='" + phone + "'";
						if (mysql_query(conn, update_acc2.c_str()))
						{
							if (mysql_affected_rows(conn) > 0)
							{
								cout << bill_t / 2 << " credited in your account" << endl;
							}
						}
						else
						{
							cout << "Error :" << mysql_error(conn) << endl;
						}
					}
				}
			}
		}
	}
} // function

// tickecct history
void train::tickect_histroy()
{
	string phone;
	bool mobile = false;
	bool found_phone = false;
	obj = mysql_init(nullptr);
	system("cls");
	cout << "                                                 History" << endl;
	do
	{
		cout << "Enter phone number :";
		cin >> phone;
		if (phone.length() != 10)
		{
			cout << "Invalid phone number!" << endl;
			mobile = false;
			continue;
		}
		for (size_t i = 0; i < phone.length(); i++)
		{
			if (!isdigit(phone[i]))
			{
				cout << "Invalid phone number!" << endl;
				mobile = false;
			}
			else
			{
				mobile = true;
			}
		}
	} while (!mobile);
	if (!mysql_real_connect(obj, host, user, pass, "trainapp", 3306, NULL, 0))
	{
		cout << "Error :" << mysql_error(obj) << endl;
	}
	else
	{
		string found = "select phone from reservation where phone='" + phone + "'";
		if (mysql_query(obj, found.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res = mysql_store_result(obj);
			MYSQL_ROW row;
			if (res)
			{
				while (row = mysql_fetch_row(res))
				{
					found_phone = true;
				}
			}
			else
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
		}
		if (found_phone != true)
		{
			cout << "No tickets booked on this number!" << endl;
		}
		else
		{
			string found_tickets = "select passenger_name,bill,date,village from reservation where phone='" + phone + "' ";
			if (mysql_query(obj, found_tickets.c_str()))
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
			else
			{
				MYSQL_RES *res = mysql_store_result(obj);
				MYSQL_ROW row;
				if (res)
				{
					while (row = mysql_fetch_row(res))
					{
						for (unsigned int i = 0; i < mysql_num_fields(res); i++)
						{
							if (row[i] != nullptr)
							{
								cout << "\t" << row[i];
							}
						}
						cout << endl;
					}
				}
				else
				{
					cout << "Error :" << mysql_error(obj) << endl;
				}
			}
		}
	}
	mysql_close(obj);
}
// booking code
void train::booking_ticket()
{
	string from, to, phone, name[100], date;
	int cost, h = 3, n, age[100], i;
	time_t now = time(nullptr);
	tm localtm;
	localtime_s(&localtm, &now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localtm);
	double balance;
	bool found = false;
	bool found_phone = false;
	bool found_route = false;
	bool found_mobile1 = false;
	bool found_mobile2 = false;
	bool reciver1 = false;
	bool reciver2 = false;
	bool equal_pin = false;
	system("cls");
	cout << "                                                   Booking tickets" << endl;
	unordered_map<string, unordered_map<string, int>> route = {
		{"Annavaram", {{"Tuni", 50}, {"Samalkot junction", 70}, {"Anaparti", 90}, {"Rajamundry", 110}, {"Nidadavolu junction", 130}, {"Tadepalligudem", 150}, {"Eluru", 170}, {"Nuzvid", 190}, {"Vijayawada", 210}}},
		{"Tuni", {{"Annavaram", 50}, {"samalkot junction", 70}, {"Anaparti", 90}, {"Rajamundry", 110}, {"Nidadavolu junction", 130}, {"Tadepalligudem", 150}, {"Eluru", 170}, {"Nuzvid", 190}, {"Vijayawada", 210}}},
		{"Samalkot junction", {{"Annavaram", 70}, {"Tuni", 50}, {"Anaparti", 90}, {"Rajamundry", 110}, {"Nidadavolu junction", 130}, {"Tadepalligudem", 150}, {"Eluru", 170}, {"Nuzvid", 190}, {"Vijayawada", 210}}},
		{"Anaparti", {{"Annavaram", 90}, {"Tuni", 50}, {"Samalkot junction", 90}, {"Rajamundry", 110}, {"Nidadavolu junction", 130}, {"Tadepalligudem", 150}, {"Eluru", 170}, {"Nuzvid", 190}, {"Vijayawada", 210}}},
		{"Rajamundry", {{"Annavaram", 110}, {"Tuni", 90}, {"Samalkot junction", 70}, {"Anaparti", 50}, {"Nidadavolu junction", 130}, {"Tadepalligudem", 150}, {"Eluru", 170}, {"Nuzvid", 190}, {"Vijayawada", 210}}},
		{"Nidadavolu junction", {{"Rajamundry", 50}, {"Anaparthi", 70}, {"Samalkot junction", 90}, {"Tuni", 110}, {"Annvaram", 130}, {"Tadepalligudem", 150}, {"Eluru", 170}, {"Nuzvid", 190}, {"Vijayawada", 210}}},
		{"Tadepalligudem", {{"Nidadavolu junction", 50}, {"Rajamundry", 70}, {"Anaparti", 90}, {"Samalkot junction", 110}, {"Tuni", 130}, {"Annavaram", 150}, {"Eluru", 170}, {"Nuzvid", 190}, {"Vijayawada", 210}}},
		{"Eluru", {{"Tadepalligudem", 50}, {"Nidadavolu junction", 70}, {"Rajamundry", 90}, {"Anaparti", 110}, {"Samalkot junction", 130}, {"Tuni", 150}, {"Annavaram", 170}, {"Nuzivid", 190}, {"Vijayawada", 210}}}};
	do
	{
		cout << "Available locations are:" << endl;
		for (const auto &location : route)
		{
			cout << location.first << endl;
		}
		cin.ignore();
		cout << "Enter the 'from' location :";
		getline(cin, from);
		cout << "Enter the 'to' location :";
		getline(cin, to);
		if (route.find(from) != route.end() && route[from].find(to) != route[from].end())
		{
			cost = route[from][to];
			found_route = true;
		}
		else
		{
			cout << "Invalid route selected or locations do not exist" << endl;
			cout << " " << endl;
			cout << " " << endl;
			cost = -1;
		}
		if (cost != -1)
		{
			cout << "The cost from " << from << " to " << to << " is : " << cost << " ruppes per penson" << endl;
		}
	} while (!found_route);
	cout << "Enter how many passengers :";
	cin >> n;
	cout << "Enter date to book tickets (y/m/d) :";
	cin >> date;
	do
	{
		cout << "Enter phone number for payment :";
		cin >> phone;
		if (phone.length() != 10)
		{
			cout << "Invalid phone number!" << endl;
			found_phone = false;
			continue;
		}
		for (size_t i = 0; i < phone.length(); i++)
		{
			if (!isdigit(phone[i]))
			{
				cout << "Invalid phone number!" << endl;
				found_phone = false;
			}
			else
			{
				found_phone = true;
			}
		}
	} while (!found_phone);
	for (i = 1; i <= n; i++)
	{
		cin.ignore();
		cout << i << " Enter name :";
		getline(cin, name[i]);
		cout << "Enter age :";
		cin >> age[i];
	}
	obj = mysql_init(nullptr);
	if (!mysql_real_connect(obj, host, user, pass, "bank", 3306, NULL, 0))
	{
		cout << "Error :" << mysql_error(obj) << endl;
	}
	else
	{
		string found_acc1 = "select phone from account where phone='" + phone + "'";
		if (mysql_query(obj, found_acc1.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res = mysql_store_result(obj);
			MYSQL_ROW row;
			if (res)
			{
				while (row = mysql_fetch_row(res))
				{
					found_mobile1 = true;
					reciver1 = true;
				}
			}
			else
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
		}
		string found_acc2 = "select phone from nmlaccount where phone='" + phone + "'";
		if (mysql_query(obj, found_acc2.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res = mysql_store_result(obj);
			MYSQL_ROW row;
			if (res)
			{
				while (row = mysql_fetch_row(res))
				{
					found_mobile2 = true;
					reciver2 = true;
				}
			}
			else
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
		}
		if (found_mobile1 != true && found_mobile2 != true)
		{
			cout << "No bank account linked with this number!" << endl;
			this_thread::sleep_for(chrono::milliseconds(3000));
		}
		else
		{
			int y;
			if (found_mobile1 == true && found_mobile2 == true)
			{
				cout << "Your phone number is linked with two bank accounts (Zero account and non zero account )" << endl;
				cout << "Enter '1' to pay with zero account :" << endl;
				cout << "Enter '2' to pay with Non-zero account" << endl;
				cin >> y;
			}
			if (y == 1)
			{
				// from zero account
				char ch;
				string acc_pin;
				do
				{
					acc_pin.clear();
					cout << "Enter account pin :";
					while (true)
					{
						ch = _getch();
						if (ch == '\r')
						{
							break;
						}
						else if (ch == '\b')
						{
							if (!acc_pin.empty())
							{
								acc_pin.pop_back();
								cout << "\b \b";
							}
						}
						else
						{
							acc_pin += ch;
							cout << "*";
						}
					}
					string from_pin = "select pin from account where phone='" + phone + "' and pin='" + acc_pin + "'";
					if (mysql_query(obj, from_pin.c_str()))
					{
						cout << "Error :" << mysql_error(obj) << endl;
					}
					else
					{
						MYSQL_RES *res = mysql_store_result(obj);
						MYSQL_ROW row;
						if (res)
						{
							while (row = mysql_fetch_row(res))
							{
								equal_pin = true;
							}
						}
						else
						{
							cout << "Error :" << mysql_error(obj) << endl;
						}
					}
					if (equal_pin != true)
					{
						cout << "Incorrect pin!" << endl;
						h--;
						if (h == 0)
						{
							cout << "Too many attempts!" << endl;
							this_thread::sleep_for(chrono::milliseconds(3000));
							break;
						}
						else
						{
							cout << h << " attempts remaining" << endl;
						}
					}
				} while (!equal_pin);
				cout << endl;
				string from_zero = "select balance from account where phone='" + phone + "'";
				if (mysql_query(obj, from_zero.c_str()))
				{
					cout << "Error :" << mysql_error(obj) << endl;
				}
				else
				{
					MYSQL_RES *res = mysql_store_result(obj);
					MYSQL_ROW row;
					if (res)
					{
						while (row = mysql_fetch_row(res))
						{
							balance = stod(row[0]);
						}
					}
					else
					{
						cout << "Error :" << mysql_error(obj) << endl;
					}
				}
				if (balance > 0 && balance > cost)
				{
					string update_money = "update account set balance=balance-'" + to_string(cost * n) + "' where phone='" + phone + "'";
					if (mysql_query(obj, update_money.c_str()))
					{
						cout << "Error :" << mysql_error(obj) << endl;
					}
					else
					{
						if (mysql_affected_rows(obj) > 0)
						{
							cout << "Payment successfull" << endl;
							cout << cost * n << "debited " << endl;
							cout << "Tickets booked" << endl;
							time_t now = time(nullptr);
							tm localtm;
							localtime_s(&localtm, &now);
							char buffer[80];
							strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localtm);
							string hist = to_string(cost) + " withdrawed from " + phone + " on " + buffer + " for railways";
							string update_history = "insert into history(transition_history,phone,timedate) values('" + hist + "','" + phone + "','" + buffer + "')";
							if (mysql_query(obj, update_history.c_str()))
							{
								cout << "Error :" << mysql_error(obj) << endl;
							}
							else
							{
								if (mysql_affected_rows(obj) > 0)
								{
									cout << "payment saved" << endl;
									cout << "Reseravtion successfull from " << from << " to " << to << " on " << date << endl;
									conn = mysql_init(nullptr);
									if (!mysql_real_connect(conn, host, user, pass, "trainapp", 3306, NULL, 0))
									{
										cout << "Error :" << mysql_error(conn) << endl;
									}
									else
									{
										string v = "from " + from + " to " + to;
										for (i = 1; i <= n; i++)
										{
											string reserve = "insert into reservation (passenger_name,age,phone,bill,date,village) values('" + name[i] + "','" + to_string(age[i]) + "','" + phone + "','" + to_string(cost) + "','" + date + "','" + v + "')";
											if (mysql_query(conn, reserve.c_str()))
											{
												cout << "Error :" << mysql_error(conn) << endl;
											}
										}
									}
									mysql_close(conn);
								}
							}
							this_thread::sleep_for(chrono::milliseconds(2000));
						}
					}
				}
				else
				{
					cout << "Insufficent balance!" << endl;
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
			}
			if (y == 2)
			{
				// from nml account
				char ch;
				string acc_pin;
				do
				{
					acc_pin.clear();
					cout << "Enter account pin :";
					while (true)
					{
						ch = _getch();
						if (ch == '\r')
						{
							break;
						}
						else if (ch == '\b')
						{
							if (!acc_pin.empty())
							{
								acc_pin.pop_back();
								cout << "\b \b";
							}
						}
						else
						{
							acc_pin += ch;
							cout << "*";
						}
					}
					string from_pin = "select pin from nmlaccount where phone='" + phone + "' and pin='" + acc_pin + "'";
					if (mysql_query(obj, from_pin.c_str()))
					{
						cout << "Error :" << mysql_error(obj) << endl;
					}
					else
					{
						MYSQL_RES *res = mysql_store_result(obj);
						MYSQL_ROW row;
						if (res)
						{
							while (row = mysql_fetch_row(res))
							{
								equal_pin = true;
							}
						}
						else
						{
							cout << "Error :" << mysql_error(obj) << endl;
						}
					}
					if (equal_pin != true)
					{
						cout << "Incorrect pin!" << endl;
						h--;
						if (h == 0)
						{
							cout << "Too many attempts!" << endl;
							this_thread::sleep_for(chrono::milliseconds(3000));
							break;
						}
						else
						{
							cout << h << " attempts remaining" << endl;
						}
					}
				} while (!equal_pin);
				cout << endl;
				string from_zero = "select balance from nmlaccount where phone='" + phone + "'";
				if (mysql_query(obj, from_zero.c_str()))
				{
					cout << "Error :" << mysql_error(obj) << endl;
				}
				else
				{
					MYSQL_RES *res = mysql_store_result(obj);
					MYSQL_ROW row;
					if (res)
					{
						while (row = mysql_fetch_row(res))
						{
							balance = stod(row[0]);
						}
					}
					else
					{
						cout << "Error :" << mysql_error(obj) << endl;
					}
				}
				if (balance > 1000 && balance > cost)
				{
					string update_money = "update nmlaccount set balance=balance-'" + to_string(cost * n) + "' where phone='" + phone + "'";
					if (mysql_query(obj, update_money.c_str()))
					{
						cout << "Error :" << mysql_error(obj) << endl;
					}
					else
					{
						if (mysql_affected_rows(obj) > 0)
						{
							cout << "Payment successfull" << endl;
							cout << cost * n << "debited " << endl;
							cout << "Tickets booked" << endl;
							time_t now = time(nullptr);
							tm localtm;
							localtime_s(&localtm, &now);
							char buffer[80];
							strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localtm);
							string hist = to_string(cost) + " withdrawed from " + phone + " on " + buffer + " for railways";
							string update_history = "insert into history(transition_history,phone,timedate) values('" + hist + "','" + phone + "','" + buffer + "')";
							if (mysql_query(obj, update_history.c_str()))
							{
								cout << "Error :" << mysql_error(obj) << endl;
							}
							else
							{
								if (mysql_affected_rows(obj) > 0)
								{
									cout << "payment saved" << endl;
									cout << "Reseravtion successfull from " << from << " to " << to << " on " << date << endl;
									conn = mysql_init(nullptr);
									if (!mysql_real_connect(conn, host, user, pass, "trainapp", 3306, NULL, 0))
									{
										cout << "Error :" << mysql_error(conn) << endl;
									}
									else
									{
										string v = "from " + from + " to " + to;
										for (i = 1; i <= n; i++)
										{
											string reserve = "insert into reservation (passenger_name,age,phone,bill,date,village) values('" + name[i] + "','" + to_string(age[i]) + "','" + phone + "','" + to_string(cost) + "','" + date + "','" + v + "')";
											if (mysql_query(conn, reserve.c_str()))
											{
												cout << "Error :" << mysql_error(conn) << endl;
											}
										}
									}
									mysql_close(conn);
								}
							}
						}
						this_thread::sleep_for(chrono::milliseconds(2000));
					}
				}
				else
				{
					cout << "Insufficent balance!" << endl;
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
			}
		}
	} // connection
	mysql_close(obj);
}

// changing login password
void train::forget_password()
{
	change_password();
}

// creating account on this app
void train::create_account()
{
	bool found = false;
	bool acc_found1 = false;
	bool acc_found2 = false;
	string name, phone, aadhar, dob;
	system("cls");
	string gmail, address;
	string password;
	// table name:appaccount name,phone,aadhar,address,dob,gmail,pass
	obj = mysql_init(nullptr);
	if (!mysql_real_connect(obj, host, user, pass, "trainapp", 3306, NULL, 0))
	{
		cout << "Error :" << mysql_error(obj) << endl;
	}
	else
	{
		cout << "                                            CREATING ACCOUNT     " << endl;
		// name,phone,aadhar,dob,bank_account_number
		cin.ignore();
		cout << "Enter name (Surname) :";
		getline(cin, name);
		do
		{
			cout << "Enter phone number :";
			cin >> phone;
			if (phone.length() != 10)
			{
				found = false;
				cout << "Invalid phone number !" << endl;
				continue;
			}
			else
			{
				for (size_t i = 0; i < phone.length(); i++)
				{
					if (!isdigit(phone[i]))
					{
						found = false;
						cout << "Invalid phone number !" << endl;
					}
					else
					{
						found = true;
					}
				}
			}
		} while (!found);
		do
		{
			cout << "Enter aadhar number :";
			cin >> aadhar;
			if (aadhar.length() != 12)
			{
				found = false;
				cout << "Invalid aadhar number !" << endl;
				continue;
			}
			for (size_t i = 0; i < aadhar.length(); i++)
			{
				if (!isdigit(aadhar[i]))
				{
					found = false;
					cout << "Invalid aadhar number !" << endl;
				}
				else
				{
					found = true;
				}
			}
		} while (!found);
		cin.ignore();
		cout << "Enter address :";
		getline(cin, address);
		cout << "Enter date of birth (day/month/year) :";
		cin >> dob;
		cout << "Enter gmail :";
		cin >> gmail;
		MYSQL *conn;
		conn = mysql_init(nullptr);
		if (!mysql_real_connect(conn, host, user, pass, "bank", 3306, NULL, 0))
		{
			cout << "Error :" << mysql_error(conn) << endl;
		}
		else
		{
			cout << "Enter bank account number :";
			cin >> bank_number;
			string found_acc = "select account_number from account where account_number='" + to_string(bank_number) + "'";
			if (mysql_query(conn, found_acc.c_str()))
			{
				cout << "Error :" << mysql_error(conn) << endl;
			}
			else
			{
				MYSQL_RES *res = mysql_store_result(conn);
				MYSQL_ROW row;
				if (res)
				{
					while (row = mysql_fetch_row(res))
					{
						acc_found1 = true;
					}
				}
				else
				{
					cout << "Error :" << mysql_error(conn) << endl;
				}
			}
			string found_acc2 = "select account_number from nmlaccount where account_number='" + to_string(bank_number) + "'";
			if (mysql_query(conn, found_acc.c_str()))
			{
				cout << "Error :" << mysql_error(conn) << endl;
			}
			else
			{
				MYSQL_RES *res = mysql_store_result(conn);
				MYSQL_ROW row;
				if (res)
				{
					while (row = mysql_fetch_row(res))
					{
						acc_found2 = true;
					}
				}
				else
				{
					cout << "Error :" << mysql_error(conn) << endl;
				}
			}
			if (acc_found1 != true && acc_found2 != true)
			{
				cout << "No bank account found!" << endl;
				this_thread::sleep_for(chrono::milliseconds(3000));
				login();
			}
			mysql_close(conn);
		}
		cout << "Set your password :";
		char ch;
		while (true)
		{
			ch = _getch();
			if (ch == '\r')
			{
				break;
			}
			else if (ch == '\b')
			{
				if (!password.empty())
				{
					password.pop_back();
					cout << "\b \b";
				}
			}
			else
			{
				password = password + ch;
				cout << "*";
			}
		}
		cout << endl;
		// inserting into database

		string line = "insert into appaccount (name,phone,aadhar,address,dob,gmail,bank_number,password) values ('" + name + "','" + phone + "','" + aadhar + "','" + address + "','" + dob + "','" + gmail + "','" + to_string(bank_number) + "','" + password + "')";
		if (mysql_query(obj, line.c_str()))
		{
			cout << "Account is already created on this number !" << endl;
		}
		else
		{
			cout << "Data inserted" << endl;
		}
	}
	mysql_close(obj);
}
// login into the app
void train::login()
{
	string name, phone, login_password;
	bool found = false;
	char ch;
	system("cls");
	cout << "                                                                       LOGIN" << endl;
	cin.ignore();
	cout << "Enter user name :";
	getline(cin, name);
	cout << "Enter phone number :";
	cin >> phone;
	cout << "Enter password :";
	while (true)
	{
		ch = _getch();
		if (ch == '\r')
		{
			break;
		}
		else if (ch == '\b')
		{
			if (!login_password.empty())
			{
				login_password.pop_back();
				cout << "\b \b";
			}
		}
		else
		{
			login_password += ch;
			cout << "*";
		}
	}
	cout << endl;
	obj = mysql_init(nullptr);
	if (!mysql_real_connect(obj, host, user, pass, "trainapp", 3306, NULL, 0))
	{
		cout << "Error :" << mysql_error(obj) << endl;
	}
	else
	{
		string word = "select phone from appaccount where phone='" + phone + "' and password='" + login_password + "'";
		if (mysql_query(obj, word.c_str()))
		{
			cout << "Error :" << mysql_error(obj) << endl;
		}
		else
		{
			MYSQL_RES *res;
			MYSQL_ROW row;
			res = mysql_store_result(obj);
			if (res)
			{
				if (mysql_num_rows(res) > 0)
				{
					found = true;
					cout << "Login successfull " << endl;
				}
				else
				{
					cout << "No to data found on this mobile number!" << endl;
				}
				mysql_free_result(res);
			}
			else
			{
				cout << "Error :" << mysql_error(obj) << endl;
			}
		}
	}
	mysql_close(obj);
	// adding internal functions
	int choice;
	if (found == true)
	{
		do
		{
			cout << " " << endl;
			cout << " " << endl;
			cout << "1) Book train tickets" << endl;
			cout << "2) Check booked tickets" << endl;
			cout << "3) Cancel tickets" << endl;
			cout << "4) EXIT" << endl;
			cout << "Enter choice :";
			cin >> choice;
			switch (choice)
			{
			case 1:
				// function to book tickets
				booking_ticket();
				break;
			case 2:
				// function to check ticket history
				tickect_histroy();
				break;
			case 3:
				cancel_tickets();
				break;
			case 4:
				break;
			default:
				cout << "Invalid choice" << endl;
			}
		} while (choice != 4);
	}
}

int main()
{
	train t;
	int choice;
	do
	{
		cout << " " << endl;
		cout << " " << endl;
		cout << " " << endl;
		cout << "1)Create account " << endl;
		cout << "2)Login" << endl;
		cout << "3)Change account password" << endl;
		cout << "4)Delete account" << endl;
		cout << "5)EXIT" << endl;
		cout << "Enter choice :";
		cin >> choice;
		switch (choice)
		{
		case 1:
			t.create_account();
			break;
		case 2:
			t.login();
			break;
		case 3:
			t.forget_password();
			break;
		case 4:
			t.close_account();
			break;
		case 5:
			cout << "Exiting" << endl;
			break;
		default:
			cout << "Invalid choice!" << endl;
		}
	} while (choice != 5);
	return 0;
}