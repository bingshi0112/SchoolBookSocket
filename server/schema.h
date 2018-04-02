#ifndef SCHEMA_H_
#define SCHEMA_H_

typedef enum {
    PER, COURSE, COURSE_STUDENTS_RLTN, SEM, FILESTORE
} TABLE_NAME;

typedef enum {
    INTEGER, VARCHAR
} COLUMN_TYPE;

typedef enum {
    AND, OR, COMMA, END
} OPERATOR;

typedef enum {
    EQUAL, MORE_THAN, LESS_THAN, NOT_LESS_THAN, NOT_MORE_THAN,IN
} RELATION_OPERATOR;

typedef struct {
    int id;
    char *first_name;
    char *last_name;
    char *email;
    char *password;
    int gender;
    int role;
} PERSON;

typedef struct {
    int id;
    char *department;
    char *number;
    char *semester;
    char *year;
    char *name;
    char *section;
} COURSE_DATA;

typedef struct {
    int courseId;
    int studentId;
} COURSE_STUDENTS_RL;

typedef struct {
    int id;
    char *filename;
    char *filepath;
    int courseId;    
} FILE_STORE;

typedef struct {
    char *column;
    char *value;
    COLUMN_TYPE type;
    OPERATOR oper;
    RELATION_OPERATOR relationOperator;
} FILTER;

typedef struct {
    int size;
    void *result;
} RESULT;

typedef struct {
    char *season;
    char *year;
} SEMESTER;
#endif