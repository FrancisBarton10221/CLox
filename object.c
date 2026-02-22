#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType, stringLength) \
    (type*)allocateObject(sizeof(type)+stringLength, objectType)

static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    
    object->next = vm.objects;
    vm.objects = object;
    return object;
}

static uint32_t hashString(const char* key, int length) {
    uint32_t hash = 216613261u;
    for (int i = 0; i < length; i++) {
	hash ^= (uint8_t)key[i];
	hash *= 16777619;
    }
    return hash;
}

ObjString* allocateString(const char* chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) return interned;
    printf("Debug");
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING, length);
    string->length = length;
    memcpy(&string->chars, chars, length);
    string->chars[length] = '\0';
    string->hash = hash;
    tableSet(&vm.strings, string, NIL_VAL);
    return string;
}	

ObjString* takeString(char* chars, int length) {
    return allocateString(chars, length);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
	case OBJ_STRING:
	    printf("%s", AS_CSTRING(value));
    }
}
