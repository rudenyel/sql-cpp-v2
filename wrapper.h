#ifndef SQL_CPP_WRAPPER_H
#define SQL_CPP_WRAPPER_H

#include <string>
#include <vector>
#include <string_view>
#include "sqlite3.h"

using namespace std;

class SQLite {
private:
    string filename;
    sqlite3* connection = nullptr;
    sqlite3_stmt* query = nullptr;
    vector<string> names; // columns names

    void disconnect();
    void reset();
    void prepare(string_view, va_list);

public:
    SQLite(string_view);
    ~SQLite();

    void select(string_view, ...);
    vector<string> get(); // get row
    void show();

    int execute(string_view, ...);
    string value(string_view, ...);

    string dbname() { return filename; }
    void message(string_view) const;
};

#endif
