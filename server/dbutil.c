
#include <stdio.h>
#include <string.h>
#include "dbutil.h"

char *insertPerson = "INSERT INTO person (first_name,last_name,email,password,gender,role) values( ";
char *insertCourse = "INSERT INTO course (department,number,semester,year,name,section) values( ";
char *insertCourseStudnets = "INSERT INTO course_students_rltn (courseId, studentId) values( ";
char *insertFilestore = "INSERT INTO filestore (filename, filepath, courseId) values( ";

char *sel = "SELECT * from ";
char *update = "UPDATE ";
char *del = "DELETE from ";

char *getTableName(TABLE_NAME name);

char *getOperName(OPERATOR name);

void retPersonDOM(MYSQL_RES *result, PERSON personArray[], int size);

void retCourseDOM(MYSQL_RES *result, COURSE_DATA courseArray[], int size);

void retCourseStudentRelDOM(MYSQL_RES *result, COURSE_STUDENTS_RL courseStudentArray[], int size);

void retFileStoreDOM(MYSQL_RES *result, FILE_STORE fs[], int rows);

void freeResult(char *name, RESULT *res, int size);

void freePerson(PERSON *person);

void freeCourse(COURSE_DATA *courseData);

void freeCourseStudentRel(COURSE_STUDENTS_RL *courseStudentRel);

void freeFileStore(FILE_STORE *fs);

char *whereClauseAppender(char *query, FILTER *filter, int filterSize);

void print_error(MYSQL *myConn);

const char *user = "root";

const char *password = "root"; //"" 900112

const char *databaseName = "207_project";//;//"testdb"

//static MYSQL *conn = getDBConnection("127.0.0.1", user, password, databaseName);

//I should be able to get connection
MYSQL *getDBConnection(char *ip, char *user, char *password, char *dbname) {
//    printf("MySQL client version: %s\n", mysql_get_client_info());
    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        print_error(conn);
        exit(1);
    }

    if (mysql_real_connect(conn, ip, user, password, dbname, 0, NULL, 0) == NULL) {
        print_error(conn);
        exit(1);
    }
    return conn;
}


