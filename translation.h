//
// Created by user on 27.12.17.
//

#ifndef TLMT_TRANSLATION_H
#define TLMT_TRANSLATION_H
#include <stdio.h>
#include <string.h>
#include "hash.h"

typedef enum { typeCon, typeId, typeOpr, typeDef, typeErr } nodeEnum;

struct treenode {
    char *key;
    nodeEnum type;
    struct treenode *p1;
    struct treenode *p2;
    struct treenode *p3;
    struct treenode *p4;
    struct treenode *p5;
};

void dfs(struct treenode*);
void excep(struct treenode*, int);
struct treenode *add_atr(struct treenode*, struct treenode*);
struct treenode *create_node(char*, nodeEnum, struct treenode*, struct treenode*, struct treenode*, struct treenode*, struct treenode*);
#endif //TLMT_TRANSLATION_H
