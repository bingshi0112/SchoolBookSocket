#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include "Client.h"
#include <sys/stat.h>


//Security
#include "encryption.h"

#define LINELEN 4096
#define HEADER_SIZE 16
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif /* INADDR_NONE */

/*
 * Security Credentials
 * */
char* IV = "AAAAAAAAAAAAAAAA";
char *key = "0123456789abcdef";
int keysize = 16; /* 128 bits */

int requestFile(const char *host, const char *service, const char *filename);

int errexit(const char *format, ...);

int connectsock(const char *host, const char *service);

char *getMessage(int s);

int getFileSize(char* name);

char* getFileNameFromInput(char *input);

char* getFullFilePath(char *name);

char* inttoa(int a);

const char *CLIENT_FILE_PATH = "/Users/bing/Desktop/courses/CMPE207_NetworkProgrammingAndApplication/project/207projectClient/filestores/";

static int s;
static char input[100];


void connectHost(const char *host, const char *service, const void (*onMessageReceive)(char const *)) {
    char buf[LINELEN + 1]; /* buffer for one line of text */
    int n;
    s = connectsock(host, service);

    onMessageReceive("Connected sucessfully!\n");
    memset(input, '\0', sizeof(input));

    while (1) {
        while(input[0] == '\0') {
        }

        char st[18 + strlen(input)];
        sprintf(st, "Input received: %s\n", input);
//        printf("------------------%s \n\n", st);
//            fflush(stdout);
        onMessageReceive(input);

        printf("------------------\n\n");
        fflush(stdout);

        sendMessage(s, input);
        char *message = getMessage(s);
        
        if(strcmp("upload",message)==0) {         
         char *file_name = getFileNameFromInput(input); printf("\nfile name is %s\n", file_name); fflush(stdout);
         int file_size = getFileSize(file_name);
         char* file_size_to_char = inttoa(file_size);
         //send file size first
         sendMessage(s, file_size_to_char);
         char* fullpath = getFullFilePath(file_name); printf("\nfile name is %s\n", file_name); fflush(stdout);
         FILE *fp  = fopen(fullpath,"r");
         char buffer[512];
                           while(!feof(fp)) {
                                memset(buffer,'\0',512);
                                int n = fread(buffer,1,512,fp);
                                if(n==0) {break;}
                                printf("\n\nbytes read are :%d",n);fflush(stdout); 
                                int n1 = write(s,buffer,n);
                                printf("\n\nbytes sent are :%d",n1);fflush(stdout); 
                            }
         //printf("\n\n I am done\n\n"); fflush(stdout);
         //printf("\n\n --------->Indicator\n\n");fflush(stdout);
         
         //send message once recoreds are updated {"result":"file has been uploaded"} OR {"error" : "ERROR IN FILE HANDLING"}
         char *response = getMessage(s);
         onMessageReceive(response);
         printf("\n\nRESPONSE FROM SERVER : %s\n\n",response); fflush(stdout);
         fclose(fp);
        }
        
        //1.getMessage which is download
        if(strcmp("download",message)==0) {
            char *file_name = getFileNameFromInput(input); printf("\nfile name is %s\n", file_name); fflush(stdout);
            //2. get file size from server            
             char *filesize = getMessage(s); int file_size = atoi(filesize); free(filesize);
             printf("\n\nFILE SIZE IS %d\n\n",file_size); fflush(stdout);
            //3. Now till file size not equals 0 
            char buffer[512];
            int max = 0;
            FILE *fp = fopen(getFullFilePath(file_name),"w+");
                                while(file_size > 0 ) {
                                 memset(buffer,'\0',512);
                                 if(file_size > 512) {
                                    max = 512;
                                 } else {
                                    max = file_size;
                                 }

                                 int n = read(s,buffer,max);
                                 printf("\n\nbytes read are :%d",n);fflush(stdout); 
                                 if(n<=0) {break;}
                                 file_size-=n;
                                 printf("\n\nsize is %d\n\n", file_size);
                                 printf("\n\nfile path  %s\n\n", getFullFilePath(file_name));
                                 printf("\n\n bytes writen to file %d\n\n",fwrite(buffer,1,n,fp));
                                }

                                char *res = getMessage(s);
            fclose(fp);
                                onMessageReceive(res);

        }

        memset(input, '\0', sizeof(input));
        char messageReceived[2 + strlen(message)];
        sprintf(messageReceived, "\n%s\n", message);
        onMessageReceive(messageReceived);

        free(message);
    }
}

