#pragma once

class database_error : public std::runtime_error{
public:
	database_error(const std::string& msg) : std::runtime_error(msg) {}
};

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
		std::cout << "--------------------------" << "\n";
	}

	static void print(const std::vector<Student>& students) {

		for (int i = 0; i < students.size(); ++i) {
			Student s = students[i];
			std::cout << "Student id:   " << s.id << "\n";
			std::cout << "Class number: " << s.number << "\n";
			std::cout << "Full Name:    " << s.name << "\n";
			std::cout << "Birthdate:    " << s.birthdate << "\n";
			std::cout << "--------------------------" << "\n";
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

	static void insert(soci::session& sql, const Student& s) {

		sql << "INSERT INTO students (number, name, birthdate) VALUES (:number, :name, :birthdate)", soci::use(s.number), soci::use(s.name), soci::use(s.birthdate);
	}

	static Student findById(soci::session& sql, int id) {
		Student s;
		soci::statement stmt = (sql.prepare << "SELECT id, number, name, birthdate FROM students WHERE id = :id", soci::use(id),
			soci::into(s.id),
			soci::into(s.number),
			soci::into(s.name),
			soci::into(s.birthdate));

		stmt.execute();

		if (!stmt.fetch()) {
			std::string message = "Non-existent student with id " + std::to_string(id) + "!";
			throw database_error(message);
		} 

		return s;
	}	

	static void editById(soci::session& sql,int id, const Student& newStudent) {

		findById(sql, id);

		sql << "UPDATE students SET number = :number, name = :name, birthdate = :birthdate WHERE id = :id",
			soci::use(newStudent.number),
			soci::use(newStudent.name),
			soci::use(newStudent.birthdate),
			soci::use(id);

		std::cout << "Successfully edited student with id " + std::to_string(id) + "!\n";
		std::cout << "---------------------------\n";
	}

	static void deleteById(soci::session& sql, int id) {

		Student s = findById(sql, id);

		sql << "DELETE FROM students WHERE id = :id", soci::use(id);

		std::cout << "Successfully deleted student " + s.name + "!\n";
		std::cout << "---------------------------\n";
	}
};	

