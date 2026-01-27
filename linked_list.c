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

// static char *ll_strdup(const char *s) {
// 	if (s == NULL) return NULL;
// 	size_t n = strlen(s) + 1;
// 	char *p = (char *)malloc(n);
// 	if (p != NULL) {
// 		memcpy(p, s, n);
// 	}
// 	return p;
// }

Node * add_newNode(Node* head, pid_t new_pid, char * new_path){
	// initialize a new node
	Node *new_node = (Node *)malloc(sizeof(Node));
	if (new_node == NULL) {
		// insertion failed
		return head;
	}

	new_node->pid = new_pid;
	new_node->next = NULL;

	// create copy of the path to store
	if (new_path != NULL) {
		new_node->path = strdup(new_path);
		if (new_node->path == NULL) {
			free(new_node);
			return head;
		}
	} else {
		new_node->path = NULL;
	}

	// if list is empty, make node head
	if (head == NULL) {
		return new_node;
	}

	// add new node to end
	Node *cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
	}
	cur->next = new_node;

	return head;
}

Node * deleteNode(Node* head, pid_t pid){
	if (head == NULL) {
		return NULL;
	}

	// if head, remove it
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
		printf("%d:\t\t%s\n", (int)cur->pid, cur->path);
		cur = cur->next;
	}
}

int listSize(Node *node) {
	Node *cur = node;
	int count = 0;
	while (cur != NULL) {
		count++;
		cur = cur->next;
	}
	return count;
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