// gcc -o server.o Server.c dbutil.c -I "C:/Program Files/MySQL/MySQL Connector.C 6.1/include" -L "C:/Program Files/MySQL/MySQL Connector.C 6.1/lib" -lmysqlclient -lz
/*gcc Server.c dbutil.c mappers.c -I "C:/Program Files/MySQL/MySQL Connector.C 6.1/include" -I "C:\cygwin64\usr\include\json-c" -L "C:/Program Files/MySQL/MySQL Connector.C 6.1/lib" -lmysqlclient -ljson-c -lz*/
/*gcc Server.c encryption.c dbutil.c mappers.c -I "C:/Program Files/MySQL/MySQL Connector.C 6.1/include" -I "C:\cygwin64\usr\include\json-c" -L "C:/Program Files/MySQL/MySQL Connector.C 6.1/lib" -l libmcrypt -lmysqlclient -ljson-c -lz*/
#include "dbutil.h"
#include "mappers.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>

#include <stdarg.h>
#include <my_global.h>
#include <mysql.h>

#include <arpa/inet.h>

//thread library
#include<pthread.h>

//Security
#include <mcrypt.h>
#include <math.h>
#include <stdint.h>
#include "encryption.h"

#define QLEN 5 /* maximum connection queue length */
#define BUFSIZE 4096
#define HEADER_SIZE 16

/*
 * Security Credentials
 * */
char* IV = "AAAAAAAAAAAAAAAA";
char *key = "0123456789abcdef";
int keysize = 16; /* 128 bits */

/*Please Edit this Path as per your Server file repository*/
const char *SERVER_FILE_PATH = "/media/lam/LAM/SJSU/Spring2017/207/project/serverfiles/";
extern int errno;

u_short portbase = 0; /* port base, for non-root servers */

int errexit(const char *format, ...);

int passivesock(const char *service, int qlen);

void createPerson(int sock, char *inputs[]);

void createCourse(int sock, char *inputs[]);

int commandNotFound(int s, char *buf);

int sendMessage(int s, char *message);

char *getMessage(int s);

void * clientHandler(void * arg);

char* createDir(char *courseId, char *fileName);

int getFileSize(char* name);

struct SEMESTER {
    char *year;
    char *season;
};

struct SEMESTER getCurrentSemester();

/*--------------------------------------------------MAIN--------------------------------------------------*/
int main(int argc, char *argv[]) {
    char *service = "9201"; /* service name or port number */
    struct sockaddr_in fsin; /* the address of a client */
    int alen, msock; /* master server socket */
    int ssock; /* slave server socket */
//    char buf[BUFSIZ];
//    char fileBuf[BUFSIZ];
//    int n;

    msock = passivesock(service, QLEN);

    while (1) {
        alen = sizeof(fsin);
        ssock = accept(msock, (struct sockaddr *) &fsin, &alen);        
        printf("connection accepted by server for %d\n\n", ssock); fflush(stdout);
        if (ssock < 0)
            errexit("accept failed: %s\n", strerror(errno));
        else {
        pthread_t clientthread;
        if(pthread_create(&clientthread,NULL,clientHandler,(void *)&ssock) !=0) {
            printf("\n\nerror is spawning thread\n\n");  fflush(stdout);
        }
        //printf("A thread has been spawned!!!");
        fflush(stdout);
        }
    }
}

