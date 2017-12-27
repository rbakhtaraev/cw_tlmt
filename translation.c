//
// Created by user on 27.12.17.
//

#include "translation.h"

char *op;
int ex;
extern FILE *fd;
int flag = 0;
extern struct listnode *hashtab[HASH_SIZE];
int stc = 4;

void free_tree(struct treenode *p) {
    if (!p)
        return;
    free_tree(p->p1);
    free_tree(p->p2);
    free_tree(p->p3);
    free_tree(p->p4);
    free_tree(p->p5);
    free(p);
}

void dfs(struct treenode *p) {
    int ex1, ex2;
    char assign[] = "==";
    char temp[] = "as";
    struct treenode *node;
    struct listnode *tmp;
    if (p == NULL)
        return;
    switch (p->type) {
        case typeDef:
            dfs(p->p1);
            dfs(p->p5);
            break;
        case typeCon:
            switch (op[0]) {
                case '=':
                    fprintf(fd, "\tmov eax, %s\n", p->key);
                    break;
                case 'a':
                    fprintf(fd, "\tmov eax, %s\n", p->key);
                    break;
                case '!':
                    fprintf(fd, "\tmov eax, %s\n", p->key);
                    break;
                case '>':
                    switch (op[1]) {
                        case '=':
                            fprintf(fd, "\tmov eax, %s\n", p->key);
                            break;
                        default:
                            fprintf(fd, "\tmov eax, %s\n", p->key);
                            break;
                    }
                    break;
                case '<':
                    switch (op[1]) {
                        case '=':
                            fprintf(fd, "\tmov eax, %s\n", p->key);
                            break;
                        default:
                            fprintf(fd, "\tmov eax, %s\n", p->key);
                            break;
                    }
                    break;
                case 'p':
                    fprintf(fd, "\tmov DWORD [ebp - 4], %s\n", p->key);
                    fprintf(fd, "\tpush DWORD [ebp - 4]\n");
                    break;
                default:
                    fprintf(fd, "%d\n", atoi(p->key));
            }
            break;
        case typeId:
            switch (op[0]) {
                case '=':
                    fprintf(fd, "\tmov [%s], eax\n", p->key);
                    break;
                case 'p':
                    fprintf(fd, "\tpush DWORD [%s]\n", p->key);
                    break;
                case 's':
                    fprintf(fd, "\tpush %s\n", p->key);
                    break;
                default:
                    fprintf(fd, "\tmov eax, [%s]\n", p->key);
                    break;
            }
            break;
        case typeOpr:
            if (strcmp(assign, p->key) == 0)
                op = strdup(temp);
            else
                op = strdup(p->key);
            switch (op[0]) {
                case 'w':
                    fprintf(fd, "L%03d:\n", ex1 = ex++);
                    dfs(p->p2);
                    ex2 = ex;
                    dfs(p->p4);
                    fprintf(fd, "\tjmp L%03d\n", ex1);
                    fprintf(fd, "L%03d:\n", ex2);
                    dfs(p->p5);
                    break;
                case 'f':
                    dfs(p->p1);
                    fprintf(fd, "L%03d:\n", ex1 = ex++);
                    dfs(p->p2);
                    ex2 = ex++;
                    dfs(p->p4);
                    dfs(p->p3);
                    fprintf(fd, "\tjmp L%03d\n", ex1);
                    fprintf(fd, "L%03d:\n", ex2);
                    dfs(p->p5);
                    break;
                case 'i':
                    ex++;
                    dfs(p->p2);
                    ex2 = ex++;
                    dfs(p->p4);
                    excep(p->p5, ex2);
                    break;
                case 'p':
                    dfs(p->p1);
                    node = p->p1;
                    if (node->type == typeId) {
                        tmp = lookup_hash(hashtab, node->key);
                        if (tmp->value == 1)
                            fprintf(fd, "\tpush fmtlw\n");
                        else
                            fprintf(fd, "\tpush fmtiw\n");
                    } else
                        fprintf(fd, "\tpush fmtiw\n");
                    fprintf(fd, "\tcall printf\n");
                    dfs(p->p5);
                    break;
                case 's':
                    dfs(p->p1);
                    node = p->p1;
                    tmp = lookup_hash(hashtab, node->key);
                    if (tmp->value == 1)
                        fprintf(fd, "\tpush fmtlr\n");
                    else
                        fprintf(fd, "\tpush fmtir\n");
                    fprintf(fd, "\tcall scanf\n");
                case '=':
                    dfs(p->p2);
                    if (flag == 1) {
                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                        op = strdup(p->key);
                    }
                    dfs(p->p1);
                    dfs(p->p5);
                    flag = 0;
                    break;
                default:
                    switch (op[0]) {
                        case '+':
                            flag = 1;
                            node = p->p1;
                            if ((node->type != typeOpr) && (node->type != typeId))
                                fprintf(fd, "\tmov DWORD [ebp - %d], ", stc);
                            dfs(p->p1);
                            if (node->type == typeId)
                                fprintf(fd, "\tmov [ebp - %d], eax\n", stc);
                            node = p->p2;
                            if (node->type != typeOpr) {
                                if (node->type != typeId)
                                    fprintf(fd, "\tmov DWORD [ebp - %d], ", stc + 4);
                            } else
                                stc = stc + 4;
                            dfs(p->p2);
                            if (node->type == typeOpr)
                                stc = stc - 4;
                            if (node->type == typeId)
                                fprintf(fd, "\tmov DWORD [ebp - %d], eax\n", stc + 4);
                            fprintf(fd, "\tmov eax, [ebp - %d]\n", stc);
                            fprintf(fd, "\tmov ecx, [ebp - %d]\n", stc + 4);
                            fprintf(fd, "\tadd eax, ecx\n");
                            fprintf(fd, "\tmov [ebp - %d], eax\n", stc);
                            break;
                        case '-':
                            flag = 1;
                            node = p->p1;
                            if ((node->type != typeOpr) && (node->type != typeId))
                                fprintf(fd, "\tmov DWORD [ebp - %d], ", stc);
                            dfs(p->p1);
                            if (node->type == typeId)
                                fprintf(fd, "\tmov [ebp - %d], eax\n", stc);
                            node = p->p2;
                            if (node->type != typeOpr) {
                                if (node->type != typeId)
                                    fprintf(fd, "\tmov DWORD [ebp - %d], ", stc + 4);
                            } else
                                stc = stc + 4;
                            dfs(p->p2);
                            if (node->type == typeOpr)
                                stc = stc - 4;
                            if (node->type == typeId)
                                fprintf(fd, "\tmov DWORD [ebp - %d], eax\n", stc + 4);
                            fprintf(fd, "\tmov eax, [ebp - %d]\n", stc);
                            fprintf(fd, "\tmov ecx, [ebp - %d]\n", stc + 4);
                            fprintf(fd, "\tsub eax, ecx\n");
                            fprintf(fd, "\tmov [ebp - %d], eax\n", stc);
                            break;
                        case '*':
                            flag = 1;
                            node = p->p1;
                            if ((node->type != typeOpr) && (node->type != typeId))
                                fprintf(fd, "\tmov DWORD [ebp - %d], ", stc);
                            dfs(p->p1);
                            if (node->type == typeId)
                                fprintf(fd, "\tmov [ebp - %d], eax\n", stc);
                            node = p->p2;
                            if (node->type != typeOpr) {
                                if (node->type != typeId)
                                    fprintf(fd, "\tmov DWORD [ebp - %d], ", stc + 4);
                            } else
                                stc = stc + 4;
                            dfs(p->p2);
                            if (node->type == typeOpr)
                                stc = stc - 4;
                            if (node->type == typeId)
                                fprintf(fd, "\tmov DWORD [ebp - %d], eax\n", stc + 4);
                            fprintf(fd, "\tmov eax, [ebp - %d]\n", stc);
                            fprintf(fd, "\tmov ecx, [ebp - %d]\n", stc + 4);
                            fprintf(fd, "\timul ecx\n");
                            fprintf(fd, "\tmov [ebp - %d], eax\n", stc);
                            break;
                        case '/':
                            flag = 1;
                            node = p->p1;
                            if ((node->type != typeOpr) && (node->type != typeId))
                                fprintf(fd, "\tmov DWORD [ebp - %d], ", stc);
                            dfs(p->p1);
                            if (node->type == typeId)
                                fprintf(fd, "\tmov [ebp - %d], eax\n", stc);
                            node = p->p2;
                            if (node->type != typeOpr) {
                                if (node->type != typeId)
                                    fprintf(fd, "\tmov DWORD [ebp - %d], ", stc + 4);
                            } else
                                stc = stc + 4;
                            dfs(p->p2);
                            if (node->type == typeOpr)
                                stc = stc - 4;
                            if (node->type == typeId)
                                fprintf(fd, "\tmov DWORD [ebp - %d], eax\n", stc + 4);
                            fprintf(fd, "\tmov eax, [ebp - %d]\n", stc);
                            fprintf(fd, "\tmov ecx, [ebp - %d]\n", stc + 4);
                            fprintf(fd, "\tmov edx, 0\n");
                            fprintf(fd, "\tidiv ecx\n");
                            fprintf(fd, "\tmov [ebp - %d], eax\n", stc);
                            break;
                        case 'a':
                            dfs(p->p1);
                            if(flag == 1) {
                                fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                op = strdup(p->key);
                            }
                            fprintf(fd, "\tmov ecx, eax\n");
                            dfs(p->p2);
                            if(flag == 1) {
                                fprintf(fd, "\tmov eax, [ebp - 4]\n");
                            }
                            fprintf(fd, "\tmov edx, eax\n");
                            fprintf(fd, "\tcmp ecx, edx\n");
                            fprintf(fd, "\tjne L%03d\n", ex2 = ex);
                            break;
                        case '!':
                            dfs(p->p1);
                            if(flag == 1) {
                                fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                op = strdup(p->key);
                            }
                            fprintf(fd, "\tmov ecx, eax\n");
                            dfs(p->p2);
                            if(flag == 1) {
                                fprintf(fd, "\tmov eax, [ebp - 4]\n");
                            }
                            fprintf(fd, "\tmov edx, eax\n");
                            fprintf(fd, "\tcmp ecx, edx\n");
                            fprintf(fd, "\tje L%03d\n", ex2 = ex);
                            break;
                        case '>':
                            switch (op[1]) {
                                case '=':
                                    dfs(p->p1);
                                    if(flag == 1) {
                                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                        op = strdup(p->key);
                                        flag = 0;
                                    }
                                    fprintf(fd, "\tmov ebx, eax\n");
                                    dfs(p->p2);
                                    if(flag == 1) {
                                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                    }
                                    fprintf(fd, "\tmov edx, eax\n");
                                    fprintf(fd, "\tcmp ebx, edx\n");
                                    fprintf(fd, "\tjl L%03d\n", ex2 = ex);
                                    break;
                                default:
                                    dfs(p->p1);
                                    if(flag == 1) {
                                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                        op = strdup(p->key);
                                    }
                                    fprintf(fd, "\tmov ebx, eax\n");
                                    dfs(p->p2);
                                    if(flag == 1) {
                                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                    }
                                    fprintf(fd, "\tmov edx, eax\n");
                                    fprintf(fd, "\tcmp ebx, edx\n");
                                    fprintf(fd, "\tjle L%03d\n", ex2 = ex);
                                    break;
                            }
                            break;
                        case '<':
                            switch (op[1]) {
                                case '=':
                                    dfs(p->p1);
                                    if(flag == 1) {
                                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                        op = strdup(p->key);
                                    }
                                    fprintf(fd, "\tmov ebx, eax\n");
                                    dfs(p->p2);
                                    if(flag == 1) {
                                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                    }
                                    fprintf(fd, "\tmov edx, eax\n");
                                    fprintf(fd, "\tcmp ebx, edx\n");
                                    fprintf(fd, "\tjg L%03d\n", ex2 = ex);
                                    break;
                                default:
                                    dfs(p->p1);
                                    if(flag == 1) {
                                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                        op = strdup(p->key);
                                    }
                                    fprintf(fd, "\tmov ebx, eax\n");
                                    dfs(p->p2);
                                    if(flag == 1) {
                                        fprintf(fd, "\tmov eax, [ebp - 4]\n");
                                    }
                                    fprintf(fd, "\tmov edx, eax\n");
                                    fprintf(fd, "\tcmp ebx, edx\n");
                                    fprintf(fd, "\tjge L%03d\n", ex2 = ex);
                                    break;
                            }
                            break;
                    }
                    break;
            }
    }
}

