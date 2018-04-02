#ifndef DBUTIL_H_
#define DBUTIL_H_

#include <my_global.h>
#include <mysql.h>
#include "schema.h"

MYSQL* getDBConnection(char *ip,char *user, char *password,char *dbname);
void writeTo(char *name, void *userdata);
RESULT readFrom(char *name, FILTER filter[],int filterSize);
char* deleteFrom(char *name, FILTER filter[], int filterSize);
void updateTable(TABLE_NAME name, FILTER setterValues[], int setterSize,FILTER filter[], int filterSize);
void freeResult(char *name,RESULT *res, int size);
RESULT joinFrom(char *name, FILTER filter[],int filterSize);
#endif