void * clientHandler(void * arg) {
       //args needed ssock, 
       int ssock = *((int *)arg);
       
        while (1) {   
            char *buf = getMessage(ssock);
            //printf("command received is: %s \n", buf);
            //fflush(stdout);
            char *token = strtok(buf, " ");
            char *inputs[20];
            int i = 0;
            while (token != NULL) {
                inputs[i++] = token;
                token = strtok(NULL, " ");
            }
            //printf("------->\n\n %s , %s, size: %d \n\n",inputs[0],inputs[1], i); fflush(stdout);
            if (i > 0) {
                
                if (strcmp(inputs[0], "current") == 0) {
                    if(i==1) {
                        struct SEMESTER cursem;
                        cursem = getCurrentSemester();
                        //CHANGED TO SEND JSON
                        RESULT currentsem_res; currentsem_res.size=1; currentsem_res.result=(void *)&cursem;
                        char *json = mapResutToJsonString(&currentsem_res, SEM);
                        char *messageSent = malloc(strlen(json)+1); 
                        memset(messageSent, '\0', strlen(json)+1);
                        strcpy(messageSent,json);              
                        //printf("sending JSON: %s",messageSent);
                        sendMessage(ssock, messageSent);
                        printf("Result sent to client is:\n%s\n", messageSent);
                        fflush(stdout);
                        free(messageSent);
                    } else {
                        printf("Wrong number of commands!\n\n"); fflush(stdout);
                        commandNotFound(ssock, "Wrong number of commands");
                    }
                }
                else if (strcmp(inputs[0], "select") == 0) {
                    
                    if ((strcmp(inputs[1], "person") == 0 || strcmp(inputs[1], "course") == 0) &&
                        ((i - 2) % 3 == 0 || ((i - 3) % 3 == 0 && strcmp(inputs[i - 1], "current") == 0))) {
                            
                        char *messageSent = NULL;
                        int length = 1;
                        messageSent = malloc(length);
                        memset(messageSent, '\0', length);
                        int filterSize = (i - 2) / 3;
                        RESULT res;
                        char *tableName = inputs[1];
                        if (i == 2) {
//select person             
//select course
                            res = readFrom(tableName, NULL, filterSize);
                        } else {
//select course current
//select person role is 1
//select person role is 0
                            struct SEMESTER cursem;
                            if (strcmp(inputs[i - 1], "current") == 0) {
                                filterSize = filterSize + 2;
                                cursem = getCurrentSemester();
                            }
                            FILTER filters[filterSize];
                            for (int j = 0; j < filterSize; j++) {
                                if ((strcmp(inputs[i - 1], "current") == 0) &&
                                    (j == (filterSize - 1) || j == (filterSize - 2))) {
                                    filters[filterSize - 2].column = "semester";
                                    filters[filterSize - 2].value = cursem.season;
                                    filters[filterSize - 2].type = VARCHAR;
                                    filters[filterSize - 1].column = "year";
                                    filters[filterSize - 1].value = cursem.year;
                                    filters[filterSize - 1].type = VARCHAR;
                                } else {
                                    filters[j].column = inputs[j * 3 + 2];
                                    filters[j].value = inputs[j * 3 + 4];
                                    if (strcmp(inputs[j * 3 + 2], "id") == 0 ||
                                        strcmp(inputs[j * 3 + 2], "gender") == 0 ||
                                        strcmp(inputs[j * 3 + 2], "role") == 0) {
                                        filters[j].type = INTEGER;
                                    } else {
                                        filters[j].type = VARCHAR;
                                    }

                                }
                                if (j == (filterSize - 1)) {
                                    filters[j].oper = END;
                                } else {
                                    filters[j].oper = AND;
                                }
                                //TODO: other operators
//                                if (strcmp(inputs[j * 3 + 3], "is") == 0) {
//                                    filters[j].relationOperator = EQUAL;
//                                }
                                //TODO: other relation opoerators

                            }
                            res = readFrom(tableName, filters, filterSize);
                        }
                        if (strcmp(inputs[1], "person") == 0) {
                            
                            char* json = mapResutToJsonString(&res, PER);
                            messageSent = realloc(messageSent,strlen(json)+1); 
                            memset(messageSent, '\0', strlen(json)+1);
                            strcpy(messageSent,json);                                
                            //printf("MESSAGE USING JSON-C  %s",messageSent);
                        }
                        if (strcmp(inputs[1], "course") == 0) {
//select course department is CMPE
                            char* json = mapResutToJsonString(&res, COURSE);
                            messageSent = realloc(messageSent, strlen(json)+1); 
                            memset(messageSent, '\0', strlen(json)+1);
                            strcpy(messageSent,json);              
                            //printf("MESSAGE USING JSON-C  %s",messageSent);                            
                        }
                        sendMessage(ssock, messageSent);
                        printf("\n\n Result sent to client is h:\n%s \n\n", messageSent);
                        fflush(stdout);
                        
                        free(messageSent);
                        freeResult(tableName, &res, res.size);
                    } else if(strcmp(inputs[1], "filestore")==0) {
//select filestore courseId is 1
                            char *json = NULL;
                            char *messageSent=NULL;
                            RESULT res;
                            if(i==5 && strcmp(inputs[2], "courseId")==0 && strcmp(inputs[3], "is")==0) {
                                FILTER fileFilter[1];
                                fileFilter[0].column="courseId";
                                fileFilter[0].value=inputs[4];
                                fileFilter[0].type=INTEGER;
                                fileFilter[0].relationOperator=EQUAL;
                                fileFilter[0].oper=END;
                                res = readFrom("filestore",fileFilter,1);
                                json = mapResutToJsonString(&res, FILESTORE);                                
                            } else {
                                json = sendErrorJson("COMMAND NOT SUPPORTED");
                            }
                                messageSent = malloc(strlen(json)+1);
                                memset(messageSent, '\0', strlen(json)+1);
                                strcpy(messageSent,json);
                                printf("\n MESSAGE USING JSON-C  %s \n",messageSent);
                                sendMessage(ssock, messageSent);
                                printf("\n Result sent to client is: %s \n", messageSent);
                                fflush(stdout);
                                free(messageSent);
                                if(res.size > 0){
                                freeResult("filestore", &res, res.size);                                
                                }         
                    } else {
                        printf("\n\n Wrong table name or wrong number of commands! \n\n"); fflush(stdout);
                        commandNotFound(ssock, "Wrong table name or wrong number of commands");
                    }
                }
               else if (strcmp(inputs[0], "insert") == 0) {
                    if (i == 8) {
                        if (strcmp(inputs[1], "person") == 0) {
//insert person Lam Tran lamtran@gmail.com 910209 2 1
                            printf("\n\n Will create a new user.\n\n");
                            fflush(stdout);
                            createPerson(ssock, inputs);
                        } else if (strcmp(inputs[1], "course") == 0) {
//insert course CMPE 280 Fall 2016 UIDesign 01
                            printf("\n\n Will create a new course.\n\n");
                            fflush(stdout);
                            createCourse(ssock, inputs);
                        } else {
                            printf("\n\n Wrong table name! \n\n");
                            fflush(stdout);
                        }
                    } else {
                        //printf("Wrong number of commands!\n\n");fflush(stdout);
                        commandNotFound(ssock, "Wrong number of commands");
                    }
                }

            else if (strcmp(inputs[0], "delete") == 0) {
//delete person email bingshi0112@gmail.com
//delete course id 9
                    if (strcmp(inputs[1], "person") == 0 || strcmp(inputs[1], "course") == 0) {
                        char *tableName = inputs[1];
                        RESULT resR = readFrom(tableName, NULL, 0);
                        int compareR = 0;
                        char *messageSent = NULL;
                        int length = 1;
                        messageSent = malloc(length);
                        memset(messageSent, '\0', length);
                        if (strcmp(tableName, "person") == 0) {
                            PERSON *data = resR.result;
                            for (int j = 0; j < resR.size; j++) {
                                if (strcmp(data[j].email, inputs[3]) == 0) {
                                    compareR = 1;
                                }
                            }
                        }
                        if (strcmp(tableName, "course") == 0) {
                            COURSE_DATA *data = resR.result;
                            for (int j = 0; j < resR.size; j++) {
                                if (data[j].id == atoi(inputs[3])) {
                                    compareR = 1;
                                }
                            }
                        }
                        if (compareR == 1) {
                            int filterSize = 1;
                            FILTER filters[filterSize];
                            filters[0].column = inputs[2];
                            filters[0].oper = END;
                            filters[0].relationOperator = EQUAL;
                            filters[0].value = inputs[3];
                            if (strcmp(inputs[2], "id") == 0) {
                                filters[0].type = INTEGER;
                            } else if (strcmp(inputs[2], "email") == 0) {
                                filters[0].type = VARCHAR;
                            } else {
                                printf("\n\n Can only delete with id or email! \n\n");
                                fflush(stdout);
                            }
                            char *res = deleteFrom(tableName, filters, filterSize);
                            messageSent = realloc(messageSent, strlen(res) + 1);
                            strcpy(messageSent, res);
                        } else {
                            //PLEASE SPECIFY APPROPRIATE MESSAGE
                            char* error = sendErrorJson("ERROR compareR==1 failed");
                            messageSent = realloc(messageSent, strlen(error)+1);
                            strcpy(messageSent, error);
                        }
                        sendMessage(ssock, messageSent);
                        printf("\n\n Result sent to client is: %s\n", messageSent);
                        fflush(stdout);
                        free(messageSent);
                        freeResult(tableName, &resR, resR.size);
                    } else {
                        printf("Wrong table name!\n\n");fflush(stdout);
                        commandNotFound(ssock, "Wrong table name");
                    }
                }

            else if (strcmp(inputs[0], "check") == 0) {
                    if (i == 3) {
//check bingshi011@gmail.com 900112
                        int filterSize = 1;
                        FILTER filters[filterSize];
                        char *tableName = "person";
                        filters[0].column = "email";
                        filters[0].type = VARCHAR;
                        filters[0].oper = END;
                        filters[0].relationOperator = EQUAL;
                        filters[0].value = inputs[1];
                        RESULT res = readFrom(tableName, filters, filterSize);
                        char *messageSent = NULL;
                        int length = 1;
                        messageSent = malloc(length);
                        memset(messageSent, '\0', length);
                        if (res.size == 0) {
                            char* error = sendErrorJson("RESULT SIZE IS ZERO");
                            messageSent = realloc(messageSent, strlen(error)+1);
                            strcpy(messageSent, error);
                        } else { 
                            PERSON *data = res.result;                           
                            if (strcmp(data[0].password, inputs[2]) == 0) {
                            char* json = mapResutToJsonString(&res,PER);
                            messageSent = realloc(messageSent,strlen(json)+1); 
                            memset(messageSent, '\0', strlen(json)+1);
                            strcpy(messageSent,json);                                              
                            } else {
                            char* error = sendErrorJson("PASSWORD DID NOT MATCH");
                            messageSent = realloc(messageSent, strlen(error)+1);
                            strcpy(messageSent, error);
                            }
                        }
                        sendMessage(ssock, messageSent);
                        printf("\n\n Result sent to client is:\n%s\n", messageSent);
                        fflush(stdout);
                        free(messageSent);
                        freeResult(tableName, &res, res.size);
                    } else {
                        printf("Wrong number of commands!\n\n");fflush(stdout);
                        commandNotFound(ssock, "Wrong number of commands");
                    }
                }

            else if (strcmp(inputs[0], "get") == 0) {
                    if ((strcmp(inputs[1], "person") == 0 || strcmp(inputs[1], "course") == 0) &&
                        ((i - 2) % 3 == 0 || ((i - 3) % 3 == 0 && strcmp(inputs[i - 1], "current") == 0))) {
                        char *messageSent = NULL;
                        int length = 1;
                        messageSent = malloc(length);
                        memset(messageSent, '\0', length);
                        int filterSize = (i - 2) / 3;
                        RESULT res;
                        char *tableName = inputs[1];

                        if (i == 2) {
//get person
//get course
                            res = joinFrom(tableName, NULL, 0);
                        } else {
//get course email is bingshi011@gmail.com current
//get person courseId is 1
//get course studentId is 1
                            struct SEMESTER cursem;
                            if (strcmp(inputs[i - 1], "current") == 0) {
                                filterSize = filterSize + 2;
                                cursem = getCurrentSemester();
                            }
                            FILTER filters[filterSize];
                            for (int j = 0; j < filterSize; j++) {
                                if ((strcmp(inputs[i - 1], "current") == 0) &&
                                    (j == (filterSize - 1) || j == (filterSize - 2))) {
                                    filters[filterSize - 2].column = "semester";
                                    filters[filterSize - 2].value = cursem.season;
                                    filters[filterSize - 2].type = VARCHAR;
                                    filters[filterSize - 1].column = "year";
                                    filters[filterSize - 1].value = cursem.year;
                                    filters[filterSize - 1].type = VARCHAR;
                                } else {
                                    filters[j].column = inputs[j * 3 + 2];
                                    filters[j].value = inputs[j * 3 + 4];
                                    if (strcmp(inputs[j * 3 + 2], "id") == 0 ||
                                        strcmp(inputs[j * 3 + 2], "gender") == 0 ||
                                        strcmp(inputs[j * 3 + 2], "role") == 0) {
                                        filters[j].type = INTEGER;
                                    } else {
                                        filters[j].type = VARCHAR;
                                    }
                                }
                                if (j == (filterSize - 1)) {
                                    filters[j].oper = END;
                                } else {
                                    filters[j].oper = AND;
                                }
                                //TODO: other operators
//                                if (strcmp(inputs[j * 3 + 3], "is") == 0) {
//                                    filters[j].relationOperator = EQUAL;
//                                }
                                //TODO: other relation operators
                            }
                            res = joinFrom(tableName, filters, filterSize);
                        }

                        if (res.size == 0) {
                            char* error = sendErrorJson("RESULT SIZE IS ZERO");
                            messageSent = realloc(messageSent, strlen(error)+1); 
                            memset(messageSent,'\0',strlen(error)+1);
                            strcpy(messageSent, error);                            
                        } else {
                            if (strcmp(inputs[1], "person") == 0) {
                                PERSON *data = res.result;
                                char role[2];
                                char* json = mapResutToJsonString(&res, PER);
                                messageSent = realloc(messageSent,strlen(json)+1); 
                                memset(messageSent, '\0', strlen(json)+1);
                                strcpy(messageSent,json);              
                            }
                            if (strcmp(inputs[1], "course") == 0) {
                                COURSE_DATA *data = res.result;
                                char id[10];
                                char* json = mapResutToJsonString(&res, COURSE);
                                messageSent = realloc(messageSent,strlen(json)+1); 
                                memset(messageSent, '\0', strlen(json)+1);
                                strcpy(messageSent,json);              
                            }
                        }
                        sendMessage(ssock, messageSent);
                        printf("Result sent to client is:\n%s\n", messageSent);
                        fflush(stdout);
                        free(messageSent);
                        freeResult(tableName, &res, res.size);
                    } else {
                        printf("Wrong table name or wrong number of commands!\n\n");fflush(stdout);
                        commandNotFound(ssock, "Wrong table name or wrong number of commands");
                    }
                }

            else if (strcmp(inputs[0], "enroll") == 0) {
//enroll person email bingshi011@gmail.com course id 1
                    if ((strcmp(inputs[1], "person") == 0 || strcmp(inputs[1], "course") == 0) &&
                        ((i - 1) % 3 == 0)) {
                        char *messageSent = NULL;
                        RESULT res;
                        COURSE_STUDENTS_RL rltnTable;
                        char *tableName = inputs[1];

                        FILTER filters[1];
                        filters[0].column = inputs[2];
                        filters[0].value = inputs[3];
                        if (strcmp(inputs[2], "id") == 0 ||
                            strcmp(inputs[2], "gender") == 0 ||
                            strcmp(inputs[2], "role") == 0) {
                            filters[0].type = INTEGER;
                        } else {
                            filters[0].type = VARCHAR;
                        }
                        filters[0].oper = END;

                        res = readFrom(tableName, filters, 1);

                        if (res.size != 0) {
                            
                            if (strcmp(inputs[1], "person") == 0) {
                                char id[10];
                                for (int i = 0; i < res.size; i++) {
                                    PERSON *data = res.result;
                                    sprintf(id, "%d", data[i].id);

                                    FILTER filterCheck[2];
                                    filterCheck[0].column = "studentId";
                                    filterCheck[0].value = id;
                                    filterCheck[0].type = INTEGER;
                                    filterCheck[0].oper = AND;
                                    filterCheck[1].column = "courseId";
                                    filterCheck[1].value = inputs[6];
                                    filterCheck[1].type = INTEGER;
                                    filterCheck[1].oper = END;

                                    RESULT resCheck = readFrom("course_students_rltn", filterCheck, 2);

                                    if (resCheck.size == 0) {
                                        rltnTable.studentId = data[i].id;
                                        rltnTable.courseId = atoi(inputs[6]);
                                        writeTo("course_students_rltn", &rltnTable);
                                        messageSent = "success";
                                    } else {
                                        messageSent = sendErrorJson("RESULT SIZE IS ZERO In course_students_rltn");                                        
                                    }
                                }
                            } 
                            else {
                             messageSent = sendErrorJson("Wrong Inputs");
                            }
                            
                        } else {
                            messageSent = sendErrorJson("Size of Result is Zero");
                        }
                        sendMessage(ssock, messageSent);
                        printf("Result sent to client is:\n%s\n", messageSent);
                        fflush(stdout);
                        freeResult(tableName, &res, res.size);
                    } else {
                        printf("\n\n Wrong table name or wrong number of commands! \n\n");fflush(stdout);
                        commandNotFound(ssock, "Wrong table name or wrong number of commands");
                    }
                }

            else if (strcmp(inputs[0], "drop") == 0) {
//drop person email bingshi011@gmail.com course id 1
                    if (strcmp(inputs[1], "person") == 0 && strcmp(inputs[4], "course") == 0 &&
                        strcmp(inputs[5], "id") == 0 &&
                        i == 7) {
                        char *messageSent;
                        char id[10];
//                        int length = 1;
                        RESULT res;
                        char *tableName = inputs[1];

                        FILTER filtersPerson[1];
                        filtersPerson[0].column = inputs[2];
                        filtersPerson[0].value = inputs[3];
                        if (strcmp(inputs[2], "id") == 0 ||
                            strcmp(inputs[2], "gender") == 0 ||
                            strcmp(inputs[2], "role") == 0) {
                            filtersPerson[0].type = INTEGER;
                        } else {
                            filtersPerson[0].type = VARCHAR;
                        }
                        filtersPerson[0].oper = END;
                        res = readFrom(tableName, filtersPerson, 1);

                        if (res.size != 0) {
                            PERSON *data = res.result;
                            for (int i = 0; i < res.size; i++) {
                                sprintf(id, "%d", data[i].id);
                                FILTER filters[2];
                                filters[0].column = "studentId";
                                filters[0].value = id;
                                filters[0].type = INTEGER;
                                filters[0].oper = AND;
                                filters[1].column = "courseId";
                                filters[1].value = inputs[6];
                                filters[1].type = INTEGER;
                                filters[1].oper = END;

                                RESULT resCheck = readFrom("course_students_rltn", filters, 2);

                                if (resCheck.size == 1) {
                                    char *result = deleteFrom("course_students_rltn", filters, 2);
                                    messageSent = result;
                                }else{
                                    messageSent = sendErrorJson("ERROR");
                                }
                            }
                        }else{
                            messageSent = sendErrorJson("ERROR");
                        }
                        sendMessage(ssock, messageSent);
                        printf("Result sent to client is:\n%s\n", messageSent);
                        fflush(stdout);
                        freeResult(tableName, &res, res.size);
                    } else {
                        printf("\n\n Wrong table name or wrong number of commands! \n\n");fflush(stdout);
                        commandNotFound(ssock, "Wrong table name or wrong number of commands");
                    }
                }
                else if(strcmp(inputs[0], "upload")==0 ){
//upload file_name course_id
                //1 validate course_id if it exists
                    
                    char *filename = inputs[1];
                    int filesize = 0;
                    FILTER course_filter[1];
                    course_filter[0].column="id"; 
                    course_filter[0].value=inputs[2]; //printf("\n\n id is %d",course_filter[0].value); fflush(stdout);
                    course_filter[0].type=INTEGER;  
                    course_filter[0].relationOperator=EQUAL; 
                    course_filter[0].oper=END;
                    RESULT coursesData = readFrom("course",course_filter,1);
                    if(coursesData.size==1) {
                        //create Folder if absent and get the full path of the FILE to be created
                        char *fullpath = createDir(inputs[2], inputs[1]);
                        printf("\n<-----------------------UPLOADING  %s--------------------->\n", fullpath); fflush(stdout);
                        //send ok
                        sendMessage(ssock,"upload");
                        char *file_size = getMessage(ssock);
                        filesize=atoi(file_size);
                        //printf("file size is %d\n\n",filesize); fflush(stdout);
                        //go and create file 
                        FILE *fp = fopen(fullpath,"w+");                        
                        char buffer[512];
                        while(filesize > 0) {
                            memset(buffer,'\0',512);
                            int n = read(ssock,buffer,512);
                            //printf("\n\nbytes read are :%d",n);fflush(stdout); 
                            if(n<=0) {break;}
                            filesize-=n;
                            //printf("\n\nsize is %d\n\n", filesize);
                            fwrite(buffer,1,n,fp);
                            //printf("\n\n bytes writen to file %d\n\n",fwrite(buffer,1,n,fp));                            
                        }
                        //printf("\n\n --------->Indicator\n\n");fflush(stdout);
                        //go and insert record into filestore
                        FILE_STORE userdata;
                        userdata.filename = filename;
                        userdata.filepath = fullpath;
                        userdata.courseId = atoi(inputs[2]);
                        writeTo("filestore", &userdata);
                        if(fp==NULL) {
                            char *error_json = sendErrorJson("ERROR IN FILE HANDLING"); 
                            sendMessage(ssock,error_json);
                        } else {
                            //char* msg = sendMessageJson("result", "success_upload");
                            char* msg = sendResultObject("test", "success_upload");
                            sendMessage(ssock, msg);
                        }                        
                        //printf("\n\n --------->Indicator\n\n");fflush(stdout);
                        free(fullpath);
                        fclose(fp);
                    }
                    else {
                        //send error json
                        char *error_json = sendErrorJson("course does not exist");
                        //printf("-->%s", error_json);fflush(stdout);
                        sendMessage(ssock,error_json);
                    }                   
                } else if(strcmp(inputs[0], "download")==0 && i==3) {
//download file_name courseId
                //1. check if file exists
                    FILTER fileFilter[2];
                    fileFilter[0].column="filename";
                    fileFilter[0].value=inputs[1];
                    fileFilter[0].type=VARCHAR;
                    fileFilter[0].relationOperator=EQUAL;
                    fileFilter[0].oper=AND;
                    
                    fileFilter[1].column="courseId";
                    fileFilter[1].value=inputs[2];
                    fileFilter[1].type=INTEGER;
                    fileFilter[1].relationOperator=EQUAL;
                    fileFilter[1].oper=END;
                    
                    
                    RESULT res = readFrom("filestore",fileFilter,2);
                
                    if(/*res==NULL ||*/ res.size==0) {
                        char *error_json = sendErrorJson("FILE DOES NOT EXIST");
                        sendMessage(ssock, error_json);
                    }
                    else if(res.size > 1) {  
                        char *error_json = sendErrorJson("Duplicate File Name For Same CourseId!");
                        sendMessage(ssock, error_json);                        
                    }
                    else {
//download file_name                        
                        //1 send message "download"
                        sendMessage(ssock,"download");
                        FILE_STORE *fs = (FILE_STORE *) res.result;
                        int filesize = getFileSize(fs->filepath); //printf("\n\n File size is %d\n\n",filesize); fflush(stdout);
                        char file_size[15]; memset(file_size,'\0',15); sprintf(file_size,"%d",filesize);
                        //2 Send file size
                        //printf("\n\n File size in String %s\n\n",file_size); fflush(stdout);
                        sendMessage(ssock, file_size);
                        
                        //get the file path;
                        //printf("\n\n Opening filepath %s",fs->filepath);fflush(stdout);
                        FILE *fp = fopen(fs->filepath,"r");
                        if(fp==NULL) {
                        printf("\n\n failed to open %s",fs->filepath);fflush(stdout);
                        }
                        char buffer[512];
                        while(!feof(fp)) {
                            memset(buffer,'\0',512);
                            int n = fread(buffer,1,512,fp);
                            if(n==0) {break;}
                            //printf("\n\nbytes read are :%d",n);fflush(stdout); 
                            int n1 = write(ssock,buffer,n);
                            //printf("\n\nbytes sent are :%d",n1);fflush(stdout); 
                        }
                        
                        char* msg = sendResultObject(fs->filename, "success_download");
                        sendMessage(ssock, msg);
                        //printf("\n\n I am done\n\n"); fflush(stdout);
                        //read the file and send it over socket
                    }
                    
                } 
                else {
                     //printf("\n\nI should not be here twice\n\n"); fflush(stdout);
                     commandNotFound(ssock, "Command Not Supported by Server");
                }
            } 
            else {
                //fixing server
                //printf("\n\n I am here after fix \n\n"); fflush(stdout);
                if(buf!=NULL && strcmp(buf,"")!=0) {
                 commandNotFound(ssock, buf);
                }
                else {
                printf("\n\n Client has Terminated the Socket! \n\n"); fflush(stdout);
                break;
                }
            }
            free(buf);
        }// end of while
}

