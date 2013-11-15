/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment: Assignment #3 - ftp server
 * File: fakemain.c
 * Date: November 2013
 *
 * Description: This is by no means what our main should look like. This is
 *              a main program to test "net.c" and "net.h" with.
 *****************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include "net.h"

#include <string.h>

//This is Evan's fake main used to test his sockets and print-out.
int main (int argc, char *argv[])
{
  int i;
  int c_sfd, d_sfd, d2_sfd;//control and data connection socket file descriptors
  fd_set sfds;
  int sock[100];     //listening sockets.
  int nsock;         //number of elements in sock

  //create the sockets to listen for a control connection.
  if (get_control_sock (sock, &nsock) == -1) {
    fprintf (stderr, "%s: could not create control socket\n", argv[0]);
    return -1;
  }

  //accept control connections, create a pthread for each connection.
  while (1) {
    //accept a connection.
    if ((c_sfd = control_accept (sock, nsock)) == -1) {
      fprintf (stderr, "%s: error accepting control connection\n", argv[0]);
      continue;
    }
    break;
  }

  char cmd_str1[] = "PASV\n";
  //Create a data connnection socket.
  if ((d_sfd = cmd_pasv (c_sfd, cmd_str1)) == -1) {
    printf ("%s: error while creating data connection\n", argv[0]);
    return -1;
  }

  /* This block is TEMPORARY, a function shall be created for this.
   * accept() a data connection.
   * This loop will pass through once under most circumstances, however, it     
   * will restart if select fails with the errno "EINTR". */
  while (1) {
    /* Set the fd set bits that select will monitor. */
    FD_ZERO (&sfds);
    FD_SET (d_sfd, &sfds);

    if (select (d_sfd + 1, &sfds, NULL, NULL, NULL) == -1) {
      /* Restart the select loop if the select failure is not fatal. */
      if (errno == EINTR)
        continue;
      perror ("select");
      return -1;
    }

    if ((d_sfd = accept (d_sfd, NULL, NULL)) == -1) {
      /* Retry accept() if it returns a non-fatal error. */
      if (errno == EINTR)
	continue;
      perror ("accept");
      return -1;
    }
    /* Accept was successful, break from the select loop. */
    break;
  }

  char cmd_str2[] = "PORT 127,0,1,1,10,0\n";

  //Testing PORT command. PORT 127,0,1,1,10,0
  if ((d2_sfd = cmd_port (c_sfd, cmd_str2)) == -1) {
    fprintf (stderr, "cmd_port: could not create the connection\n");
  }

  //Mesg tells us if the data connection is reachable.
  printf ("data connection established. Exiting program...\n");

  //Close all sockets before exiting.
  close (c_sfd);
  close (d_sfd);
  close (d2_sfd);
  for (i = 0; i < nsock; i++)
    close (sock[i]);

  return 0;
}

/******************************************************************************
 * Futer function to read from the control socket. Add any commands ending in
 * a newline to the queue. (This will require a queue or linked list to
 * implement properly as testing at (ftp.gnu.org) has shown.
 *****************************************************************************/

/******************************************************************************
 * Futer function to accept a passive connection (data connection).
 *****************************************************************************/
