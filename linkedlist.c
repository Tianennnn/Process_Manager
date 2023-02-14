#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "emalloc.h"

typedef struct Node {
    pid_t pid;
    struct Node* next;
} Node;

/**
 * Function: create
 * ------------------
 * @brief  Create an node and store the process id to the node.
 *
 * @param pid The id of the process to be stored in the new node.
 *
 * @return The new node created.
 *
 */
Node* create(pid_t pid){
    Node* new = (Node*) malloc (sizeof(Node));
    if(new == NULL){
        printf("Failed to allocate memory.\n");
        exit(0);
    }
    new->pid = pid;
    new->next = NULL;

    return new;
}


/**
 * Function: printList
 * ------------------
 * @brief  Print the id and the path of the processes in the given list.
 *
 * @param n The list of background processes.
 */
void printList(Node* n) {
    while(n!=NULL){
        printf("%d: ", n->pid);
        FILE* temp;
        temp = popen("readlink -f inf", "r");
        int c = getc(temp);
        while (c != EOF)
        {
            putchar(c);
            c = getc(temp);
        }
        n = n->next;
    }
}


/**
 * Function: append_list
 * ------------------
 * @brief  Append the given node to the end of the list.
 *
 * @param head The list of background processes.
 * @param new The node to be appended to the end.
 *
 * @return The head of the new list.
 */
Node* append_list( Node* head,  Node* new) {
    if(head == NULL){
        head = new;
    }
    else{
        while (head->next != NULL) {
            head = head->next;
        }
        head->next = new;
    }
    return head;
}


/**
 * Function: count
 * ------------------
 * @brief  Count the number of processes in the list.
 *
 * @param head The list of background processes.
 *
 * @return The number of processes in the list.
 */
int count(Node* head){
    int counter = 0;
    while (head != NULL) {
        head = head->next;
        counter ++;
    }
    return counter;
}


/**
 * Function: if_exist
 * ------------------
 * @brief  Check if the process with the given pid is in the list of 
 *         the background processes.
 *
 * @param head The list of background processes.
 * @param pid The id of the background process to be looked for.
 *
 * @return 1 if the process exists in the list; 0 otherwise.
 */
int if_exist(Node* head, pid_t pid){
    int result = 0;
    while (head != NULL) {
        if(head->pid == pid){
            result = 1;
            break;
        }
        head = head->next;
    }
    return result;
}


/**
 * Function: list_remove
 * ------------------
 * @brief  remove the node with the input pid from the list.
 *
 * @param head The list of background processes.
 * @param pid The id of the background process to be removed.
 *
 * @return The (new) head of the list.
 */
Node* list_remove(Node* head, pid_t pid){
    // when there is at least one element in the list
    if(head!=NULL){
        // check if the head is to be removed
        if (head->pid == pid){
            head = NULL;
        }
        else{
            while (head->next != NULL) {
                if(head->next->pid == pid){
                    Node* new_next = head->next->next;
                    head->next = NULL;
                    head->next = new_next;
                    break;
                }
                head = head->next;
            }
        }
    }
    return head;
}


/**
 * Function: free_list
 * ------------------
 * @brief  Free the space dynamically allocated to the list.
 *
 * @param head The list of background processes.
 *
 */
void free_list(Node* head) {
    Node* temp = NULL;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