int sendMessage(int s, char *message) {
    printf("\n\nMessage is %s", message); 
    char *sizeSt = malloc(sizeof(char) * HEADER_SIZE);
    bzero(sizeSt, HEADER_SIZE);
    sprintf(sizeSt, "%016d", strlen(message));
    
    ////encrypt sizeSt
    encrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    (void) write(s, sizeSt, HEADER_SIZE);
    printf("\n\nSERVER SENT SIZE %s\n",sizeSt);
    decrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    printf("\n\nDEBUG SERVER SENT SIZE %s\n",sizeSt); fflush(stdout);
    
    int oldlength = strlen(message);
    int newlength =0;
    if(oldlength%16!=0) {
        newlength = oldlength + (16 - (oldlength%16));
    }
    else{
        newlength=oldlength;
    }
    bzero(sizeSt, HEADER_SIZE);
    sprintf(sizeSt, "%016d", newlength);
    encrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    (void) write(s, sizeSt, HEADER_SIZE);
    printf("\n\nSERVER SENT SIZE %s",sizeSt);
    
    char *newMessage = malloc(newlength);
    strncpy(newMessage,message,newlength);
    encrypt(newMessage, newlength, IV, key, keysize);
    printf("\n\nSERVER SENT Message %d\n",strlen(message)); fflush(stdout);
    (void) write(s, newMessage, newlength);
    printf("\n\nEncrpted Message SENT BY SERVER %s\n",newMessage); fflush(stdout);
    free(newMessage);
    free(sizeSt);
}

