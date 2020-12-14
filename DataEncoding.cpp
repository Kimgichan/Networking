#include "DataEncoding.h"
#include <memory>


unordered_map<wstring, DB_PROJECT> DataEncoding::PROJECT_TABLE;


template<typename ... Args>
std::string format(const string& fmt, Args ... args)
{
    int len = snprintf(nullptr, 0, fmt.c_str(), args ...) + 1;
    vector<char> buf(len);
    snprintf(buf.data(), buf.size(), fmt.c_str(), args ...);
    return string(buf.begin(), buf.end());
    //va_list args;
    //va_start(args, fmt);
    ////int len = _vscprintf(fmt, args) + 1;
    //size_t len = snprintf(nullptr, 0, fmt, args) + 1;
    //cout << len << endl;;


    //vector<char> buf(10);
    //sprintf_s(buf.data(), buf.size(), fmt, args);
    //va_end(args);


    //return string(buf.begin(), buf.end());
}



//출처 https://stackoverrun.com/ko/q/4146450 []->start
string utf8_encode(const std::wstring& wstr)
{
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, 0, 0, 0, 0);
    vector<char> buf(len);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &buf[0], len, 0, 0);
    return std::string(buf.begin(), buf.end());
}
//출처 https://stackoverrun.com/ko/q/4146450 []->end


DataEncoding* DataEncoding::SelectEncoding(DB_PROJECT _projectName)
{
	switch (_projectName) {
		case DB_PROJECT::DB_PROJECT_A:
		{
            
			return &DE_ProjectA::PROJECT_A;
		}
		break;

	}
	return nullptr;
}


DE_ProjectA DE_ProjectA::PROJECT_A;

//int query, wchar_t name, int level, int money
string DE_ProjectA::Add(const char* _query, MYSQL* _db)
{
    bool success = false;

    if (mysql_query(_db, format("insert into `%s` (name, level, money) values (\'%s\', %d, %d)",
        DE_ProjectA::GetTableName().c_str(),
        utf8_encode((wchar_t*)(&_query[4])).c_str(), *((int*)(&_query[28])), *((int*)(&_query[32]))).c_str())
        == 0) 
    {
        success = true;
    }

    int query_state = 1;
    vector<char> sendMSG(5);
    sendMSG[0] = ((char*)(&query_state))[0];
    sendMSG[1] = ((char*)(&query_state))[1];
    sendMSG[2] = ((char*)(&query_state))[2];
    sendMSG[3] = ((char*)(&query_state))[3];
    sendMSG[4] = ((char*)(&success))[0];
    return string(sendMSG.begin(), sendMSG.end());
}

string DE_ProjectA::SearchName(const char* _query, MYSQL* _db)
{
    //bool success = false;

    //if (mysql_query(_db, format("insert into `%s` (name) values (\'%s\')",
    //    DE_ProjectA::GetTableName().c_str(),
    //    utf8_encode((wchar_t*)(&_query[4])).c_str()).c_str())
    //    == 0)
    //{
    //    success = true;
    //}

    //int result = 2;
    //vector<char> sendMSG(4);
    //sendMSG[0] = ((char*)(&result))[0];
    //sendMSG[1] = ((char*)(&result))[1];
    //sendMSG[2] = ((char*)(&result))[2];
    //sendMSG[3] = ((char*)(&result))[3];

    return string();
}

string DE_ProjectA::SearchData(const char* _query, MYSQL* _db)
{
    bool success = false;

    if (mysql_query(_db, format("select `level`, `money` from `%s` where `name` = \'%s\'",
        DE_ProjectA::GetTableName().c_str(),
        utf8_encode((wchar_t*)(&_query[4])).c_str()).c_str())
        == 0)
    {
        success = true;
    }

    int query_state = 3;
    vector<char> sendMSG(13);
    int indx = 0;
    sendMSG[indx++] = ((char*)(&query_state))[0];
    sendMSG[indx++] = ((char*)(&query_state))[1];
    sendMSG[indx++] = ((char*)(&query_state))[2];
    sendMSG[indx++] = ((char*)(&query_state))[3];
    sendMSG[indx++] = ((char*)(&success))[0];

    if (success) {
        MYSQL_RES* sql_result;
        MYSQL_ROW sql_row;

        sql_result = mysql_store_result(_db);
        sql_row = mysql_fetch_row(sql_result);

        {
            int level = stoi(sql_row[0]);
            for (int i = 0; i < 4; i++) sendMSG[indx++] = ((char*)(&level))[i];
        }

        {
            int money = stoi(sql_row[1]);
            for (int i = 0; i < 4; i++) sendMSG[indx++] = ((char*)(&money))[i];
        }
    }

    return string(sendMSG.begin(), sendMSG.end());
}

string DE_ProjectA::Erase(const char* _query, MYSQL* _db)
{
    return string();
}

string DE_ProjectA::GetTableName()
{
    return "project_a";
}
