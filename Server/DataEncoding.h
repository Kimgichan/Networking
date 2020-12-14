#pragma once

#include <iostream>

#include <string>
#include <mysql.h>
#include <vector>
#include <unordered_map>


#pragma comment(lib, "libmysql.lib")

using namespace std;

enum QUERY {
	Q_USER = 0,
	Q_ADD,
	Q_SERCH_NAME,
	Q_SERCH_DATA,
	Q_DELETE
};

enum class DB_PROJECT {
	DB_NULL,
	DB_PROJECT_A
};

class DataEncoding abstract
{
public:
	static unordered_map<wstring, DB_PROJECT> PROJECT_TABLE;
	static DataEncoding* SelectEncoding(DB_PROJECT _projectName);

	virtual string Add(const char* _query, MYSQL* _db) = 0;
	virtual string SearchName(const char* _query, MYSQL* _db) = 0;
	virtual string SearchData(const char* _query, MYSQL* _db) = 0;
	virtual string Erase(const char* _query, MYSQL* _db) = 0;
	virtual string GetTableName() = 0;
};

class DE_ProjectA : public DataEncoding {

public:
	static DE_ProjectA PROJECT_A;

	virtual string Add(const char* _query, MYSQL* _db) override;
	virtual string SearchName(const char* _query, MYSQL* _db) override;
	virtual string SearchData(const char* _query, MYSQL* _db) override;
	virtual string Erase(const char* _query, MYSQL* _db) override;
	virtual string GetTableName() override;
};
