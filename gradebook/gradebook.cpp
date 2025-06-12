
#include <iostream>
#include<string>
#include <vector>

#include "./odbc_helper.hpp"

struct Student {
    int id;
    int number;
    std::wstring name;
    std::wstring birthdate;

    bool is_valid() const {
        return true;
    }
};

void printStudent(const Student& s) {

    std::cout << "Class number: ";
    std::cout << s.number << '\n';
    std::cout << "Full name: ";
    std::wcout << s.name << L'\n';
    std::cout << "Date of birth: ";
    std::wcout << s.birthdate << L'\n';
}

void insertStudent(const Student& s, const SQLHDBC& hDbc, SQLHSTMT& hStmt) {
    SQLRETURN ret = 0;

    std::wstring sql = LR"(
        INSERT INTO students (number, name, birthdate)
        VALUES(?, ?, ?)
    )";

    ret = SQLPrepare(
        hStmt,
        (SQLWCHAR*)sql.c_str(),
        SQL_NTS
    );
    if (!SQL_SUCCEEDED(ret)) {
        std::cerr << "Preparation has failed!";
        return;
    }

    SQLLEN ind_num = 0;  // not NULL
    ret = SQLBindParameter(
        hStmt,                   // statement
        1,                       // parameter #1
        SQL_PARAM_INPUT,
        SQL_C_SLONG,             // C type: signed long
        SQL_INTEGER,             // SQL type: INT
        0, 0,
        (SQLPOINTER)&s.number,   // address of your int
        0,
        &ind_num
    );
    if (!SQL_SUCCEEDED(ret)) {
        std::cerr << "Binding #1 has failed!";
        return;
    }

    // 4b) name → NVARCHAR/VARCHAR
    SQLLEN ind_name = SQL_NTS;
    ret = SQLBindParameter(
        hStmt,
        2,
        SQL_PARAM_INPUT,
        SQL_C_WCHAR,             // C type: wchar_t*
        SQL_WVARCHAR,            // SQL type: (W)VARCHAR
        100,                     // max chars in column
        0,
        (SQLPOINTER)s.name.c_str(),
        (SQLLEN)s.name.size(),   // buffer length in WCHARs
        &ind_name
    );
    if (!SQL_SUCCEEDED(ret)) {
        std::cerr << "Binding #2 has failed!";
        return;
    }


    SQLLEN ind_date = SQL_NTS;
    ret = SQLBindParameter(
        hStmt,
        3,
        SQL_PARAM_INPUT,
        SQL_C_WCHAR,
        SQL_WVARCHAR,
        10,                      // “DD/MM/YYYY” is 10 chars
        0,
        (SQLPOINTER)s.birthdate.c_str(),
        (SQLLEN)s.birthdate.size(),
        &ind_date
    );
    if (!SQL_SUCCEEDED(ret)) {
        std::cerr << "Binding #3 has failed!";
        return;
    }


    ret = SQLExecute(hStmt);
    if (!SQL_SUCCEEDED(ret)) {
        std::cerr << "Executoin has failed!";
        return;
    }
}

int main()
{
    //1. Declare ODBC handles

    SQLHENV hEnv = nullptr;     //Environment handle
    SQLHDBC hDbc = nullptr;     //Database connection handle
    SQLHSTMT hStmt = nullptr;   //Statement handle 
    
    //2. Allocate environment handle
    if (!SQL_SUCCEEDED(allocEnvironment(hEnv))) {
        std::cerr << "Env alloc failed\n"; 
        return 1;
    }

    //3. Allocate connection handle
    if (!SQL_SUCCEEDED(allocConnection(hEnv, hDbc))) {
        std::cerr << "Connection alloc failed\n"; 
        cleanUpHandles(hDbc, hEnv); 
        return 1;
    }
   

    SQLWCHAR connStr[] = L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=DESKTOP-3KAPKMM;DATABASE=TestDB;UID=sa;PWD=sa;";
    //for now 


    if (!SQL_SUCCEEDED(connectDatabase(hDbc, connStr))) {
        std::cerr << "Connect failed\n"; cleanUpHandles(hDbc, hEnv); 
        return 1;
    }

    std::cout << "Connected to SQL Server!\n";

    //Statement logic

    if (!SQL_SUCCEEDED(allocStatement(hDbc, hStmt))) {
        std::cerr << "Statement alloc has failed!";
        return 1;
    }

    Student s;
    s.number = 20;
    s.name = L"Mitko Petrov";
    s.birthdate = L"20/04/2002";


    insertStudent(s, hDbc, hStmt);

    cleanUpStmt(hStmt);

    cleanUpHandles(hDbc, hEnv);

    return 0;
}