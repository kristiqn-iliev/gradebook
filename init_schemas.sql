USE SchoolDB;

CREATE TABLE students (
    id INT PRIMARY KEY IDENTITY(1,1),
    number INT UNIQUE NOT NULL,
    name VARCHAR(100) NOT NULL,
    birthdate VARCHAR(100)
);

CREATE TABLE subjects (
    id INT PRIMARY KEY IDENTITY(1,1),
    number INT NOT NULL,
    name VARCHAR(100),
    teacher VARCHAR(100)
);

CREATE TABLE grades (
    id INT PRIMARY KEY IDENTITY(1,1),
    student_id INT NOT NULL,
    subject_id INT NOT NULL,
    date VARCHAR(100),
    grade FLOAT,
    FOREIGN KEY (student_id) REFERENCES students(id),
    FOREIGN KEY (subject_id) REFERENCES subjects(id)
);