void writeTo(char *name, void *userdata) {
    MYSQL *conn = getDBConnection("127.0.0.1", user, password, databaseName);
    const char *tableName = name;
//    printf("table name is %s", tableName);
    char *query = NULL;
    if (strcmp(tableName, "person") == 0) {
        PERSON *p = (PERSON *) userdata;
        int gender = p->gender;
        char gen[15];
        sprintf(gen, "%d", gender);
        int role = p->role;
        char rol[15];
        sprintf(rol, "%d", role);
        int totalLength = strlen(insertPerson) + strlen(p->first_name) + strlen(p->last_name) + strlen(p->email) +
                          strlen(p->password) + strlen(gen) + strlen(rol) + 8 + 5 + 1;
        query = malloc(totalLength + 1);
        memset(query, '\0', totalLength + 1);
        strcpy(query, insertPerson);
        strcat(query, "'");
        strcat(query, p->first_name);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, "'");
        strcat(query, p->last_name);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, "'");
        strcat(query, p->email);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, "'");
        strcat(query, p->password);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, gen);
        strcat(query, ",");
        strcat(query, rol);
        strcat(query, ")");
        printf("\nQuery is: %s\n", query);
        fflush(stdout);
    } else if (strcmp(tableName, "course") == 0) {
        COURSE_DATA *c = (COURSE_DATA *) userdata;

        int totalLength = strlen(insertCourse) + strlen(c->department) + strlen(c->number) + strlen(c->semester) +
                          strlen(c->year) + strlen(c->name) + strlen(c->section) + 12 + 5 + 1;
        query = malloc(totalLength + 1);
        memset(query, '\0', totalLength + 1);
        strcpy(query, insertCourse);
        strcat(query, "'");
        strcat(query, c->department);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, "'");
        strcat(query, c->number);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, "'");
        strcat(query, c->semester);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, "'");
        strcat(query, c->year);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, "'");
        strcat(query, c->name);
        strcat(query, "'");
        strcat(query, ",");
        strcat(query, "'");
        strcat(query, c->section);
        strcat(query, "'");
        strcat(query, ")");
        printf("Query is: %s\n", query);
        fflush(stdout);
    } else if (strcmp(tableName, "course_students_rltn") == 0) {
        COURSE_STUDENTS_RL *c = (COURSE_STUDENTS_RL *) userdata;
        int courseid = c->courseId;
        char crsid[15];
        sprintf(crsid, "%d", courseid);
        int studentid = c->studentId;
        char stuid[15];
        sprintf(stuid, "%d", studentid);

        int totalLength = strlen(insertCourseStudnets) + strlen(crsid) + strlen(stuid) + 2 + 1 + 1;
        query = malloc(totalLength + 1);
        memset(query, '\0', totalLength + 1);
        strcpy(query, insertCourseStudnets);
        strcat(query, crsid);
        strcat(query, ",");
//        strcat(query, "'");
        strcat(query, stuid);
//        strcat(query, "'");
        strcat(query, ")");
        printf("Query is: %s\n", query);
        fflush(stdout);
    } else if(strcmp(tableName, "filestore") == 0) {
        FILE_STORE *s = (FILE_STORE *) userdata; //filename, filepath, courseId
        int courseid = s->courseId; char crsId[15]; sprintf(crsId, "%d", courseid);
        int totalLength = strlen(insertFilestore) + strlen(s->filename) + strlen(s->filepath) + strlen(crsId) + 2 + 1 + 2 + 1 + 1;
        query = malloc(totalLength+1); memset(query,'\0',totalLength+1);
        strcpy(query, insertFilestore); 
        strcat(query, "'"); strcat(query, s->filename); strcat(query,"'"); strcat(query,","); printf("\n\n 1 Query is: %s\n", query);
        fflush(stdout);
        strcat(query, "'"); strcat(query, s->filepath); strcat(query,"'"); strcat(query,","); printf("\n\n 2 Query is: %s\n", query);
        fflush(stdout);
        strcat(query, crsId); strcat(query, ")"); 
        printf("Query is: %s\n", query);
        fflush(stdout);
    }

    if (mysql_query(conn, query)) {
        int errNo = mysql_errno(conn);
                
        if(strcmp(tableName, "filestore") == 0 && errNo==1062) {
            //update  entry
            FILE_STORE *s = (FILE_STORE *) userdata;
            //void updateTable(TABLE_NAME name, FILTER setterValues[], int setterSize, FILTER filter[], int filterSize)
            FILTER set_clause[1];
            set_clause[0].column="filepath"; 
            set_clause[0].value=s->filepath; 
            set_clause[0].type=VARCHAR; 
            set_clause[0].relationOperator=EQUAL; 
            set_clause[0].oper=END;
            
            FILTER where_update_duplicates[2];
            where_update_duplicates[0].column="filename"; 
            where_update_duplicates[0].value=s->filename; 
            where_update_duplicates[0].type=VARCHAR; 
            where_update_duplicates[0].relationOperator=EQUAL; 
            where_update_duplicates[0].oper=AND;
            where_update_duplicates[1].column="courseId";  char crsId[15]; sprintf(crsId, "%d", s->courseId);
            where_update_duplicates[1].value=crsId; 
            where_update_duplicates[1].type=INTEGER; 
            where_update_duplicates[1].relationOperator=EQUAL; 
            where_update_duplicates[1].oper=END;
            updateTable(FILESTORE, set_clause, 1, where_update_duplicates, 2);                      
        }
        else {
        print_error(conn);
        exit(1);        
        }        
    }
    mysql_close(conn);
    free(query);
}