char *getMessage(int sock) {
    
    char *sizeSt = malloc(sizeof(char) * HEADER_SIZE);
    bzero(sizeSt, HEADER_SIZE);
    //sizeSt[HEADER_SIZE] = '\0';
    int n = 0;
    int count = 0;

    for (count = 0; count < HEADER_SIZE; count += n) {
        n = read(sock, &sizeSt[count], HEADER_SIZE - count);
        if (n < 0) {
            printf("error = %s \n", strerror(errno));
            fflush(stdout);
            break;
        }
        else if(n==0) {
         printf("\n\n--------May be Client has terminated Socket------------\n\n");
         return "";
        }
    }
    //decrypt sizeSt
    printf("\n\nServer Received SIZE %s\n\n",sizeSt); fflush(stdout);    
    decrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    printf("\n\nServer Received SIZE After decrypt %s",sizeSt); fflush(stdout); 
    int messageLen = atoi(sizeSt); printf("\n\nMessage len is %d\n\n",messageLen); fflush(stdout);
    
    //2nd read for updated size
    bzero(sizeSt, HEADER_SIZE);
    /*sizeSt[HEADER_SIZE] = '\0';*/ n=0; count=0;
    for (count = 0; count < HEADER_SIZE; count += n) {
        n = read(sock, &sizeSt[count], HEADER_SIZE - count);
        if (n < 0) {
            printf("error = %s \n", strerror(errno));
            fflush(stdout);
            break;
        }
        else if(n==0) {
         printf("\n\n--------May be Client has terminated Socket------------\n\n");
         return "";
        }
    }
    
    printf("\n\nServer Received SIZE %s",sizeSt); fflush(stdout);    
    decrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    printf("\n\nServer Received SIZE After decrypt %s\n\n",sizeSt); fflush(stdout); 
    int newlength = atoi(sizeSt);

    char *message = malloc(sizeof(char) * newlength);
    bzero(message, newlength);
    message[newlength] = '\0';

    n = 0;
    for (count = 0; count < newlength; count += n) {
        if ((n = read(sock, &message[count], newlength - count)) <= 0) {
            break;
        }
    }
    //decrypt message
    printf("\n\nServer Received Message %s\n\n",message); fflush(stdout);
    int ret =decrypt(message, newlength, IV, key, keysize);
    printf("\n\nServer Received Message After Decryption %s\n\n",message); fflush(stdout);
    //printf("\n\n------------>RESULT %d\n\n",ret); fflush(stdout);*/
    free(sizeSt);
    //char * message="test";
    return message;
}



