#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 62219
#define HASH_MUL 1801

struct listnode {
    char *key;
    int value;
    struct listnode *next;
};

long long hash(char*);
void hashtab_init(struct listnode**);
void push_hash(struct listnode**, char*, int);
void add_hashtab(struct listnode**, int);
void init_asm(struct listnode**);
void end_asm();
struct listnode *lookup_hash(struct listnode**, char*);