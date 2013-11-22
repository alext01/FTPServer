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

#include <unistd.h> //required for sleep(), remove me later.





/******************************************************************************
 * control_thread - see controlthread.h
 *
 * NOT FINAL!!
 *****************************************************************************/
void *control_thread (void * arg) {
  int *c_sfd = arg;
  printf ("Starting thread %d\n", *c_sfd);
  sleep(5);
  printf("Thread done. Cleaning up %d\n", *c_sfd);
  close (*c_sfd);
  free (c_sfd);

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