struct SEMESTER getCurrentSemester() {
    struct tm *current;
    time_t timenow;
    time(&timenow);
    current = localtime(&timenow);
    struct SEMESTER currentSemester;
    int *month = current->tm_mon + 1;
    if (month < 6) {
        currentSemester.season = "Spring";
    } else if (month > 8) {
        currentSemester.season = "Fall";
    } else {
        currentSemester.season = "Summer";
    }
//    itoa((current->tm_year+1900), currentSemester.year, 10);
//    sprintf(currentSemester.year, "%d", (current->tm_year+1900));
    currentSemester.year = "2017";
    return currentSemester;
}

/*--------------------------------------------------API--------------------------------------------------*/

void createPerson(int s, char *inputs[]) {
    char *messageSent;
    FILTER filters[1];
    filters[0].column = "email";
    filters[0].value = inputs[4];
    filters[0].type = VARCHAR;
    filters[0].oper = END;
    RESULT res = readFrom("person", filters, 1);
    if (res.size == 0) {
        PERSON p;
        p.first_name = inputs[2];
        p.last_name = inputs[3];
        p.email = inputs[4];
        p.password = inputs[5];
        p.gender = atoi(inputs[6]);
        p.role = atoi(inputs[7]);
        writeTo("person", &p);
        messageSent = "success";
    } else {
        messageSent = "error";
    }
    sendMessage(s, messageSent);
    printf("Message sent to client is:\n%s\n", messageSent);
    fflush(stdout);
}

