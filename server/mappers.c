 //gcc mappers.c mappers.h -I "C:\cygwin64\usr\include\json-c" -ljson-c
#include <stdio.h>
#include <string.h>
#include "mappers.h"

json_object * mapPersonToJson(PERSON *p);
PERSON* mapJsonToPerson(json_object *jsonobj);

json_object* mapCourseToJson(COURSE_DATA *c);
COURSE_DATA* mapJsonToCourse(json_object* jobj);

json_object* mapSemesterToJson(SEMESTER *sem);

json_object* mapFileStoreToJson(FILE_STORE *fs);

json_object* mapFileStoreToJson(FILE_STORE *fs){
    json_object *jfilename = NULL;
    if(fs!=NULL) {
        jfilename = json_object_new_string(fs->filename);
    }
    return jfilename;
}

json_object* mapPersonToJson(PERSON *p) {
    json_object *jobj = NULL;    
    if(p!=NULL) {        
    jobj = json_object_new_object();
    json_object *jid = json_object_new_int(p->id);
    json_object *jfirst_name = json_object_new_string(p->first_name);
    json_object *jlast_name = json_object_new_string(p->last_name);
    json_object *jemail = json_object_new_string(p->email);
    json_object *jpassword = json_object_new_string(p->password);
    json_object *jgender = json_object_new_int(p->gender);
    json_object *jrole = json_object_new_int(p->role);
    
    json_object_object_add(jobj,"id", jid);
    json_object_object_add(jobj,"first_name", jfirst_name);
    json_object_object_add(jobj,"last_name", jlast_name);
    json_object_object_add(jobj,"email", jemail);
    json_object_object_add(jobj,"password", jpassword);
    json_object_object_add(jobj,"gender", jgender);
    json_object_object_add(jobj,"role", jrole);    
    }
    return jobj;    
}
/* PLEASE DONT DELETE THIS CODE MAY NEED IN FUTURE
PERSON* mapJsonToPerson(json_object *jobj) {
    
    PERSON *result= malloc(sizeof(PERSON)) ;    
    json_object_object_foreach(jobj, key, val) {
        if(strcmp("id",key)==0) {
         result->id = json_object_get_int(val);  
        }
        else if(strcmp("first_name",key)==0) {
         result->first_name = json_object_get_string(val);
        }
        else if(strcmp("last_name",key)==0) {
         result->last_name = json_object_get_string(val);
        }
        else if(strcmp("email",key)==0) {
         result->last_name = json_object_get_string(val);
        }
        else if(strcmp("password",key)==0) {
            result->password = json_object_get_string(val);
        }
        else if(strcmp("gender",key)==0) {
            result->gender = json_object_get_int(val);
        }
        else if(strcmp("role",key)==0) {
            result->role = json_object_get_int(val);
        }
    } 
    return result;
    //Don't forget to free PERSON
}*/

json_object* mapCourseToJson(COURSE_DATA *c) {
    json_object * jobj = NULL;
    if(c!=NULL) {        
    jobj = json_object_new_object();
    json_object *jid = json_object_new_int(c->id);
    json_object *jdepartment = json_object_new_string(c->department);
    json_object *jnumber = json_object_new_string(c->number);
    json_object *jsemester= json_object_new_string(c->semester);
    json_object *jyear = json_object_new_string(c->year);
    json_object *jname = json_object_new_string(c->name);
    json_object *jsection = json_object_new_string(c->section);
    
    json_object_object_add(jobj,"id", jid);
    json_object_object_add(jobj,"department", jdepartment);
    json_object_object_add(jobj,"number", jnumber);
    json_object_object_add(jobj,"semester", jsemester);
    json_object_object_add(jobj,"year", jyear);
    json_object_object_add(jobj,"name", jname);
    json_object_object_add(jobj,"section", jsection);    
    }
    return jobj;    
}


