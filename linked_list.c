#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linked_list.h"

static char *ll_strdup(const char *s) {
	if (s == NULL) return NULL;
	size_t n = strlen(s) + 1;
	char *p = (char *)malloc(n);
	if (p != NULL) {
		memcpy(p, s, n);
	}
	return p;
}
 
Node * add_newNode(Node* head, pid_t new_pid, char * new_path){
	Node *new_node = (Node *)malloc(sizeof(Node));
	if (new_node == NULL) {
		return head;
	}

	new_node->pid = new_pid;
	new_node->next = NULL;

	if (new_path != NULL) {
		new_node->path == ll_strdup(new_path);
		if (new_node->path == NULL) {
			free(new_node);
			return head;
		}
	} else {
		new_node->path = NULL;
	}
}

Node * deleteNode(Node* head, pid_t pid){
	if (head == NULL) {
		return NULL;
	}

	//if head, remove it
	if (head->pid == pid) {
		Node *next = head->next;
		free(head->path);
		free(head);
		return next;
	}

	Node *prev = head;
	Node *cur = head->next;
	while (cur != NULL) {
		if (cur->pid == pid) {
			prev->next = cur->next;
			free(cur->path);
			free(cur);
			break;
		}
		prev = cur;
		cur = cur->next;
	}
	return head;
}

void printList(Node *node){
	Node *cur = node;
	while (cur != NULL) {
		if (cur->path != NULL) {
			printf("%d\t%s\n", (int)cur->pid, cur->path);
		} else {
			printf("%d\t$s\n", (int)cur->pid, "(null)");
		}
		cur = cur->next;
	}
}


int PifExist(Node *node, pid_t pid){
	Node *cur = node;
	while (cur != NULL) {
		if (cur->pid == pid) {
			return 1;
		}
		cur = cur->next;
	}
  	return 0;
}

