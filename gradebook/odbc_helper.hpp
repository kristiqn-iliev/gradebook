#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

SQLRETURN allocEnvironment(SQLHENV& hEnv) {

	return SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) |
	//SQLAllocHandle will give us a env handle object into the hEnv var
	//first argument expects the type of the handle (1 of 3 - ENV, DBC, STMT)
	//second argument expects the parent handle, the hierarchy is as follows:
	//(Null) -> ENV -> DBC -> STMT
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	//Here we set one of the attributes of the environment, namely the version
	//of ODBC
}

SQLRETURN allocConnection(const SQLHENV& hEnv, SQLHDBC& hDbc) {
	return SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
	//Here we allocate the database connection handle into the hDbc var
}

SQLRETURN connectDatabase(const SQLHDBC& hDbc,const SQLWCHAR* connStr) {
		
	return SQLDriverConnect(
		hDbc,
		nullptr,
		const_cast<SQLWCHAR*>(connStr),
		SQL_NTS,
		nullptr,
		0,
		nullptr,
		SQL_DRIVER_COMPLETE);
}

SQLRETURN allocStatement(const SQLHDBC& hDbc, SQLHSTMT& hStmt) {
	return SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
}

void cleanUpStmt(SQLHSTMT& hStmt) {
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void cleanUpHandles(SQLHENV& hDbc, SQLHDBC& hEnv) {

	if (hDbc) {
		SQLDisconnect(hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	}

	if(hEnv)
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}