void createCourse(int s, char *inputs[]) {
    char *messageSent;
    FILTER filters[5];
    filters[0].column = "department";
    filters[0].value = inputs[2];
    filters[0].type = VARCHAR;
    filters[0].oper = AND;
    filters[1].column = "number";
    filters[1].value = inputs[3];
    filters[1].type = VARCHAR;
    filters[1].oper = AND;
    filters[2].column = "semester";
    filters[2].value = inputs[4];
    filters[2].type = VARCHAR;
    filters[2].oper = AND;
    filters[3].column = "year";
    filters[3].value = inputs[5];
    filters[3].type = VARCHAR;
    filters[3].oper = AND;
    filters[4].column = "section";
    filters[4].value = inputs[7];
    filters[4].type = VARCHAR;
    filters[4].oper = END;
    RESULT res = readFrom("course", filters, 5);
    if (res.size == 0) {
        COURSE_DATA c;
        c.department = inputs[2];
        c.number = inputs[3];
        c.semester = inputs[4];
        c.year = inputs[5];
        c.name = inputs[6];
        c.section = inputs[7];
        writeTo("course", &c);
        messageSent = "success";
    } else {
        messageSent = "error";
    }
    sendMessage(s, messageSent);
    printf("Message sent to client is:\n%s\n", messageSent);
    fflush(stdout);
}

