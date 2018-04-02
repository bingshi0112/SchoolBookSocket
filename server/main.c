#include <stdio.h>
#include <string.h>

int main() {
    printf("Hello, World!\n");
    char buffer[256];
    memset(buffer,0,256);
    printf("\nBefore %d\n\n",sizeof(buffer));//256
    buffer[0]='a'; buffer[1]='b'; buffer[2]='c'; 
    printf("\nBefore %d\n\n",sizeof(buffer));//256
    char *ptr = "Hello hi";
    printf("\nBefore %d\n\n",sizeof(ptr));//8
    printf("\nBefore %d\n\n",sizeof(*ptr));//1
    return 0;
}