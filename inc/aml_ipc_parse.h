#include "aml_ipc_define.h"
#ifndef AML_IPC_PARSE_H
#define AML_IPC_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum JSONValueType {
    JSON_TYPE_UNKNOWN,
    JSON_TYPE_NULL,
    JSON_TYPE_BOOLEAN,
    JSON_TYPE_DOUBLE,
    JSON_TYPE_INT,
    JSON_TYPE_OBJECT,
    JSON_TYPE_ARRAY,
    JSON_TYPE_STRING,
};

struct JSONValueSimple {
    const char* start;
    const char* end;
};

struct JSONValue {
    struct JSONValue* next;
    struct JSONValue* parent;
    enum JSONValueType val_type;
    union {
        struct JSONValueSimple val_text;
        struct {
            struct JSONValue* val_list;
            struct JSONValue* val_last;
        };
    };
    union {
        int num_child;
        int val_state;
    };
};

struct AmlIPCPipeline* aml_ipc_parse_launch(const char* str);
AmlStatus aml_ipc_parse_json(const char* str, struct JSONValue** outval);
int aml_ipc_get_json_value_string(struct JSONValue* val, char* buf, int len);
void aml_ipc_free_json_value(struct JSONValue* val);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_PARSE_H
