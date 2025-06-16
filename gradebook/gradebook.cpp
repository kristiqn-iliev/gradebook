#include <soci/soci.h>
#include <soci/odbc/soci-odbc.h>
#include <iostream>

#include "./application.hpp"
#include "./env_parser.hpp"

int main()
{
	EnvParser env;

	std::string connectionString =
		"DRIVER=" + env.get("DRIVER") + ";" +
		"SERVER=" + env.get("SERVER") + ";" +
		"DATABASE=" + env.get("DATABASE") + ";" +
		"UID=" + env.get("UID") + ";" +
		"PWD=" + env.get("PWD");

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