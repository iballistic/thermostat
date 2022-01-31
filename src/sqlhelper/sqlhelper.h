#pragma once
#include"sqlite3.h"
class SQLHelper
{
private: sqlite3* db{ nullptr };
public: SQLHelper();
public: int connect(const char *file_name);
public: int run_query(const char* query);
public: int run_query(const std::string query);
public: int run_query_exec(const char* query);
public: int run_query_exec(const std::string query);
public: int close();
};