/* PLEASE DONT DELETE THIS CODE MAY NEED IN FUTURE
COURSE_DATA* mapJsonToCourse(json_object* jobj) {
    
    COURSE_DATA *result= malloc(sizeof (COURSE_DATA));    
    json_object_object_foreach(jobj, key, val) {
        if(strcmp("id",key)==0) {
         result->id = json_object_get_int(val);  
        }
        else if(strcmp("department",key)==0) {
         result->department = json_object_get_string(val);
        }
        else if(strcmp("number",key)==0) {
         result->number = json_object_get_string(val);
        }
        else if(strcmp("semester",key)==0) {
         result->semester = json_object_get_string(val);
        }
        else if(strcmp("year",key)==0) {
            result->year = json_object_get_string(val);
        }
        else if(strcmp("name",key)==0) {
            result->name = json_object_get_string(val);
        }
        else if(strcmp("section",key)==0) {
            result->section = json_object_get_string(val);
        }
    } 
    return result;
    //Don't forget to free COURSE_DATA
}*/


json_object* mapCourseStudentRelToJson(COURSE_STUDENTS_RL *c) {
    json_object * jobj = NULL;
    if(c!=NULL) {        
    jobj = json_object_new_object();
    json_object *jcourseId = json_object_new_int(c->courseId);
    json_object *jstudentId = json_object_new_int(c->studentId);
    
    json_object_object_add(jobj,"courseId", jcourseId);
    json_object_object_add(jobj,"studentId", jstudentId);
    
    }
    return jobj;    
}


char* mapResutToJsonString(RESULT *res,TABLE_NAME name) {
    
json_object *jobj = json_object_new_object();
json_object *jarray = json_object_new_array();

for(int i=0; i < res->size;i++) {
    
switch (name) {
        case PER:
            json_object_array_add(jarray, mapPersonToJson( &(((PERSON *)res->result)[i]) ));            
            break;
        case COURSE:
            json_object_array_add(jarray, mapCourseToJson( &(((COURSE_DATA *)res->result)[i]) ));            
            break;
        case COURSE_STUDENTS_RLTN:
            json_object_array_add(jarray, mapCourseStudentRelToJson( &(((COURSE_STUDENTS_RL *)res->result)[i]) ));            
            break;
        case SEM:
            json_object_object_add(jobj, "result", mapSemesterToJson( &((SEMESTER *)res->result)[i] ));
            break;
        case FILESTORE:
            json_object_array_add(jarray,mapFileStoreToJson( &((FILE_STORE *)res->result)[i] ));
            break;
    }
}
 //{"result":[jobj1, job2, jobj3]}
if(name!=SEM) {
json_object_object_add(jobj,"result", jarray);
}
//printf("\n JSON string %s", json_object_to_json_string(jobj));
return json_object_to_json_string(jobj);
}

json_object* mapSemesterToJson(SEMESTER *sem) {
 json_object * jobj = NULL;
 
    if(sem!=NULL) {        
    jobj = json_object_new_object();
    json_object *jseason = json_object_new_string(sem->season);
    json_object *jyear = json_object_new_string(sem->year);
    json_object_object_add(jobj,"season", jseason);
    json_object_object_add(jobj,"year", jyear);
    }
   return jobj;   
}

/* PLEASE DONT DELETE THIS CODE MAY NEED IN FUTURE
SEMESTER* mapSemesterToJson(char *jsonString) {
 json_object *jobj = json_tokener_parse(stringJSON);
 
 SEMESTER *result = malloc(sizeof(SEMESTER));
 
 json_object_object_foreach(jobj, key, val) {
        if(strcmp("season",key)==0) {
         result->season = json_object_get_string(val);
        }
        else if(strcmp("year",key)==0) {
         result->year = json_object_get_string(val);
        }        
    } 
   //Free the result afte use; 
  return result;  
}*/