RESULT readFrom(char *name, FILTER filter[], int filterSize) {
    const char *tableName = name;
//    printf("table name is %s", tableName);
    MYSQL *conn = getDBConnection("127.0.0.1", user, password, databaseName);
    char *query = NULL;
    int length = strlen(sel) + strlen(tableName) + 1;
    query = malloc(length);
    memset(query, '\0', length);
    strcpy(query, sel);
    strcat(query, tableName);
    //copied from here******
    if (filter != NULL) {
        length = length + sizeof(" where");
        query = realloc(query, length);
        strcat(query, " where");
        query = whereClauseAppender(query, filter, filterSize);
    }

    printf("Query is: %s\n", query);
    fflush(stdout);
    if (mysql_query(conn, query)) {
        print_error(conn);
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        print_error(conn);
        exit(1);
    }

    int num_fields = mysql_num_fields(result);
    unsigned long rows = (unsigned long) mysql_num_rows(result);//my_ulonglong rows
    MYSQL_ROW row;
    RESULT res;

    if (strcmp(name, "person") == 0) {
        ///PERSON personArray[rows];
        PERSON *personArray = malloc(sizeof(PERSON) * rows);
        memset(personArray, 0, sizeof(PERSON) * rows);
        retPersonDOM(result, personArray, rows);
        res.size = rows;
        res.result = personArray;
    } else if (strcmp(name, "course") == 0) {
        //COURSE_DATA courseArray[rows];
        COURSE_DATA *courseArray = malloc(sizeof(COURSE_DATA) * rows);
        memset(courseArray, 0, sizeof(COURSE_DATA) * rows);
        retCourseDOM(result, courseArray, rows);
        //printf("\n ok %s",courseArray[0].department);fflush(stdout); printf("\n\n");fflush(stdout);
        //printf("\n not ok %s",courseArray[1].department);fflush(stdout);
        res.size = rows;
        res.result = courseArray;
    } else if (strcmp(name, "course_students_rltn") == 0) {

        COURSE_STUDENTS_RL *courseStudentArray = malloc(sizeof(COURSE_STUDENTS_RL) * rows);
        memset(courseStudentArray, 0, sizeof(COURSE_STUDENTS_RL) * rows);
        retCourseStudentRelDOM(result, courseStudentArray, rows);
        res.size = rows;
        res.result = courseStudentArray;
    } else if(strcmp(name, "filestore") == 0) {
        FILE_STORE *fs = malloc(sizeof(FILE_STORE)*rows);
        memset(fs, 0, sizeof(FILE_STORE)*rows);
        retFileStoreDOM(result, fs, rows);
        res.size = rows;
        res.result = fs;
    }
    free(query);
    mysql_close(conn);
    return res;
}

char *deleteFrom(char *name, FILTER filter[], int filterSize) {
    const char *tableName = name;
//    printf("table name is %s", tableName);
    MYSQL *conn = getDBConnection("127.0.0.1", user, password, databaseName);
    char *query = NULL;

    int length = strlen(del) + strlen(tableName) + 1;
    query = malloc(length);
    memset(query, '\0', length);
    strcpy(query, del);
    strcat(query, tableName);

    if (filter != NULL) {
        length = length + sizeof(" where");
        query = realloc(query, length);
        strcat(query, " where");
        query = whereClauseAppender(query, filter, filterSize);
    }
    printf("Query is: %s\n", query);
    fflush(stdout);
//    RESULT res = readFrom(name, filter, filterSize);
    if (mysql_query(conn, query)) {
        print_error(conn);
        exit(1);
    }
    free(query);
    mysql_close(conn);
    return "success";
}

void updateTable(TABLE_NAME name, FILTER setterValues[], int setterSize, FILTER filter[], int filterSize) {
    const char *tableName = getTableName(name);
    printf("table name is %s", tableName);
    MYSQL *conn = getDBConnection("127.0.0.1", user, password, databaseName);
    char *query = NULL;
    int length = strlen(update) + strlen(tableName) + 1;
    query = malloc(length);
    memset(query, '\0', length);
    strcpy(query, update);
    strcat(query, tableName);

    if (setterValues != NULL) {
        length = length + sizeof(" SET");
        query = realloc(query, length);
        strcat(query, " SET");
        query = whereClauseAppender(query, setterValues, setterSize);
        length = strlen(query);
    }

    printf("\nAfter SET : %s %d", query, strlen(query));

    if (filter != NULL) {
        length = length + sizeof(" where");
        query = realloc(query, length);
        strcat(query, " where");
        query = whereClauseAppender(query, filter, filterSize);
    }
    if (mysql_query(conn, query)) {
        print_error(conn);
        exit(1);
    }
    printf("\n\n Query is : %s",query); fflush(stdout);
    free(query);
    mysql_close(conn);
}

