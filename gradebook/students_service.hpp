#pragma once

struct Student {
	int id;
	int number;
	std::string name;
	std::string birthdate;
};

class StudentService {
	StudentService() = delete;

public:
	static void print(const Student& s) {
		std::cout << "Student id:   " << s.id << "\n";
		std::cout << "Class number: " << s.number << "\n";
		std::cout << "Full Name:    " << s.name << "\n";
		std::cout << "Birthdate:    " << s.birthdate << "\n";
		std::cout << "-----------------------" << "\n";
	}

	static void print(const std::vector<Student>& students) {

		for (int i = 0; i < students.size(); ++i) {
			Student s = students[i];
			std::cout << "Student id:   " << s.id << "\n";
			std::cout << "Class number: " << s.number << "\n";
			std::cout << "Full Name:    " << s.name << "\n";
			std::cout << "Birthdate:    " << s.birthdate << "\n";
			std::cout << "-----------------------" << "\n";
		}
	}

	static std::vector<Student> fetchAll(soci::session& sql) {

		std::vector<Student> students;

		Student s;

		soci::statement st = (sql.prepare << "SELECT id, number, name, birthdate FROM students",
			soci::into(s.id),
			soci::into(s.number),
			soci::into(s.name),
			soci::into(s.birthdate));


		st.execute();
		while (st.fetch()) {
			students.push_back(s);
		}

		return students;
	}
};

