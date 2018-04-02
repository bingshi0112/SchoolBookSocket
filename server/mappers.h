#ifndef MAPPERS_H_
#define MAPPERS_H_

#include <json.h>
#include "schema.h"


char* mapResutToJsonString(RESULT *res, TABLE_NAME name);

char* sendErrorJson(char* message);

char* sendMessageJson(char* key, char* message);

char* sendResultObject(char *fileName, char *status);

//RESULT mapJsonStringToResult(char *jsonString, TABLE_NAME name);

#endif