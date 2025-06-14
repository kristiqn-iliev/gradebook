#pragma once

struct Subject {
	int id;
	int number;
	std::string name;
	std::string teacher;
};

class SubjectService {
private:
	static bool isValidNameFormat(const std::string& name) {

		std::regex nameRegex(R"(^ ([a - zA - Z] {2, }\s[a - zA - Z]{ 1, }'?-?[a-zA-Z]{2,}\s?([a-zA-Z]{1,})?)");

		return std::regex_match(name, nameRegex);
	}

	static bool isValidSubject(const Subject& s) {
		return isValidNameFormat(s.name) && isValidNameFormat(s.teacher);
	}

	SubjectService() = delete;

public:
	static void print(const Subject& s) {
		std::cout << "Subject id:   " << s.id << "\n";
		std::cout << "Room number:  " << s.number << "\n";
		std::cout << "Subject name: " << s.name<< "\n";
		std::cout << "Teacher:		" << s.teacher << "\n";
		std::cout << "--------------------------" << "\n";
	}

	static void print(const std::vector<Subject>& subjects) {

		for (int i = 0; i < subjects.size(); ++i) {
			Subject s = subjects[i];
			std::cout << "Student id:   " << s.id << "\n";
			std::cout << "Class number: " << s.number << "\n";
			std::cout << "Full Name:    " << s.name << "\n";
			std::cout << "Teacher:    " << s.teacher << "\n";
			std::cout << "--------------------------" << "\n";
		}
	}

	static std::vector<Subject> fetchAll(soci::session& sql) {

		std::vector<Subject> subjects;

		Subject s;

		soci::statement st = (sql.prepare << "SELECT id, number, name, teacher FROM subjects",
			soci::into(s.id),
			soci::into(s.number),
			soci::into(s.name),
			soci::into(s.teacher));


		st.execute();
		while (st.fetch()) {
			subjects.push_back(s);
		}

		return subjects;
	}

	static void insert(soci::session& sql, const Subject& s) {

		if (!isValidSubject(s)) throw std::runtime_error("Invalid subject format!");

		sql << "INSERT INTO subjects (number, name, teacher) VALUES (:number, :name, :teacher)", soci::use(s.number), soci::use(s.name), soci::use(s.teacher);


		std::cout << " Successful insertion of subject " + s.name + "!\n";
		std::cout << "--------------------------\n";
	}

	static Subject findById(soci::session& sql, int id) {
		Subject s;
		soci::statement stmt = (sql.prepare << "SELECT id, number, name, teacher FROM subjects WHERE id = :id", soci::use(id),
			soci::into(s.id),
			soci::into(s.number),
			soci::into(s.name),
			soci::into(s.teacher));

		stmt.execute();

		if (!stmt.fetch()) {
			std::string message = "Non-existent subject with id " + std::to_string(id) + "!";
			throw database_error(message);
		}

		return s;
	}

	static void editById(soci::session& sql, int id, const Subject& newSubject) {

		findById(sql, id);

		sql << "UPDATE subjects SET number = :number, name = :name, teacher = :teacher WHERE id = :id",
			soci::use(newSubject.number),
			soci::use(newSubject.name),
			soci::use(newSubject.teacher),
			soci::use(id);

		std::cout << "Successfully edited subject with id " + std::to_string(id) + "!\n";
		std::cout << "------------------------\n";
	}

	static void deleteById(soci::session& sql, int id) {

		Subject s = findById(sql, id);

		sql << "DELETE FROM subjects WHERE id = :id", soci::use(id);

		std::cout << "Successfully deleted subject " + s.name + "!\n";
		std::cout << "------------------------\n";
	}
};

