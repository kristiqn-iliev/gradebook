#pragma once

class ReportingService {
public:

	static double getAverageGradeForStudentInSubject(int student_id, int subject_id, soci::session& sql) {

		double average;

		sql
			<< R"(
			SELECT
				CAST(AVG(g.grade) AS DECIMAL(4,2)) AS average_grade
			FROM grades AS g
			JOIN students AS stud
			  ON g.student_id = stud.id
			JOIN subjects AS subj
			  ON g.subject_id = subj.id
			WHERE (subj.id = :subj_id AND stud.id = :stud_id)
          )",
			soci::use(subject_id, "subj_id"),
			soci::use(student_id, "stud_id"),
			soci::into(average);

		return average;
	}

	static double getAverageGradeForStudent(int student_id, soci::session& sql) {

		double average;

		sql
			<< R"(
				SELECT
			  CAST(AVG(g.grade) AS DECIMAL(4,2)) AS average_grade
			FROM students   AS s
			RIGHT JOIN grades AS g
			  ON g.student_id = s.id
			  WHERE s.id = :stud_id
          )",
			soci::use(student_id, "stud_id"),
			soci::into(average);

		return average;
	}

	static void listTopStudents(soci::session& sql) {

		struct Log {
			std::string student_name;
			double student_grade;
		}log = {};

		std::vector<Log> records;

		soci::statement st = (sql.prepare
			<< R"(
				SELECT
				s.name,
			  CAST(AVG(g.grade) AS DECIMAL(4,2)) AS average_grade
			FROM students   AS s
			RIGHT JOIN grades AS g
			  ON g.student_id = s.id
			GROUP BY
			  s.id,
			  s.name
			  HAVING 
				AVG(g.grade) > 5.0
          )",
			soci::into(log.student_name),
			soci::into(log.student_grade)
			);

		st.execute();
		while (st.fetch()) {
			records.push_back(log);
		}

		for (int i = 0; i < records.size(); ++i) {
			std::cout << records[i].student_name << " | " << records[i].student_grade << "\n";
		}

	}

	static void listRemedyStudents(soci::session& sql) {

		struct Log {
			std::string student_name;
			std::string subject_name;
			double grade;
		}log = {};

		std::vector<Log> records;

		soci::statement st = (sql.prepare
			<< R"(
			SELECT
				s.name, subj.name,
			  CAST(AVG(g.grade) AS DECIMAL(4,2)) AS average_grade
			FROM students   AS s
			JOIN grades AS g
			  ON g.student_id = s.id
			JOIN subjects AS subj
				ON g.subject_id = subj.id
			GROUP BY
			  s.id,
			  s.name,
			  subj.id,
			  subj.name
			HAVING AVG(g.grade) < 3.0
          )",
			soci::into(log.student_name),
			soci::into(log.subject_name),
			soci::into(log.grade)
			);

		st.execute();
		while (st.fetch()) {
			records.push_back(log);
		}

		for (int i = 0; i < records.size(); ++i) {
			std::cout << records[i].student_name << " | " << records[i].subject_name << " | " << records[i].grade << "\n";
		}

	}

	static void listFailingStudents(soci::session& sql) {
		struct Log {
			std::string student_name;
			int count;
		}log = {};

		std::vector<Log> records;

		soci::statement st = (sql.prepare
			<< R"(
				WITH failing_subjects AS (
				  SELECT
					s.name AS student_name,    
					g.subject_id
				  FROM grades   AS g
				  JOIN students AS s  ON g.student_id = s.id
				  JOIN subjects AS subj ON g.subject_id = subj.id
				  GROUP BY
					g.student_id,
					s.name,
					g.subject_id
				  HAVING
					AVG(g.grade) < 3.0)
				SELECT
				  student_name,
				  COUNT(*) AS cnt
				FROM failing_subjects
				GROUP BY
				  student_name
				HAVING
				  COUNT(*) >= 3 
				  ;
			)",
			soci::into(log.student_name),
			soci::into(log.count)
			);

		st.execute();
		while (st.fetch()) {
			records.push_back(log);
		}

		for (int i = 0; i < records.size(); ++i) {
			std::cout << records[i].student_name << " | " << records[i].count << "\n";
		}
	}

	static void listBirthdayStudents(soci::session& sql) {
		struct Log {
			std::string student_name;
			std::string birthdate;
		}log = {};

		std::vector<Log> records;

		soci::statement st = (sql.prepare
			<< R"(
				SELECT
				  name,
				  birthdate
				FROM students
				WHERE LEFT(birthdate, 5) = CONVERT(varchar(5), GETDATE(), 103);
			)",
			soci::into(log.student_name),
			soci::into(log.birthdate)
			);

		st.execute();
		while (st.fetch()) {
			records.push_back(log);
		}

		for (int i = 0; i < records.size(); ++i) {
			std::cout << records[i].student_name << " | " << records[i].birthdate << "\n";
		}
	}


};