int commandNotFound(int s, char *msg) {
    printf("command \"%s\" is not valid!\n", msg); fflush(stdout);
    char *message = sendErrorJson(msg);
    sendMessage(s, message);
}

/*--------------------------------------------------HELPER AND CONNECTION--------------------------------------------------*/
int errexit(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}

int passivesock(const char *service, int qlen) {
    struct servent *pse; /* pointer to service information entry */
    struct protoent *ppe; /* pointer to protocol information entry*/
    struct sockaddr_in sin; /* an Internet endpoint address */
    int s, type; /* socket descriptor and socket type */

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

    if (pse = getservbyname(service, "tcp"))
        sin.sin_port = htons(ntohs((u_short) pse->s_port) + portbase);
    else if ((sin.sin_port = htons((u_short) atoi(service))) == 0)
        errexit("can't get \"%s\" service entry\n", service);

    if ((ppe = getprotobyname("tcp")) == 0)
        errexit("can't get \"%s\" protocol entry\n", "tcp");

    type = SOCK_STREAM;

    /* Allocate a socket */
    s = socket(PF_INET, type, ppe->p_proto);
    if (s < 0)
        errexit("can't create socket: %s\n", strerror(errno));

    int enable = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        errexit("setsockopt(SO_REUSEADDR) failed");

    if (bind(s, (struct sockaddr *) &sin, sizeof(sin)) < 0)
        errexit("can't bind to %s port: %s\n", service,
                strerror(errno));

    if (listen(s, qlen) < 0)
        errexit("can't listen on %s port: %s\n", service,
                strerror(errno));

    return s;
}

char* createDir(char *courseId, char *fileName) {
struct stat st = {0};
int len = strlen(SERVER_FILE_PATH)+strlen(courseId)+1;
char *fullpath = malloc(len); memset(fullpath,'\0', len);
strcpy(fullpath, SERVER_FILE_PATH); strcat(fullpath, courseId);
if (stat(fullpath, &st) == -1) {
    mkdir(fullpath, 0700);
}
//DIR is created now send full file path
len+=strlen(fileName)+1; //+1 for appending '/'
fullpath = realloc(fullpath,len); strcat(fullpath,"/");strcat(fullpath,fileName); fullpath[len]='\0';
//printf("\n\nfull file path is ---------> %s\n\n",fullpath);
return fullpath;
}

int getFileSize(char* name) {
    int size=0;
	if(name != NULL) {
	char* full_path = name;
	FILE *fp = fopen(full_path,"r");
    if(fp==NULL) {printf("\ncan not open file"); fflush(stdout);}
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	rewind(fp);
	fclose(fp);
	//free(full_path);
	}
return size;
}
