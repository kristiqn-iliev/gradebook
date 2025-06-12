
#include <iostream>
#include <vector>

#include "./odbc_helper.hpp"

int main()
{
    //1. Declare ODBC handles

    SQLHENV hEnv = nullptr;     //Environment handle
    SQLHDBC hDbc = nullptr;     //Database connection handle (knex)
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


    cleanUpHandles(hDbc, hEnv);


    return 0;
}