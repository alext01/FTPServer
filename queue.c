#include <string>
#include "queue.h"



queue* addToQueue(char commandstr, queue *cmd_queue_ptr) {
	queue *temp, *newptr = malloc(sizeof(queue));
	strcpy(newptr->cmd,commandstr);


	if (cmd_queue_ptr) {
		temp = cmd_queue_ptr
		while(temp->next) //get to the of the list
			temp = temp->next;
		temp->next = newptr; //add node to the end
	}

	else
		cmd_queue_ptr = newptr; //otherwise node is the head


	return cmd_queue_ptr;
}

void pullFromQueue(char commandstr, queue *cmd_queue_ptr) {
	queue *tempptr =
	strcpy(commandstr, cmd_queue_ptr->cmd);


}

