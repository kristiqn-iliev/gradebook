#pragma once

#include<regex>

struct Grade {
	int id;
	int student_id;
	int subject_id;
	std::string grade_date;
	double grade;
};


class GradeService {
private:
	static bool isValidDateFormat(const std::string& date) {
		std::regex dateRegex(R"(^(0?[1-9]|[12][0-9]|3[01])[\/\-](0?[1-9]|1[012])[\/\-]\d{4}$)");
		

		return std::regex_match(date, dateRegex);
	}

	static bool isValidGradeFormat(double grade) {
		return grade >= 2.0 && grade <= 6.0;
	}


	static bool isValidGrade(const Grade& g) {
		return isValidDateFormat(g.grade_date) && isValidGradeFormat(g.grade);
	}

	GradeService() = delete;
public:
	static void insert(soci::session& sql, const Grade& g) {

		if (!isValidGrade(g)) throw std::runtime_error("Invalid grade format!");

		sql << "INSERT INTO grades (student_id, subject_id, grade_date, grade) VALUES (:student_id, :subject_id, :grade_date, :grade)",
			soci::use(g.student_id),
			soci::use(g.subject_id),
			soci::use(g.grade_date),
			soci::use(g.grade);

		std::cout << " Successfully insrted a new grade!\n";
		std::cout << "---------------------------\n";
	}

	static Grade findById(soci::session& sql, int id) {
		Grade g;

		soci::statement stmt = (sql.prepare << "SELECT id, student_id, subject_id, grade_date, grade FROM grades WHERE id = :id", soci::use(id),
			soci::into(g.id),
			soci::into(g.student_id),
			soci::into(g.subject_id),
			soci::into(g.grade_date),
			soci::into(g.grade));

		stmt.execute();

		if (!stmt.fetch()) {
			std::string message = "Non-existent grade with id " + std::to_string(id) + "!";
			throw database_error(message);
		}

		return g;
	}

	static void editGradeById(soci::session& sql, int id, double newGrade) {

		findById(sql, id);

		sql << "UPDATE grades SET grade = :grade WHERE id = :id",
			soci::use(newGrade),
			soci::use(id);

		std::cout << "Successfully edited grade with id " + std::to_string(id) + "!\n";
		std::cout << "---------------------------\n";
	}

	static void deleteById(soci::session& sql, int id) {

		Grade g = findById(sql, id);

		sql << "DELETE FROM grades WHERE id = :id", soci::use(id);

		std::cout << "Successfully deleted grade " + std::to_string(g.grade) + " of student with id " + std::to_string(g.student_id) + " for subject with id "  << std::to_string(g.subject_id) << "!\n";
		std::cout << "---------------------------\n";
	}

};