RESULT joinFrom(char *name, FILTER filter[], int filterSize) {
    const char *tableName = name;
//    printf("table name is %s", tableName);
    MYSQL *conn = getDBConnection("127.0.0.1", user, password, databaseName);
    char *query = NULL;
    char *otherTable = NULL;
    int lengthOtherTable = 1;

//    otherTable = malloc(lengthOtherTable);
//    memset(otherTable, '\0', lengthOtherTable);
    int length = strlen(sel) + strlen(tableName) + 1;
    query = malloc(length);
    memset(query, '\0', length);
    strcpy(query, sel);
    strcat(query, tableName);
    if (strcmp(tableName, "person") == 0) {
        length += strlen(" p, course c, course_students_rltn r where c.id = r.courseId AND p.id = r.studentId");
        query = realloc(query, length);
        strcat(query, " p, course c, course_students_rltn r where c.id = r.courseId AND p.id = r.studentId");
    }
    if (strcmp(tableName, "course") == 0) {
        length = length + strlen(" c, person p, course_students_rltn r where c.id = r.courseId AND p.id = r.studentId");
        query = realloc(query, length);
        strcat(query, " c, person p, course_students_rltn r where c.id = r.courseId AND p.id = r.studentId");
    }
//    length = length + sizeof(otherTable);
//    query = realloc(query, length);
//    strcat(query, otherTable);
    if (filter != NULL) {
        length = length + strlen(" AND");
        query = realloc(query, length);
        strcat(query, " AND");
        query = whereClauseAppender(query, filter, filterSize);
    }
    printf("Query is: %s\n", query);
    fflush(stdout);
    if (mysql_query(conn, query)) {
        print_error(conn);
        exit(1);
    }
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        print_error(conn);
        exit(1);
    }

    int num_fields = mysql_num_fields(result);
    unsigned long rows = (unsigned long) mysql_num_rows(result);//my_ulonglong rows
    MYSQL_ROW row;
    RESULT res;
    if (strcmp(name, "person") == 0) {
        ///PERSON personArray[rows];
        PERSON *personArray = malloc(sizeof(PERSON) * rows);
        memset(personArray, 0, sizeof(PERSON) * rows);
        retPersonDOM(result, personArray, rows);
        res.size = rows;
        res.result = personArray;
    } else if (strcmp(name, "course") == 0) {
        //COURSE_DATA courseArray[rows];
        COURSE_DATA *courseArray = malloc(sizeof(COURSE_DATA) * rows);
        memset(courseArray, 0, sizeof(COURSE_DATA) * rows);
        retCourseDOM(result, courseArray, rows);
        //printf("\n ok %s",courseArray[0].department);fflush(stdout); printf("\n\n");fflush(stdout);
        //printf("\n not ok %s",courseArray[1].department);fflush(stdout);
        res.size = rows;
        res.result = courseArray;
    } else if (strcmp(name, "course_students_rltn") == 0) {
        COURSE_STUDENTS_RL *courseStudentArray = malloc(sizeof(COURSE_STUDENTS_RL) * rows);
        memset(courseStudentArray, 0, sizeof(COURSE_STUDENTS_RL) * rows);
        retCourseStudentRelDOM(result, courseStudentArray, rows);
        res.size = rows;
        res.result = courseStudentArray;
    }
    mysql_close(conn);
    free(otherTable);
    free(query);
    return res;
}

char *whereClauseAppender(char *query, FILTER *filter, int filterSize) {
    int length = strlen(query) + 1;//1 extra for '/0'
    int i = 0;
    for (i = 0; i < filterSize; i++) {
//        printf("\nIn a loop %d", i);
//        fflush(stdout);
        FILTER current = filter[i];
        char *oper = getOperName(current.oper);
        if (current.type == INTEGER) {
            length = length + strlen(current.column) + strlen(current.value) + 1 + 1 +
                     1;// &nbsp column_name=0 &nbsp oper
            query = realloc(query, length);
            // &nbsp column_name=0 &nbsp oper
            strcat(query, " ");
            strcat(query, current.column);
            strcat(query, "=");
            strcat(query, current.value);
            strcat(query, " ");
        } else if (current.type == VARCHAR) {
            length = length + strlen(current.column) + strlen(current.value) + 1 + 1 + 2 +
                     1;// &nbsp column_name='0' &nbsp oper
            query = realloc(query, length);
            // &nbsp column_name='0' &nbsp oper
            strcat(query, " ");
            strcat(query, current.column);
            strcat(query, "=");
            strcat(query, "'");
            strcat(query, current.value);
            strcat(query, "'");
            strcat(query, " ");
        }

        if (oper != NULL) {
//            printf("\nQuery ----> %s",query);fflush(stdout);
//            &nbsp and ELSE &nbsp or
//            printf("\nBefore Query Test ----> %s %d %d", query, length, strlen(query));
//            fflush(stdout);
            length = length + strlen(oper);
//            printf("\n %d %s", length, oper);
//            fflush(stdout);
            query = realloc(query, length);
            if (query == NULL) {
                printf("\n\nrealloc failed");
                fflush(stdout);
            }
            strcat(query, oper);
//            printf("\nAfter Query ----> %s %d %d", query, length, strlen(query));
//            fflush(stdout);
        }
    }
    query[strlen(query)] = '\0';
//    printf("\nAfter Query ----> %s %d %d", query, length, strlen(query));
//    fflush(stdout);
//    printf("\nQuery is: %s", query);
//    fflush(stdout);
    return query;
}

