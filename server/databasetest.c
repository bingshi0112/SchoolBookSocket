#include <my_global.h>
#include <stdio.h>
#include <mysql.h>

void print_error(MYSQL *myConn) {
    fprintf(stderr,"%s\n",mysql_error(myConn));
    mysql_close(myConn);
}


int main(int argc, char **argv)
{
	printf("hello world\n");
    printf("MySQL client version: %s\n", mysql_get_client_info());
    MYSQL *conn = mysql_init(NULL);
    
    if(conn==NULL) {
    print_error(conn);
    exit(1);
    }
    //localhost was causing issue instead give IP
    if(mysql_real_connect(conn,"127.0.0.1","root","900112",NULL,0,NULL,0)==NULL) {
    print_error(conn);
    exit(1);
    }
    
    if(mysql_query(conn,"CREATE DATABASE test")) {
    print_error(conn);
    exit(1);
    }
    
    mysql_close(conn);
	return 0;
}
