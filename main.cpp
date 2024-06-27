
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include "main.h"
#include "wrapper.h"

using namespace std;

auto queryCheckBookTable =
    "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'books'";

auto queryCreateTable =
    "CREATE TABLE IF NOT EXISTS books ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "title VARCHAR(127) CHECK(title != ''),"
    "first_name VARCHAR(127) CHECK(first_name != ''),"
    "last_name VARCHAR(127) CHECK(last_name != ''),"
    "year INTEGER DEFAULT 0"
    ")"
;

const char* queryBooks = "SELECT * FROM books";
const char* queryBooksSortByAuthor = "SELECT * FROM books ORDER BY last_name, title";
const char* queryBooksSortByTitle = "SELECT * FROM books ORDER BY title, last_name";
const char* queryBooksSortByYear = "SELECT * FROM books ORDER BY year, title";
const char* queryBooksFindByAuthor = "SELECT * FROM books WHERE last_name LIKE ?";
const char* queryBooksFindByTitle = "SELECT * FROM books WHERE title LIKE ?";
const char* queryBookAdd = "INSERT INTO books (title, first_name, last_name, year) VALUES (?, ?, ?, ?)";
const char* queryBookDelete = "DELETE FROM books WHERE id = ?";
const char* queryBooksDropTable = "DROP TABLE IF EXISTS books";

const char* menu[] = {
    "List books",
    "List books (sort by author)",
    "List books (sort by title)",
    "List books (sort by year)",
    "Find books by author",
    "Find books by title",
    "Add book",
    "Delete book",
    "Drop books table and exit",
};

function<void(SQLite& db)> execute[] = {
    Books,
    BooksSortByAuthor,
    BooksSortByTitle,
    BooksSortByYear,
    BooksFindByAuthor,
    BooksFindByTitle,
    BookAdd,
    BookDelete,
    BooksDropTable,
};

void TextMenu(SQLite& db) {
    while(true) {
        cout << endl;
        cout << "Current database " << db.dbname() << ":" << endl;
        int count = 0;
        for (const char* item : menu)
            cout << count++ << ": " << item << endl;

        string input;
        int item;
        cout << "Select an action or 'q' to quit > ";
        while (true) {
            getline(cin, input);
            if (stringstream(input) >> item) {
                if ((item >=0) && (item <=count)) break;
            }
            else
                if (input == "q") exit(0);
            cout << "Incorrect input, try again > ";
        }

        execute[item](db);
    }    
}

void Books(SQLite& db) {
    cout << "List books:" << endl;
    db.select(queryBooks);
    db.show();
}

void BooksSortByAuthor(SQLite& db) {
    cout << "List books (sort by author):" << endl;
    db.select(queryBooksSortByAuthor);
    db.show();
}

void BooksSortByTitle(SQLite& db) {
    cout << "List books (sort by title):" << endl;
    db.select(queryBooksSortByTitle);
    db.show();
}

void BooksSortByYear(SQLite& db) {
    cout << "List books (sort by year):" << endl;
    db.select(queryBooksSortByYear);
    db.show();
}

void BooksFindByAuthor(SQLite& db) {
    string last_name;
    cout << "Find books by author last name > ";
    getline(cin, last_name);
    db.select(queryBooksFindByAuthor, last_name.data());
    db.show();
}

void BooksFindByTitle(SQLite& db) {
    string title;
    cout << "Find books by title > ";
    getline(cin, title);
    db.select(queryBooksFindByTitle, title.data());
    db.show();
}

void BookAdd(SQLite& db) {
    string title;
    string first_name;
    string last_name;
    string year;
    cout << "Add book:" << endl;
    cout << "Title > ";
    getline(cin, title);
    cout << "Author first name > ";
    getline(cin, first_name);
    cout << "Author last name > ";
    getline(cin, last_name);
    cout << "Book finished year > ";
    getline(cin, year);

    if (!db.execute(queryBookAdd, title.data(), first_name.data(), last_name.data(), year.data()))
        db.message("Could not add row.");
}

void BookDelete(SQLite& db) {
    string id;
    cout << "Delete book with id > ";
    getline(cin, id);
    if (!db.execute(queryBookDelete, id.data()))
        db.message("Could not delete row.");
}

void BooksDropTable(SQLite& db) {
     db.execute(queryBooksDropTable);
     exit(0);
}

int main() {
    string filename;
    cout << "Create (or open if exists) database:" << endl;
    cout << "Database filename (default books.db) > ";
    getline(cin, filename);
    if (filename.empty()) {
        filename = "books.db";
    }

    SQLite db(filename);
     if (db.value(queryCheckBookTable).empty()) {
         db.execute(queryCreateTable);
     }

    TextMenu(db);

    return 0;
}
