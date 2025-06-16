#pragma once

#include "./services/students_service.hpp"
#include "./services/subjects_service.hpp"
#include "./services/grades_service.hpp"
#include "./services//reporting_service.hpp"

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
	try {
		int testValue = 0;
		sql_ << "SELECT 1", soci::into(testValue);

		std::cout << "------------------------------\n";
		std::cout << "Connected via SOCI-ODBC!\n";
		//std::cout << "Connection test result: " << testValue << "\n";
		std::cout << "------------------------------";
	}
	catch (const soci::soci_error& e) {
		std::cerr << "[Connection Error] " << e.what() << "\n";
		throw; 
	}
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

		<< "32) Add a grade\n"
		<< "33) Edit a grade\n"
		<< "34) Delete a grade\n\n"

		<< "1) Get average grade for student in subject\n"
		<< "2) Get average grade for student over all subjects\n"
		<< "3) Display a list for top students (above 5.00)\n"
		<< "4) Display a list of students who will have to take a remedial exam in a subject\n"
		<< "5) Display a list of students who have a grade less than 3.00 in three or more subjects\n"
		<< "6) Display a list of students who have birthday today\n\n"

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
		case 23: {
			Subject newSubject;
			int id;
			std::cout << "===  EDIT SUBJECT INFO ===\n";
			std::cout << "--------------------------\n";
			std::cout << " First, pick the id of the\n";
			std::cout << " chosen for update subject:\n";
			std::cout << "--------------------------\n";
			std::cout << " id: "; std::cin >> id;

			Subject s = SubjectService::findById(sql_, id);

			std::cout << "--------------------------\n";
			std::cout << " To edit the information  \n";
			std::cout << " for the chosen subject   \n";
			std::cout << " provide the following    \n";
			std::cout << " data: classroom number,  \n";
			std::cout << " name and teacher:		\n";
			std::cout << "--------------------------\n";
			std::cout << " Current number : "; std::cout << s.number << '\n';;
			std::cout << " number : "; std::cin >> newSubject.number; std::cin.ignore();
			std::cout << "--------------------------\n";

			std::cout << " Current name : "; std::cout << s.name << '\n';;
			std::cout << " name : "; std::getline(std::cin, newSubject.name, '\n');
			std::cout << "--------------------------\n";

			std::cout << " Current teacher : "; std::cout << s.teacher << '\n';
			std::cout << " teacher : "; std::cin >> newSubject.teacher;
			std::cout << "--------------------------\n";

			SubjectService::editById(sql_, id, newSubject);

			break;
		}
		case 24: {
			int id;
			char answer;
			std::cout << "===   DELETE SUBJECT   ===\n";
			std::cout << "--------------------------\n";
			std::cout << " First, pick the id of    \n";
			std::cout << " the chosen for deletion  \n";
			std::cout << " subject.				    \n";
			std::cout << "--------------------------\n";
			std::cout << " id: "; std::cin >> id;

			Subject s = SubjectService::findById(sql_, id);

			std::cout << "--------------------------\n";
			std::cout << " Confirm the deletion of  \n";
			std::cout << " subject with id: " << s.id << "\n";
			std::cout << " " << s.name << "\n";
			std::cout << " y/n : "; std::cin >> answer;

			if (answer == 'y') {
				std::cout << "--------------------------\n";
				SubjectService::deleteById(sql_, id);
			}

			else if (answer == 'n')
				std::cout << "\n--------------------------\n     Deletion aborted!\n--------------------------\n";

			break;
		}


		case 32: {
			Grade g;

			std::cout << "===  INSERT NEW GRADE  ===\n";
			std::cout << "--------------------------\n";
			std::cout << " To insert a new grade    \n";
			std::cout << " provide the following    \n";
			std::cout << " data: student_id, sub-   \n";
			std::cout << " ject_id, grade_date and  \n";
			std::cout << " grade.				    \n";
			std::cout << "--------------------------\n";
			std::cout << " student_id : "; std::cin >> g.student_id;
			std::cout << "--------------------------\n";
			std::cout << " subject_id : "; std::cin >> g.subject_id; std::cin.ignore();
			std::cout << "--------------------------\n";
			std::cout << " grade_date : "; std::getline(std::cin, g.grade_date, '\n');
			std::cout << "--------------------------\n";
			std::cout << " grade : "; std::cin >> g.grade;
			std::cout << "--------------------------\n";

			GradeService::insert(sql_, g);

			break;
		}


		case 33: {
			Grade newGrade;
			int id;
			std::cout << "===   EDIT GRADE INFO  ===\n";
			std::cout << "--------------------------\n";
			std::cout << " First, pick the id of the\n";
			std::cout << " chosen for update grade :\n";
			std::cout << "--------------------------\n";
			std::cout << " id: "; std::cin >> id;

			Grade g = GradeService::findById(sql_, id);

			std::cout << "--------------------------\n";
			std::cout << " To edit the grade of a   \n";
			std::cout << " student for this subject \n";
			std::cout << " provide the new grade:   \n";
			std::cout << "--------------------------\n";

			std::cout << " Current grade : "; std::cout << g.grade << '\n';
			std::cout << " New grade : "; std::cin >> newGrade.grade;
			std::cout << "--------------------------\n";

			GradeService::editGradeById(sql_, id, newGrade.grade);

			break;
		}
		case 34: {
			int id;
			char answer;
			std::cout << "===    DELETE GRADE    ===\n";
			std::cout << "--------------------------\n";
			std::cout << " First, pick the id of    \n";
			std::cout << " the chosen for deletion  \n";
			std::cout << " grade.				    \n";
			std::cout << "--------------------------\n";
			std::cout << " id: "; std::cin >> id;

			Grade g = GradeService::findById(sql_, id);

			std::cout << "--------------------------\n";
			std::cout << " Confirm the deletion of  \n";
			std::cout << " grade with id: " << g.id << "\n";
			std::cout << " y/n : "; std::cin >> answer;

			if (answer == 'y') {
				std::cout << "--------------------------\n";
				GradeService::deleteById(sql_, id);
			}

			else if (answer == 'n')
				std::cout << "\n--------------------------\n     Deletion aborted!\n--------------------------\n";

			break;
		}

		case 1: {
			int student_id;
			int subject_id;

			std::cout << "===  AVG GRADE IN SUB  ===\n";
			std::cout << "Student id: "; std::cin >> student_id;
			std::cout << "Subject id: "; std::cin >> subject_id;

			try {

				std::cout << "Average grade: " << ReportingService::getAverageGradeForStudentInSubject(student_id, subject_id, sql_) << "\n";
			}
			catch (std::exception& e) {

				std::cout << "Average grade: NULL \n";
			}

			std::cout << "---------------------------\n";

			break;
		}
		case 2: {
			int student_id;

			std::cout << "===  STUDENT AVG GRADE  ===\n";
			std::cout << "Student id: "; std::cin >> student_id;

			try {

				std::cout << "Average grade: " << ReportingService::getAverageGradeForStudent(student_id, sql_) << "\n";
			}
			catch (std::exception& e) {

				std::cout << "Average grade: NULL \n";
			}

			std::cout << "---------------------------\n";

			break;
		}
		case 3: {
			std::cout << "===    TOP STUDENTS    ===\n";

			ReportingService::listTopStudents(sql_);

			std::cout << "---------------------------\n";

			break;
		}
		case 4: {
			std::cout << "=== STUDENTS FOR REMEDY ===\n";

			ReportingService::listRemedyStudents(sql_);

			std::cout << "---------------------------\n";

			break;
		}
		case 5: {
			std::cout << "=== FAILING STUDENTS AND COUNT ====\n";
			ReportingService::listFailingStudents(sql_);

			std::cout << "---------------------------\n";

			break;
		}

		case 6: {
			std::cout << "=== BIRTHDAY STUDENTS! ===\n";


			ReportingService::listBirthdayStudents(sql_);

			std::cout << "---------------------------\n";

			break;
		}
			  // … other cases …
		default:
			std::cout << "===    UNKOWN CHOICE   ===\n";
			std::cout << "--------------------------\n";
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		std::cout << "--------------------------\n";
	}
}