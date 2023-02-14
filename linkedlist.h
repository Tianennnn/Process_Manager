#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

    typedef struct Node {
        pid_t pid;
        struct Node *next;
    } Node;

    Node* create(pid_t pid);
    void printList( Node *n);
    Node* append_list( Node *head,  Node *new);
    int count(Node* head);
    int if_exist(Node *head, pid_t pid);
    Node* list_remove(Node* head, pid_t pid);
    void free_list(Node *head);
#endif