void retPersonDOM(MYSQL_RES *result, PERSON personArray[], int size) {
    MYSQL_ROW row;
    int j = 0;
    while ((row = mysql_fetch_row(result))) {
        personArray[j].id = atoi(row[0]);
        personArray[j].first_name = malloc(strlen(row[1]) + 1);
        memset(personArray[j].first_name, '\0', strlen(row[1]) + 1);
        strcpy(personArray[j].first_name, row[1]);
        personArray[j].last_name = malloc(strlen(row[2]) + 1);
        memset(personArray[j].last_name, '\0', strlen(row[2]) + 1);
        strcpy(personArray[j].last_name, row[2]);
        personArray[j].email = malloc(strlen(row[3]) + 1);
        memset(personArray[j].email, '\0', strlen(row[3]) + 1);
        strcpy(personArray[j].email, row[3]);
        personArray[j].password = malloc(strlen(row[4]) + 1);
        memset(personArray[j].password, '\0', strlen(row[4]) + 1);
        strcpy(personArray[j].password, row[4]);
        personArray[j].gender = atoi(row[5]);
        personArray[j].role = atoi(row[6]);
        j++;
    }

}

void retCourseDOM(MYSQL_RES *result, COURSE_DATA courseArray[], int size) {
    MYSQL_ROW row;
    int j = 0;
    while ((row = mysql_fetch_row(result))) {
        courseArray[j].id = atoi(row[0]);
        courseArray[j].department = malloc(strlen(row[1]) + 1);
        memset(courseArray[j].department, '\0', strlen(row[1]) + 1);
        strcpy(courseArray[j].department, row[1]);
//        printf("Department %s", courseArray[j].department);
//        fflush(stdout);
        courseArray[j].number = malloc(strlen(row[2]) + 1);
        memset(courseArray[j].number, '\0', strlen(row[2]) + 1);
        strcpy(courseArray[j].number, row[2]);
        courseArray[j].semester = malloc(strlen(row[3]) + 1);
        memset(courseArray[j].semester, '\0', strlen(row[3]) + 1);
        strcpy(courseArray[j].semester, row[3]);
        courseArray[j].year = malloc(strlen(row[4]) + 1);
        memset(courseArray[j].year, '\0', strlen(row[4]) + 1);
        strcpy(courseArray[j].year, row[4]);
        courseArray[j].name = malloc(strlen(row[5]) + 1);
        memset(courseArray[j].name, '\0', strlen(row[5]) + 1);
        strcpy(courseArray[j].name, row[5]);
        courseArray[j].section = malloc(strlen(row[6]) + 1);
        memset(courseArray[j].section, '\0', strlen(row[6]) + 1);
        strcpy(courseArray[j].section, row[6]);
        j++;
    }
//    printf("\n REACHED HERE ");
//    fflush(stdout);
}

void retCourseStudentRelDOM(MYSQL_RES *result, COURSE_STUDENTS_RL courseStudentArray[], int size) {
    MYSQL_ROW row;
    int j = 0;
    while ((row = mysql_fetch_row(result))) {
        courseStudentArray[j].courseId = atoi(row[0]);
        courseStudentArray[j].studentId = atoi(row[1]);
//        courseStudentArray[j].studentId = malloc(strlen(row[1]) + 1);
//        memset(courseStudentArray[j].studentId, '\0', strlen(row[1]) + 1);
//        strcpy(courseStudentArray[j].studentId, row[1]);
        j++;
    }
}

void retFileStoreDOM(MYSQL_RES *result, FILE_STORE fs[], int rows) {
    MYSQL_ROW row;
    int j = 0;
    while((row = mysql_fetch_row(result))){
        fs[j].id = atoi(row[0]);
        fs[j].filename = malloc(strlen(row[1])+1); memset(fs[j].filename,'\0', strlen(row[1])+1); strcpy(fs[j].filename, row[1]);
        fs[j].filepath = malloc(strlen(row[2])+1); memset(fs[j].filepath,'\0', strlen(row[2])+1); strcpy(fs[j].filepath, row[2]);
        fs[j].courseId = atoi(row[3]);
        j++;
    }
    
}

char *getTableName(TABLE_NAME name) {
    switch (name) {
        case PER:
            return "person";
        case COURSE:
            return "course";
        case COURSE_STUDENTS_RLTN:
            return "course_students_rltn";
        case FILESTORE:
            return "filestore";
    }
}

char *getOperName(OPERATOR name) {
    switch (name) {
        case AND:
            return "and";
        case OR:
            return "or";
        case COMMA:
            return ",";
        default:
            return NULL;
    }
}


