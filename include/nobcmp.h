#include <cstdio>

typedef const char * Cstr;

typedef struct {
    Cstr *elems;
    size_t count;
} Cstr_Array;

typedef struct {
    Cstr_Array line;
} Cmd;

typedef struct {
    const char **items;
    size_t count;
    size_t capacity;
} Nob_Cmd;
