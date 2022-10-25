// light weight mysql driver for netlib
#pragma once
#include <iostream>
#include <string.h>
#include <mysql/mysql.h>
using namespace std;
typedef struct __MYSQL_RESULT
{
    MYSQL_RES *result;
    int no_of_columns;
    int no_of_rows;
} MYSQL_RESULT;

class Dbconnection
{
public:
    Dbconnection(string host, string user, string password, string database, int port=3306);
    bool executeQuery(string query);
    unsigned long int number_of_row();
    unsigned int number_of_columns();
    MYSQL_ROW getRow();
    MYSQL_RESULT getResult();
    void freeResult(MYSQL_RESULT);
    ~Dbconnection();

private:
    MYSQL mysql;
    MYSQL_RES *res;
    bool connect = 1;
};
Dbconnection::Dbconnection(string host, string user, string password, string database, int port)
{
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, NULL, CLIENT_SSL))
    {
        connect = 0;
    }
}
void Dbconnection::freeResult(MYSQL_RESULT result){
   mysql_free_result(result.result);

}
bool Dbconnection::executeQuery(string query)
{
    if (connect == 0)
        return false;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_store_result(&mysql);
    return true;
}
MYSQL_RESULT Dbconnection::getResult()
{
    MYSQL_RESULT r;
    r.result = res;
    r.no_of_columns = number_of_columns();
    r.no_of_rows = number_of_row();
    return r;
}
Dbconnection::~Dbconnection()
{

    mysql_free_result(res);
    mysql_close(&mysql);
}
uint64_t Dbconnection::number_of_row()
{
    return mysql_num_rows(res);
}
unsigned int Dbconnection::number_of_columns()
{
    return mysql_field_count(&mysql);
}
MYSQL_ROW Dbconnection::getRow()
{
    return mysql_fetch_row(res);
}
