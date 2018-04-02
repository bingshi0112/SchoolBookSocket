//
// Created by Bing Shi on 4/22/17.
//

#include <json.h>
#include <stdio.h>

int main() {
    char * string = "{'name' : 'joys of programming'}";
    json_object * jobj = json_tokener_parse(string);
    enum json_type type = json_object_get_type(jobj);
    //printf("type: ",type);fflush(stdout);
    switch (type) {
        case json_type_null: printf("json_type_nulln");
            break;
        case json_type_boolean: printf("json_type_booleann");
            break;
        case json_type_double: printf("json_type_doublen");
            break;
        case json_type_int: printf("json_type_intn");
            break;
        case json_type_object: printf("json_type_objectn"); fflush(stdout);
            break;
        case json_type_array: printf("json_type_arrayn");
            break;
        case json_type_string: printf("json_type_stringn");
            break;
        default:
            printf("\n type did not match");
    }
}