void excep(struct treenode *p, int ex2) {
    int ex1;
    char str[] = "else";
    if (p == NULL) {
        fprintf(fd, "L%03d:\n", ex2);
        return;
    }
    if (strcmp(str, p->key) == 0) {
        fprintf(fd, "\tjmp L%03d\n", ex1 = ex++);
        fprintf(fd, "L%03d:\n", ex2);
        dfs(p->p4);
        fprintf(fd, "L%03d:\n", ex1);
        dfs(p->p5);
    } else {
        fprintf(fd, "L%03d:\n", ex2);
        dfs(p);
    }
}

struct treenode *add_atr(struct treenode *first, struct treenode *second) {
    if (first->p5 == NULL) {
        first->p5 = second;
        return first;
    }
    struct treenode *node;
    char str[] = "else";
    node = first->p5;
    if (strcmp(node->key, str) == 0)
        node->p5 = second;
    return first;
}

struct treenode *create_node(char *key, nodeEnum type, struct treenode *p1, struct treenode *p2, struct treenode *p3, struct treenode *p4, struct treenode *p5) {
    struct treenode *node = malloc(sizeof(struct treenode));
    if (node == NULL)
        return NULL;
    node->key = strdup(key);
    node->type = type;
    node->p1 = p1;
    node->p2 = p2;
    node->p3 = p3;
    node->p4 = p4;
    node->p5 = p5;
    return node;
}