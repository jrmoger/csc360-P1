#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct Node Node;

struct Node{
    pid_t pid;
    char * path;
    Node * next;
};

Node * add_node(Node* head, pid_t new_pid, char * new_path);
Node * delete_node(Node* head, pid_t pid);
void print_list(struct Node *node);
int pid_exists(Node *node, pid_t pid);
int list_size(Node *node);



#endif