void setInput(const char *newInput) {
    strcat(input, newInput);
}

/*int sendMessage(int s, char *message) {
    char *sizeSt = malloc(sizeof(char) * HEADER_SIZE + 1);
    bzero(sizeSt, sizeof(sizeSt));
    sizeSt[HEADER_SIZE] = '\0';

    sprintf(sizeSt, "%08d", strlen(message));

    (void) write(s, sizeSt, sizeof(sizeSt));
    (void) write(s, message, strlen(message));

    free(sizeSt);
}*/

int sendMessage(int s, char *message) {
    char *sizeSt = malloc(sizeof(char) * HEADER_SIZE);
    bzero(sizeSt, HEADER_SIZE);
    //sizeSt[HEADER_SIZE] = '\0';

    sprintf(sizeSt, "%016d", strlen(message));
    //myencrypt sizeSt
    myencrypt(sizeSt, 16, IV, key, keysize); 
    (void) write(s, sizeSt, HEADER_SIZE);
    //printf("\n\nCLIENT SENT SIZE %s",sizeSt); fflush(stdout);
    //mydecrypt(sizeSt, 16, IV, key, keysize); 
    printf("\n\nCLIENT SENT SIZE %s\n",sizeSt); fflush(stdout);
    
    int oldlength = strlen(message);
    int newlength =0;
    if(oldlength%16!=0) {
        newlength = oldlength + (16 - (oldlength%16));
    }
    else {
        newlength=oldlength;
    }
    
    bzero(sizeSt, HEADER_SIZE);
    sprintf(sizeSt, "%016d", newlength);
    printf("New length is    %s",sizeSt); fflush(stdout);
    myencrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    (void) write(s, sizeSt, HEADER_SIZE);
    printf("\n\nClIENT SENT SIZE %s",sizeSt);
    
    mydecrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    printf("\n\nDEBUG : %s\n",sizeSt);fflush(stdout);
    
    //myencrypt message
    char *newMessage  = malloc(sizeof(char) * newlength);
    strncpy(newMessage,message,newlength);
    myencrypt(newMessage, newlength, IV, key, keysize);     
    (void) write(s, newMessage, newlength);
    
    mydecrypt(newMessage, newlength, IV, key, keysize);     
    printf("\n\nDEGUB CLIENT SENT Message %s",newMessage);fflush(stdout);
    
    free(newMessage);
    free(sizeSt);
}

/*char *getMessage(int sock) {
    char *sizeSt = malloc(sizeof(char) * HEADER_SIZE + 1);
    bzero(sizeSt, sizeof(sizeSt));
    sizeSt[HEADER_SIZE] = '\0';
    int n = 0;
    int count = 0;

    for (count = 0; count < HEADER_SIZE; count += n) {
        if ((n = read(sock, &sizeSt[count], HEADER_SIZE - count)) <= 0) {
            break;
        }
    }

    int messageLen = atoi(sizeSt);
    char *message = malloc(sizeof(char) * messageLen + 1);
    bzero(message, messageLen);
    message[messageLen] = '\0';

    n = 0;
    for (count = 0; count < messageLen; count += n) {
        if ((n = read(sock, &message[count], messageLen - count)) <= 0) {
            break;
        }
    }

    return message;
}*/

