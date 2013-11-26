#ifndef __QUEUE_H__
#define __QUEUE_H__



typedef struct node {
	char cmd[4096];
	struct node *next;
} queue;

/******************************************************************************
 * Adds a command to the end of the queue.
 *
 * Arguments:
 *   commandstr - null terminated string holding command and parameter
 *   *cmd_queu_ptr - head of the queue
 *
 * Returns
 * 	head of the queue
 *
 * Original author: Justin Slind
 *****************************************************************************/
queue* addToQueue(char *commandstr, queue *cmd_queue_ptr);


/******************************************************************************
 * Pulls a command from the front of the queue.
 *
 * Arguments:
 *   commandstr - null termianted string holding command and parameter
 *   cmd_queu_ptr - head the queue
 *
 * Returns
 * 	returns 0 when function is complete
 *
 * Original author: Justin Slind
 *****************************************************************************/

queue* pullFromQueue(char *commandstr, queue *cmd_queue_ptr);

/******************************************************************************
 * frees allocated memory in the queue
 *
 * Arguments:
 *   cmd_queu_ptr - head the queue
 *
 * Returns
 * 	void
 *
 * Original author: Justin Slind
 *****************************************************************************/

void freeQueue(queue *cmd_queue_ptr);



#endif //__QUEUE_H__
