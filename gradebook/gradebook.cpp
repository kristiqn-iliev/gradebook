#include <soci/soci.h>
#include <soci/odbc/soci-odbc.h>
#include <iostream>

#include "./application.hpp"


int main()
{

	std::string connectionString = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=DESKTOP-3KAPKMM;DATABASE=TestDB;UID=sa;PWD=sa;";

	try {
		App app(connectionString);
		app.run();
	}
	catch (const soci::soci_error& e) {
		std::cerr << "Connection failed: " << e.what() << "\n";
		return 1;
	}
	catch (const database_error& e) {
		std::cerr << "Database Error: " << e.what() << "\n";
		return 1;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
	return 0;
}