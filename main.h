#ifndef SQL_CPP_MAIN_H
#define SQL_CPP_MAIN_H

#include "wrapper.h"

void TextMenu(SQLite&);
void DisplayData(SQLite&);

void Books(SQLite&);
void BooksSortByAuthor(SQLite&);
void BooksSortByTitle(SQLite&);
void BooksFindByAuthor(SQLite&);
void BooksFindByTitle(SQLite&);
void BookAdd(SQLite&);
void BookDelete(SQLite&);
void BooksDropTable(SQLite&);

#endif
