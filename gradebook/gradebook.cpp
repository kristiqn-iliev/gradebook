
#include <iostream>
#include <vector>

#include <windows.h>
#include <sql.h>
#include <sqlext.h>

struct User {
    int id;
    std::wstring name;
    std::wstring email;
};

void selectUsers(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, std::vector<User>& users) {

    const SQLWCHAR* query = L"SELECT * FROM [users]";

    SQLPrepare(hStmt, (SQLWCHAR*)query, SQL_NTS);
    if (SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS) == SQL_SUCCESS) {
        while (SQLFetch(hStmt) == SQL_SUCCESS) {
            User user;
            wchar_t nameBuf[100] = { 0 };
            wchar_t emailBuf[100] = { 0 };

            SQLGetData(hStmt, 1, SQL_C_LONG, &user.id, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_WCHAR, nameBuf, sizeof(nameBuf), NULL);
            SQLGetData(hStmt, 3, SQL_C_WCHAR, emailBuf, sizeof(emailBuf), NULL);

            user.name = nameBuf;
            user.email = emailBuf;

            users.push_back(user);
        }
    }
    else {
        std::wcerr << L"Query failed.\n";
    }


}

void insertRow(const std::wstring& value1, const std::wstring& value2, const SQLHSTMT& hStmt) {
    const SQLWCHAR* sql = L"INSERT INTO [users] (name, email) VALUES (?, ?)";

    SQLPrepare(hStmt, (SQLWCHAR*)sql, SQL_NTS);

    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)value1.c_str(), 0, NULL);

    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)value2.c_str(), 0, NULL);

    SQLExecute(hStmt);
}



int main()
{

    std::vector<User> users;
    //1. Declare ODBC handles (What are the handles exactly ?)

    SQLHENV hEnv = nullptr;     //Environment handle
    SQLHDBC hDbc = nullptr;     //Database connection handle (knex)
    SQLHSTMT hStmt = nullptr;   //Statement handle 
    SQLRETURN ret = 0;              //Return code
    
    //2. Allocate environment handle
    
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    //3. Allocate connection handle
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    //4. Connection string
    SQLWCHAR connStr[] = L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=DESKTOP-3KAPKMM;DATABASE=TestDB;UID=sa;PWD=sa;";
    SQLWCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;

    //5. Connect to the databse

    SQLSetConnectAttr(hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);

    ret = SQLDriverConnect(hDbc, NULL, connStr, SQL_NTS, outConnStr, sizeof(outConnStr) / sizeof(SQLWCHAR), &outConnStrLen, SQL_DRIVER_COMPLETE);

    if (SQL_SUCCEEDED(ret)) {
        std::cout << "Connected to SQL Server!\n";

        //6. Allocate a statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);


        //7. Create a table 
        /*
        const SQLWCHAR* createTableSQL = LR"sql(
            CREATE TABLE users (
            id INT PRIMARY KEY IDENTITY(1,1),
            name NVARCHAR(100),
            email NVARCHAR(100)
            )
        )sql";

        SQLExecDirect(hStmt, (SQLWCHAR*)createTableSQL, SQL_NTS);
        std::cout << "Table created\n";
        */
        
        //8. Insert a row

        //const std::wstring name = L"Plamen Mixailov";
        //const std::wstring email = L"dihh@example.com";
        //insertRow(name, email, hStmt);
        //std::cout << "Row inserted.\n";

        selectUsers(hDbc, hStmt, users);

        /*
        const SQLWCHAR* insertSQL = LR"sql(
            INSERT INTO [users] (name, email)
            VALUES ('Plamen Kraewv', 'plamen@example.com')
        )sql";
        */

        //9. Clean up
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    }

    else {
        std::cerr << "Failed to connect to database.\n";
    }

    std::wcout << users[3].email;


    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}