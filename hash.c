#include "hash.h"

extern FILE *fd;

long long hash(char *key) {
    long long h = 0;
    char *p;
    for (p = key; *p != '\0'; p++)
        h = (h * HASH_MUL + (long long)*p) % HASH_SIZE;
    return h;
}

void hashtab_init(struct listnode **hashtab) {
    int i;
    for (i = 0; i < HASH_SIZE; i++)
        hashtab[i] = NULL;
}

void push_hash(struct listnode **hashtab, char *key, int value) {
    struct listnode *node;
    struct listnode *search;
    int index = hash (key);
    search = lookup_hash(hashtab, key);
    if (search != NULL)
        return;
    node = malloc(sizeof(struct listnode));
    if (node == NULL) {
        return;
    }
    node->key = strdup(key);
    node->value = value;
    node->next = hashtab[index];
    hashtab[index] = node;
}

void add_hashtab(struct listnode **hashtab, int value) {
    int i;
    struct listnode *node;
    for (i = 0; i < HASH_SIZE; i++) {
        for (node = hashtab[i]; node != NULL; node = node->next) {
            if (node->value == 3)
                node->value = value;
        }
    }
}

void init_asm(struct listnode **hashtab) {
    int i;
    fprintf(fd, "extern printf\n");
    fprintf(fd, "extern scanf\n");
    fprintf(fd, "global main\n\n");
    fprintf(fd, "SECTION .data\n");
    fprintf(fd, "\tfmtir:    db \"%%d\", 0\n");
    fprintf(fd, "\tfmtlr:    db \"%%ld\", 0\n");
    fprintf(fd, "\tfmtiw:    db \"%%d\", 10, 0\n");
    fprintf(fd, "\tfmtlw:    db \"%%ld\", 10, 0\n");
    struct listnode *node;
    for (i = 0; i < HASH_SIZE; i++) {
        for (node = hashtab[i]; node != NULL; node = node->next) {
            if (node->value == 1)
                fprintf(fd, "\t%s dd 0\n", node->key);
            else
                fprintf(fd, "\t%s dq 0\n", node->key);
        }
    }
    fprintf(fd, "SECTION .text\n");
    fprintf(fd, "main:\n");
    fprintf(fd, "\tpush ebp\n");
    fprintf(fd, "\tmov ebp, esp\n");
    fprintf(fd, "\tsub esp, 64\n");
}

void end_asm() {
    fprintf(fd, "\tmov esp, ebp\n");
    fprintf(fd, "\tpop ebp\n");
    fprintf(fd, "\tret\n");
}

struct listnode *lookup_hash(struct listnode **hashtab, char *key) {
    struct listnode *node;
    int index = hash(key);
    for (node = hashtab[index]; node != NULL; node = node->next)
        if (strcmp(key, node->key) == 0)
            return node;
    return NULL;
}

void free_hash(struct listnode **hashtab) {
    struct listnode *node, *swap;
    for (int i = 0; i < HASH_SIZE; i++) {
        for (node = hashtab[i]; node != NULL; node = swap) {
            swap = node->next;
            free(node);
        }
    }
}