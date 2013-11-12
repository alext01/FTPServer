#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <pthread.h>
#include <stdbool.h>


typedef struct node{
	pthread_t thread;
	bool complete;
	struct node *next;
} thread_node;


#endif //__LINKEDLIST_H__