char *getMessage(int sock) {
    char *sizeSt = malloc(sizeof(char) * HEADER_SIZE);
    bzero(sizeSt, HEADER_SIZE);
    //sizeSt[HEADER_SIZE] = '\0';
    int n = 0;
    int count = 0;

    for (count = 0; count < HEADER_SIZE; count += n) {
        if ((n = read(sock, &sizeSt[count], HEADER_SIZE - count)) <= 0) {
            break;
        }
    }
    printf("\n\nClient Received SIZE %s",sizeSt); fflush(stdout);
    //mydecrypt sizeSt
    mydecrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    printf("\n\nClient Received SIZE after mydecryption %s\n\n",sizeSt); fflush(stdout);
    int messageLen = atoi(sizeSt);
    
    bzero(sizeSt, HEADER_SIZE);
    /*sizeSt[HEADER_SIZE] = '\0';*/ n=0;count=0;
    for (count = 0; count < HEADER_SIZE; count += n) {
        if ((n = read(sock, &sizeSt[count], HEADER_SIZE - count)) <= 0) {
            break;
        }
    }
    printf("\n\nClient Received SIZE %s",sizeSt); fflush(stdout);
    //mydecrypt sizeSt
    mydecrypt(sizeSt, HEADER_SIZE, IV, key, keysize);
    printf("\n\nClient Received SIZE after mydecryption %s\n",sizeSt); fflush(stdout);
    int newLength = atoi(sizeSt);
    printf("%d",newLength);fflush(stdout);
    
    char *message = malloc(sizeof(char) * newLength);
    bzero(message, newLength);
    message[newLength] = '\0';

    n = 0;
    for (count = 0; count < newLength; count += n) {
        if ((n = read(sock, &message[count], newLength - count)) <= 0) {
            break;
        }
    }
    printf("\n\nClient Received Message %s\n",message); fflush(stdout);
    mydecrypt(message, newLength, IV, key, keysize);
    printf("\n\nClient Received Message After mydecryption %s",message); fflush(stdout);
    
    char *newMessage = malloc(sizeof(char) * messageLen+1); memset(newMessage,'\0',messageLen+1);
    strncpy(newMessage,message,messageLen);
    printf("Final message received from API is %s", newMessage); fflush(stdout);
    free(message);
    return newMessage;
}

int connectsock(const char *host, const char *service) {
    struct hostent *phe; /* pointer to host information entry */
    struct servent *pse; /* pointer to service information entry */
    struct protoent *ppe; /* pointer to protocol information entry*/
    struct sockaddr_in sin; /* an Internet endpoint address */
    int s, type; /* socket descriptor and socket type */
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    if (pse = getservbyname(service, "tcp"))
        sin.sin_port = pse->s_port;
    else if ((sin.sin_port = htons((u_short) atoi(service))) == 0)
        errexit("can't get \"%s\" service entry\n", service);

    if (phe = gethostbyname(host))
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    else if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
        errexit("can't get \"%s\" host entry\n", host);
    if ((ppe = getprotobyname("tcp")) == 0)
        errexit("can't get \"%s\" protocol entry\n", "tcp");

    type = SOCK_STREAM;
    s = socket(PF_INET, type, ppe->p_proto);

    if (s < 0)
        errexit("can't create socket: %s\n", strerror(errno));
    if (connect(s, (struct sockaddr *) &sin, sizeof(sin)) < 0)
        errexit("can't connect to %s.%s: %s\n", host, service,
                strerror(errno));
    return s;
}

int getFileSize(char* name) {
    int size=0;
	if(name != NULL) {
	char* full_path = getFullFilePath(name);
	FILE *fp = fopen(full_path,"r");
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	rewind(fp);
	fclose(fp);
	free(full_path);
	}
return size;
}

char* getFileNameFromInput(char *input) {
    //upload file_name course_id
    char *token = strtok(input, " ");
    char *inputs[20];
    int i = 0;
    while (token != NULL) {
        inputs[i++] = token;
        token = strtok(NULL, " ");
    }
    return inputs[1];
}

char* getFullFilePath(char *name) {
    char *full_path = malloc(strlen(CLIENT_FILE_PATH)+1);
    memset(full_path,'\0',strlen(CLIENT_FILE_PATH)+1);
    strcpy(full_path,CLIENT_FILE_PATH);
    full_path= realloc(full_path,strlen(full_path)+strlen(name));
    strcat(full_path,name); printf("%s",full_path);
    return full_path;
}

char* inttoa(int a) {
    char *result = malloc(9);
    bzero(result, sizeof(result));
    sprintf(result, "%08d", a);
    return result;
}

int errexit(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}
