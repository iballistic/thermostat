#include<iostream>
#include"sqlhelper.h"
#include"sqlite3.h"
//http://utf8everywhere.org/

using std::cout;
using std::cin;
using std::cerr;
using std::endl;


SQLHelper::SQLHelper()
{
    cout << "version: " << sqlite3_libversion() << endl;
}

int SQLHelper::connect(const char *file_name){
    int return_code = sqlite3_open(file_name, &this->db);
    cout << "db address: " << db << endl;
    if (return_code!= SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db);
        return 1;
    }
    else {
        cout << "Opened database successfully" << endl;
    }

    return 0;
}


int SQLHelper::run_query(const std::string query) {
    const char* temp = query.c_str();
    return this->run_query(temp);
}

//run scalar sql query
int SQLHelper::run_query(const char* query) {
    /*
    **The sqlite3_exec() interface is a convenience wrapper around
    ** [sqlite3_prepare_v2()], [sqlite3_step()], and [sqlite3_finalize()],
    ** that allows an application to run mult
    */
    int return_code {0};
    sqlite3_stmt* statement{ nullptr };
    const char* tail{ nullptr };
    const char* error{ nullptr };

    if (this->db == nullptr) {
        throw "Not connected to a db. Must call connect method first.";
        return -1;
    }

    return_code = sqlite3_prepare_v2(db, query, -1, &statement, &tail);
    #ifdef DEBUG
        cout << "sqlite3_prepare_v2: return code: " << return_code << endl;
    #endif


    if (return_code != SQLITE_OK && return_code != SQLITE_DONE) {
        error = sqlite3_errmsg(db);
        cerr << "Error: " << *error << "while running: " << query <<endl;
        return 1;
    }

    //Evaluate An SQL Statement
    return_code = sqlite3_step(statement);
    #ifdef DEBUG
        cout << "sqlite3_step: return code: " << return_code << endl;
    #endif

    if (return_code != SQLITE_OK && return_code!=SQLITE_DONE) {
        error = sqlite3_errmsg(db);
        cerr << "Error: " << *error << " Return code: " << return_code << std::endl;
        return 1;
    }
    //Destroy A Prepared Statement Object
    sqlite3_finalize(statement);

    return 0;
}


int SQLHelper::run_query_exec(const std::string query) {
    const char* temp = query.c_str();
    return this->run_query(temp);
}

int SQLHelper::run_query_exec(const char* query) {
    /*
  **The sqlite3_exec() interface is a convenience wrapper around
  ** [sqlite3_prepare_v2()], [sqlite3_step()], and [sqlite3_finalize()],
  ** that allows an application to run mult
  */
    int return_code{ 0 };
    char* error{ 0 };

    return_code = sqlite3_exec(db, query, nullptr, 0, &error);
    #ifdef DEBUG
        cout << "sqlite3_exec: return code: " << return_code << endl;
    #endif // DEBUG
    if (return_code != SQLITE_OK && return_code != SQLITE_DONE) {
        cerr << "Error: " << *error << "while running: " << query << endl;
        sqlite3_free(error);
        return 1;
    }

    return 0;
}

int SQLHelper::close() {
    //Closing A Database Connection
    sqlite3_close_v2(db);
    return 0;
}
