#include <iostream>
#include <cstdarg>
#include <string>
#include <string_view>
#include "wrapper.h"

using namespace std;

SQLite::SQLite(string_view const filename) : filename(filename) {
    disconnect();
    if (sqlite3_open(filename.data(), &connection)) {
        message("sqlite3_open() function error");
        disconnect();
        exit(0);
    }
}

SQLite::~SQLite() {
    disconnect();
}

void SQLite::disconnect() {
    reset();
    if (connection) {
        sqlite3_close(connection);
        connection = nullptr;
    }
}

void SQLite::reset() {
    if (query) {
        sqlite3_finalize(query);
        query = nullptr;
    }
    names.clear();
}

void SQLite::prepare(string_view const sql, va_list args) {
    reset();
    if (sqlite3_prepare_v2(connection, sql.data(), -1, &query, nullptr)) {
        message("sqlite3_prepare_v2() function error");
        reset();;
    }

    int count = sqlite3_column_count(query);
    for (int i = 0; i < count; i++) {
        string name = sqlite3_column_name(query, i);
        names.push_back(name);
    }

    count = sqlite3_bind_parameter_count(query);
    if (count) {
        for (int i = 1; i <= count; i++) {
            const char* parameter = va_arg(args, const char*);
            sqlite3_bind_text(query, i, parameter, -1, SQLITE_STATIC);
        }
    }
}

void SQLite::select(string_view const sql, ...) {
    va_list args;
    va_start(args, sql);
    prepare(sql, args);
    va_end(args);
}

vector<string> SQLite::get() {
    vector<string> row;
    if (query) {
        if (sqlite3_step(query) == SQLITE_ROW) {
            size_t count = names.size();
            for (int i = 0; i < count; i++) {
                // unsigned char* -> char*
//                const char* str = reinterpret_cast<const char*>(sqlite3_column_text(query, i));
                string data =  reinterpret_cast<const char*>(sqlite3_column_text(query, i));
                row.push_back(data);
            }
        } else reset();
    } else reset();
    return row;
}

void SQLite::show() {
    int rows = 0;
    size_t const columns = names.size();
    vector<string> row = get();
    while (!row.empty()) {
        for (int i = 0; i < columns; i++)
            cout << row[i] << ((i < columns - 1) ? "; " : "\n");
        row = get();
        rows++;
    }
    if (rows == 0) cout << "Queryset is empty" << endl;
}

int SQLite::execute(string_view sql, ...) {
    va_list args;
    va_start(args, sql);
    prepare(sql, args);
    va_end(args);
    sqlite3_step(query);
    reset();
    return sqlite3_changes(connection);
}

string SQLite::value(string_view sql, ...) {
    va_list args;
    va_start(args, sql);
    prepare(sql, args);

    va_end(args);
    vector<string> row = get();
    reset();
    string str;
    if (!row.empty()) str = row[0];
    return str;
}


void SQLite::message(string_view const text) const {
    cout << text << endl;
    if (connection) {
        cout << sqlite3_errmsg(connection) << endl;
    } else cout << "Unknown error." << endl;
}