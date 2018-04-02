#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

int myencrypt(
    void* buffer,
    int buffer_len, /* Because the plaintext could include null bytes*/
    char* IV, 
    char* key,
    int key_len 
);

int mydecrypt(
    void* buffer,
    int buffer_len,
    char* IV, 
    char* key,
    int key_len 
);

void display(char* ciphertext, int len);

#endif
