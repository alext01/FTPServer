#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "linkedlist.h"
#include "threadparam.h"

void *server_thread (void * arg) {
	threadparam *params = (threadparam*) arg;
	printf("New thread %d started.\n",params->sock);
	sleep(10);
	*(params->complete) = true;
	printf("Thread %d done.  Cleaning up\n", params->sock);
	free(params);
	return NULL;
}


int main (int argc, char *argv[]) {

	thread_node *head = NULL, *tail = NULL, *iter = NULL, *prev = NULL;

	threadparam *params = NULL;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int sock = 0;

	while (1)
	{
		getchar();
		sock++;

		//if linked list is empty, create new head
		if (head == NULL) {
			head = (thread_node*)malloc(sizeof(thread_node));
			tail = head; 
		} 
		//otherwise add new node to tail
		else {
			tail->next = (thread_node*)malloc(sizeof(thread_node));
			tail = tail->next;
		}

		tail->next = NULL;
		tail->complete = false;
		//allocate memory for thread parameters
		params = (threadparam*)malloc(sizeof(threadparam));
		params->sock = sock;
		params->complete = &(tail->complete);
		printf("Creating thread\n");

		//create thread to handle client
		pthread_create(&(tail->thread), &attr, &server_thread, (void*) params);

		iter = head;
		prev = 0;
		while(iter) {
			if (iter->complete == true) {
				if (iter == head)
					head = head->next;
				else
					prev->next = iter->next;
				pthread_join(iter->thread,NULL);
				free(iter);
				if (prev)
					iter = prev->next;
				else
					iter = NULL;
			} else
			{
				prev = iter;
				iter = iter->next;
			}
		}

	}


	return 0;
}
