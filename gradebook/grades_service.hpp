#pragma once

struct Grade {
	int id;
	int student_id;
	int subject_id;
	std::tm grade_date;
	double grade;
};



class GradeService {

	GradeService() = delete;
public:
	static void insert(soci::session& sql, const Grade& g) {

		sql << "INSERT INTO grades (student_id, subject_id, grade_date, grade) VALUES (:student_id, :subject_id, :grade_date, :grade)",
			soci::use(g.student_id),
			soci::use(g.subject_id),
			soci::use(g.grade_date),
			soci::use(g.grade);
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

	static void editById(soci::session& sql, int id, double newGrade) {

		findById(sql, id);

		sql << "UPDATE students SET grade = :grade WHERE id = :id",
			soci::use(newGrade),
			soci::use(id);

		std::cout << "Successfully edited grade with id " + std::to_string(id) + "!\n";
		std::cout << "---------------------------\n";
	}

	static void deleteById(soci::session& sql, int id) {

		Grade g = findById(sql, id);

		sql << "DELETE FROM students WHERE id = :id", soci::use(id);

		std::cout << "Successfully deleted grade " + std::to_string(g.grade) + "of student with id" + std::to_string(g.id) + "for subject with id!\n";
		std::cout << "---------------------------\n";
	}

};