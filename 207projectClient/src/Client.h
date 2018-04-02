#ifndef DBUTIL_H_
#define DBUTIL_H_

void connectHost(const char *host, const char *service, const void (*onMessageReceive)(char const *));
int sendMessage(int s, char *message);
void setInput(const char *newInput);
#endif