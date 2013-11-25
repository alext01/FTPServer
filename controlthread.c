/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: controlthread.c
 * Date: November 2013
 *
 * Description:
 *    This file contains the functions that start a thread for an established
 *    control connection with a client. Global variables that are modified
 *    by these threads are done so by functions found here.
 *
 * Acknowledgements:
 *    Evan - I learned how to use pthreads with these sections of this tutorial
 *           website, and of course many manpages.
 *           https://computing.llnl.gov/tutorials/pthreads/#CreatingThreads
 *           https://computing.llnl.gov/tutorials/pthreads/#Joining
 *           https://computing.llnl.gov/tutorials/pthreads/#Mutexes
 *****************************************************************************/
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controlthread.h"
#include "response.h"
#include "queue.h"

#include <unistd.h> //required for sleep(), remove me later.


/******************************************************************************
 * control_thread - see controlthread.h
 *
 * NOT FINAL!!
 *****************************************************************************/
void *control_thread (void * arg) {
  int *c_sfd = arg;
  struct queue *cmd_queue_ptr;

  //Send the welcome message to the client.
  while (send_welcome_mesg_220 (*c_sfd) != 0);

  //Transfer control of the thread to session() to perform user commands.
  session (*c_sfd, &cmd_queue_ptr);

  //Free all heap memory. Close sockets which are no longer required.
  close (*c_sfd);
  free (c_sfd);
  if (cmd_queue_ptr != NULL) {
    //Free queue function().
  }

  //Decrement the active control thread count and terminate the thread.
  modify_cthread_count (-1);
  return NULL;
}

/******************************************************************************
 * modify_cthread_count - see controlthread.h
 *****************************************************************************/
int modify_cthread_count (int value)
{
  if (value == 0)
    return 0;

  //Lock the mutex.
  if (pthread_mutex_lock (&control_count_mutex) == -1) {
    fprintf (stderr, "%s: pthread_mutex_lock: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }

  //Modify the count, only one thread can be added or removed at one time.
  if (value > 0)
    active_control_threads++;
  else
    active_control_threads--;

  //Unlock the mutex.
  if (pthread_mutex_unlock (&control_count_mutex) == -1) {
    fprintf (stderr, "%s: pthread_mutex_unlock: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }

  return 0;
}


/******************************************************************************
 * get_cthread_count - see controlthread.h
 *****************************************************************************/
int get_cthread_count (void)
{
  int count;

  //Lock the mutex.
  if (pthread_mutex_lock (&control_count_mutex) == -1) {
    fprintf (stderr, "%s: pthread_mutex_lock: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }

  count = active_control_threads;

  //Unlock the mutex.
  if (pthread_mutex_unlock (&control_count_mutex) == -1) {
    fprintf (stderr, "%s: pthread_mutex_unlock: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }

  //The count should never be negative. Treat negative values as an error.
  return count;
}
