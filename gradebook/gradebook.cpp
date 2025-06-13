#include <soci/soci.h>
#include <soci/odbc/soci-odbc.h>
#include <iostream>
#include <string>
#include "./students_service.hpp"

int main()
{
	try {
		soci::session sql(soci::odbc, "DRIVER={ODBC Driver 17 for SQL Server};SERVER=DESKTOP-3KAPKMM;DATABASE=TestDB;UID=sa;PWD=sa;");

		std::cout << "------------------------\n";
		std::cout << "Connected via SOCI-ODBC!\n";
		std::cout << "------------------------\n";


	}
	catch (const soci::soci_error& e) {
		std::cerr << "Connection failed: " << e.what() << "\n";
		return 1;
	}
	return 0;
}