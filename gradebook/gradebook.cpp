#include <soci/soci.h>
#include <soci/odbc/soci-odbc.h>
#include <iostream>
#include <string>
#include "./students_service.hpp"
#include "./subjects_service.hpp"
#include "./grades_service.hpp"


#include<chrono>
#include <iomanip>  
#include<string>




class App {
public:
	App(const std::string& connectString);
	int run();  // returns exit code

private:
	void printMenu();
	void handleChoice(int choice);

	soci::session sql_;
};

App::App(const std::string& connectString)
	: sql_(soci::odbc, connectString)
{
	std::cout << "------------------------\n";
	std::cout << "Connected via SOCI-ODBC!\n";
	std::cout << "------------------------\n";
}

int App::run() {
	while (true) {
		printMenu();
		int choice;
		if (!(std::cin >> choice))     // invalid input?
			return 1;                  // or clear and continue
		if (choice == 0)
			break;
		handleChoice(choice);
	}
	return 0;
}

void App::printMenu() {
	std::cout
		<< "\n=== Student Management ===\n\n"
		<< "11) List all students\n"
		<< "12) Add a student\n"
		<< "13) Edit a student\n"
		<< "14) Delete a student\n\n"

		<< "21) List all subjects\n"
		<< "22) Add a subject\n"
		<< "23) Edit a subject\n"
		<< "24) Delete a subject\n\n"

		<< "31) Add a grade\n"
		<< "32) Edit a grade\n"
		<< "33) Delete a grade\n\n"

		<< "1) List top students\n"
		<< "2) Birthday today\n\n"

		<< "0) Quit\n\n"
		<< "Choose> ";
}

void App::handleChoice(int choice) {
	try {

		std::cout << "\n--------------------------\n";
		switch (choice) {
		case 11: {
			std::cout << "===LISTING ALL STUDENTS===\n";
			std::cout << "--------------------------\n";
			std::vector<Student> students = StudentService::fetchAll(sql_);
			StudentService::print(students);
			break;
		}

		case 12: {
			Student s;

			std::cout << "=== INSERT NEW STUDENT ===\n";
			std::cout << "--------------------------\n";
			std::cout << " To insert a new student  \n";
			std::cout << " provide the following    \n";
			std::cout << " data: number in class,   \n";
			std::cout << " name and date of birth:	\n";
			std::cout << "--------------------------\n";
			std::cout << " number : "; std::cin >> s.number; std::cin.ignore();
			std::cout << "--------------------------\n";
			std::cout << " name : "; std::getline(std::cin, s.name, '\n'); 
			std::cout << "--------------------------\n";
			std::cout << " birthdate : "; std::cin >> s.birthdate;
			std::cout << "--------------------------\n";

			StudentService::insert(sql_, s);

			break;
		}
		case 13: {
			Student newStudent;
			int id;
			std::cout << "===  EDIT STUDENT INFO ===\n";
			std::cout << "--------------------------\n";
			std::cout << " First, pick the id of the\n";
			std::cout << " chosen for update student:\n";
			std::cout << "--------------------------\n";
			std::cout << " id: "; std::cin >> id;

			Student s = StudentService::findById(sql_, id);

			std::cout << "--------------------------\n";
			std::cout << " To edit the information  \n";
			std::cout << " for the chosen student   \n";
			std::cout << " provide the following    \n";
			std::cout << " data: number in class,   \n";
			std::cout << " name and date of birth:	\n";
			std::cout << "--------------------------\n";
			std::cout << " Current number : "; std::cout << s.number << '\n';;
			std::cout << " number : "; std::cin >> newStudent.number; std::cin.ignore();
			std::cout << "--------------------------\n";

			std::cout << " Current name : "; std::cout << s.name << '\n';;
			std::cout << " name : "; std::getline(std::cin, newStudent.name, '\n');
			std::cout << "--------------------------\n";

			std::cout << " Current birthdate : "; std::cout << s.birthdate << '\n';
			std::cout << " birthdate : "; std::cin >> newStudent.birthdate;
			std::cout << "--------------------------\n";

			StudentService::editById(sql_, id, newStudent);

			break;
		}

		case 14: {
			Student newStudent;
			int id;
			char answer;
			std::cout << "===   DELETE STUDENT   ===\n";
			std::cout << "--------------------------\n";
			std::cout << " First, pick the id of    \n";
			std::cout << " the chosen for deletion  \n";
			std::cout << " student.				    \n";
			std::cout << "--------------------------\n";
			std::cout << " id: "; std::cin >> id;

			Student s = StudentService::findById(sql_, id);

			std::cout << "--------------------------\n";
			std::cout << " Confirm the deletion of  \n";
			std::cout << " student with id: " << s.id << "\n";
			std::cout << " " << s.name << "\n";
			std::cout << " y/n : "; std::cin >> answer;

			if (answer == 'y') {
				std::cout << "--------------------------\n";
				StudentService::deleteById(sql_, id);
			}

			else if (answer == 'n')
				std::cout << "\n--------------------------\n     Deletion aborted!\n--------------------------\n";

			break;
		}

		case 21: {
			std::cout << "===LISTING ALL SUBJECTS===\n";
			std::cout << "--------------------------\n";
			std::vector<Subject> subjects = SubjectService::fetchAll(sql_);
			SubjectService::print(subjects);
			break;
		}
		case 22: {
			Subject s;

			std::cout << "=== INSERT NEW SUBJECT ===\n";
			std::cout << "--------------------------\n";
			std::cout << " To insert a new subject  \n";
			std::cout << " provide the following    \n";
			std::cout << " data: number of the room,\n";
			std::cout << " name and teacher:		\n";
			std::cout << "--------------------------\n";
			std::cout << " number : "; std::cin >> s.number; std::cin.ignore();
			std::cout << "--------------------------\n";
			std::cout << " name : "; std::getline(std::cin, s.name, '\n');
			std::cout << "--------------------------\n";
			std::cout << " teacher : "; std::getline(std::cin, s.teacher, '\n');
			std::cout << "--------------------------\n";

			SubjectService::insert(sql_, s);

			break;
		}
			  // … other cases …
		default:
			std::cout << "===    UNKOWN CHOICE   ===\n";
			std::cout << "--------------------------\n";
		}
	}
	catch (const std::exception& e) {
		std::cout << "--------------------------\n";
		std::cerr << "Error: " << e.what() << "\n";
		std::cout << "--------------------------\n";
	}
}
int main()
{

	std::string connectionString = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=DESKTOP-3KAPKMM;DATABASE=TestDB;UID=sa;PWD=sa;";
	try {
		App app(connectionString);
		app.run();

		auto now_tp = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now_tp);
		std::tm local_tm = *std::localtime(&now_c);

		// now print it:
		std::cout << "Current date: "
			<< std::put_time(&local_tm, "%d/%m/%Y")
			<< '\n';

		//6,7,8,9,12,13,29,30,32
		Grade g;
		g.student_id = 30;
		g.subject_id = 1;
		g.grade_date = local_tm;
		g.grade = 2.00;



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