void print_error(MYSQL *myConn) {
    fprintf(stderr, "%s \n", mysql_error(myConn));
    mysql_close(myConn);
}


void freeResult(char *name, RESULT *res, int size) {
    int i = 0;
    if (strcmp(name, "person") == 0) {
        PERSON *personArray = res->result;
//        printf("size:\n%d\n", size);
//        fflush(stdout);
        for (i = 0; i < size; i++) {
//            printf("loop:\n%d\n", i);
//            fflush(stdout);
            freePerson(&personArray[i]);
        }
//        printf("1\n");
//        fflush(stdout);
        free(personArray);
    } else if (strcmp(name, "course") == 0) {
        COURSE_DATA *courseArray = res->result;
        for (i = 0; i < size; i++) {
            freeCourse(&courseArray[i]);
        }
        free(courseArray);
    } else if (strcmp(name, "course_students_rltn") == 0) {
        COURSE_STUDENTS_RL *courseStudentRel = res->result;
        for (i = 0; i < size; i++) {
            freeCourseStudentRel(&courseStudentRel[i]);
        }
        free(courseStudentRel);
    } else if(strcmp(name, "filestore") == 0) {
        FILE_STORE *fs = res->result;
        for (i = 0; i < size; i++) {
            freeFileStore(&fs[i]);
        }
    }
//    printf("\nFree Suceess!");
//    fflush(stdout);
}

void freePerson(PERSON *person) {
    free(person->first_name);
    free(person->last_name);
    free(person->email);
    free(person->password);
}

void freeCourse(COURSE_DATA *courseData) {
    free(courseData->department);
    free(courseData->number);
    free(courseData->semester);
    free(courseData->year);
    free(courseData->name);
    free(courseData->section);
}

void freeCourseStudentRel(COURSE_STUDENTS_RL *courseStudentRel) {
    free(courseStudentRel->studentId);
}

void freeFileStore(FILE_STORE *fs){
    free(fs->filename);
    free(fs->filepath);
}

//int main(int argc, char **argv)
//{
    /*PERSON p;
    p.first_name="test2";
    p.last_name="test2";
    p.email="test2@gmail.com";
    p.password="test2@gmail.com";
    p.gender=0;
    p.role=1;
    
    COURSE_DATA c;
    c.department ="test";
    c.number="test1";
    c.semester="test2";
    c.year="test3";
    c.name="test4";
    c.section="test5";
    
    COURSE_STUDENTS_RL d;
    d.courseId=1;
    d.studentId="007";
    //test(&p);
    TABLE_NAME name = PER;
    name = COURSE_STUDENTS_RLTN;
    name = COURSE;
    //writeTo(name, (void *) &c);
    
    //write to
    FILTER simple[1];
    simple[0].column="courseId";
    simple[0].value="1";
    simple[0].type=INTEGER;
    simple[0].oper=END;*/
    
  /*  FILE_STORE userdata;
    userdata.filename = "json_in_c.txt";
    userdata.filepath = "E:/MS/FinalYear/207 material/Project/clientfiles/json_in_c.txt";
    userdata.courseId = 4;
    writeTo("filestore", &userdata);*/
    
    /*FILTER fileFilter[1];
    fileFilter[0].column="filename";
    fileFilter[0].value="json_in_c.txt";
    fileFilter[0].type=VARCHAR;
    fileFilter[0].relationOperator=EQUAL;
    fileFilter[0].oper=END;
    RESULT res = readFrom("filestore",fileFilter,1);
    FILE_STORE *fs = (FILE_STORE *) res.result;
    
    int i=0;
    for(i=0;i< res.size;i++) {
     printf("\n-----------> %s", fs[i].filepath);fflush(stdout);
    }
    freeResult("filestore", &res, res.size);*/
    /*simple[1].column="first_name";
    simple[1].value="test2";
    simple[1].type=VARCHAR;
    simple[1].oper=END;*/
/* RESULT res =readFrom(COURSE_STUDENTS_RLTN,simple,1);
 int i=0;
 COURSE_STUDENTS_RL *p_arr = (COURSE_STUDENTS_RL *)res.result;
 printf("\n Printing Result\n");fflush(stdout);
 for(i=0;i< res.size;i++) {
     printf("\n-----------> %s",p_arr[i].studentId);fflush(stdout);
 }
 freeResult(COURSE_STUDENTS_RLTN,&res,res.size);*/
 //return 0;
//}