/*RESULT mapJsonStringToResult(char *jsonString, TABLE_NAME name) {
    json_object* jobj = json_tokener_parse(string);
    enum json_type type = json_object_get_type(jobj);
 
 switch (type) {
 case json_type_null: 
 printf("json_type_nulln");
 break;
 case json_type_boolean: 
 printf("json_type_booleann");
 break;
 case json_type_double: 
 printf("json_type_doublen");
 break;
 case json_type_int: 
 printf("json_type_intn");
 break;
 case json_type_object: 
 printf("json_type_objectn"); 
 //{"result" :[]}
 if(name==PER) {
     
 }
 else if(name==)
 
 
 fflush(stdout);
 break;
 case json_type_array: 
 printf("json_type_arrayn");
 break;
 case json_type_string: 
 printf("json_type_stringn");
 break;
 default:
 printf("\n type did not match");
 }
    
    
}*/

char* sendErrorJson(char* message) {
    json_object * jobj = NULL;
    if(message!=NULL) {        
    jobj = json_object_new_object();
    json_object *jerror = json_object_new_string(message);
    json_object_object_add(jobj,"error", jerror);    
    }
   return json_object_to_json_string(jobj);;   
}

char* sendMessageJson(char* key, char* message) {
    json_object * jobj = NULL;
    if(message!=NULL && key!=NULL) {        
    jobj = json_object_new_object();
    json_object *jmsg = json_object_new_string(message);
    json_object_object_add(jobj, key, jmsg);
    }
   return json_object_to_json_string(jobj);
}

char* sendResultObject(char *filename, char *status) {
    json_object * jobj = NULL;
    
    if(filename!=NULL && status!=NULL) {
        jobj = json_object_new_object();
        json_object * innerObject = json_object_new_object();
        json_object *file_name = json_object_new_string(filename);
        json_object *file_status = json_object_new_string(status);
        json_object_object_add(innerObject, "filename", file_name);
        json_object_object_add(innerObject, "status", file_status);
        json_object_object_add(jobj, "result", innerObject);
    }
    
    return json_object_to_json_string(jobj);
}

/*
int main(int argc, char *argv[]) {
   //printf("Compiled\n\n");
   
   PERSON *p = malloc(sizeof(PERSON)*2);
   p[0].id=1; p[0].first_name="test1"; p[0].last_name="test1"; p[0].email="test1"; p[0].password="test1"; p[0].gender=1; p[0].role=1;
   p[1].id=1; p[1].first_name="test2"; p[1].last_name="test2"; p[1].email="test2"; p[1].password="test2"; p[1].gender=1; p[1].role=1;
   RESULT res;
   res.size=2; res.result=(void*)p;
   mapResutToJsonString(&res,PER);
   
   SEMESTER s;
   s.season="test1";
   s.year="test2";
   RESULT res1;
   res1.size=1;
   res1.result=&s;
   printf("\n\n\n");
   mapResutToJsonString(&res1,SEM);
   
   COURSE_DATA *c = malloc(sizeof(COURSE_DATA)*2);
   c[0].id=1; c[0].department="test1"; c[0].number="test2"; c[0].semester="test3"; c[0].year="test4"; c[0].name="test5"; c[0].section="test6";
   c[1].id=1; c[1].department="test1"; c[1].number="test2"; c[1].semester="test3"; c[1].year="test4"; c[1].name="test5"; c[1].section="test6";
   RESULT res2;
   printf("\n\n\n");
   res2.size=2; res2.result=(void*)c;
   mapResutToJsonString(&res2, COURSE);
   
   COURSE_STUDENTS_RL *csrln = malloc(sizeof(COURSE_STUDENTS_RL)*2);
   csrln[0].courseId=1; csrln[0].studentId=1;
   csrln[1].courseId=2; csrln[1].studentId=2;
   RESULT res3;
   printf("\n\n\n");
   res3.size=2; res2.result=(void*)csrln;
   mapResutToJsonString(&res2, COURSE_STUDENTS_RLTN);
   
   
return 0; 
}*/