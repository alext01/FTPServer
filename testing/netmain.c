/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment: Assignment #3 - ftp server
 * File: netmain.c
 * Date: November 2013
 *
 * Description: A program used to test the network functions of the server.
 *              This file must not be included in the final submission.
 *****************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "../config.h"
#include "../net.h"

//Network testing main. Do not confuse this with "main.c" function main().
//Read the file header.
int main (int argc, char *argv[])
{
  int c_sfd, d_sfd, d2_sfd;//control and data connection socket file descriptors
  int c_listen_sfd;

  //Temporary, recv a command from the client over a socket.
  char buf[80];
  int nread;

  //Get my IP.
  char my_ip[INET_ADDRSTRLEN];
  char *setting = "INTERFACE_CONFIG";
  char *interface;

  //Get the interface from the config file.
  if ((interface = get_config_value ((const char *)setting)) == NULL)
    return -1;

  //Get the IPv4 address for the interface specified in the config file.
  if (get_interface_address ((const char *)interface, &my_ip) == -1)
    return -1;
  free (interface); //Free heap memory.
  printf ("Server IP address: %s\n", my_ip);

  //create the sockets to listen for a control connection.
  if ((c_listen_sfd = get_control_sock ()) == -1) {
    fprintf (stderr, "%s: could not create control socket\n", argv[0]);
    return -1;
  }

  //Accept a control connection.
  while (1) {
    if ((c_sfd = accept_connection (c_listen_sfd, ACCEPT_CONTROL)) == -1) {
      fprintf (stderr, "%s: error accepting control connection\n", argv[0]);
      continue;
    }
    break;
  }
  printf ("control connection accepted\n");



  //Read a message, no guarantee the entire message is read.
  nread = 0;
  if ((nread = recv (c_sfd, buf, 80, 0)) == -1) {
    fprintf (stderr, "%s: recv: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }
  buf[nread] = '\0';


  //Create a data connnection socket.
  if ((d_sfd = cmd_pasv (c_sfd, buf)) == -1) {
    printf ("%s: error while creating data connection\n", argv[0]);
    return -1;
  }
 

  //Accept data connection on the socket created with the PASV command.
  while (1) {
    if ((d_sfd = accept_connection (d_sfd, ACCEPT_PASV)) == -1) {
      fprintf (stderr, "%s: error accepting control connection\n", argv[0]);
      continue;
    }
    break;
  }
  printf ("passive socket connection accepted\n");



  //Read a message, no guarantee the entire message is read.
  nread = 0;
  if ((nread = recv (c_sfd, buf, 80, 0)) == -1) {
    fprintf (stderr, "%s: recv: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }
  buf[nread] = '\0';



  //Testing PORT command.
  if ((d2_sfd = cmd_port (c_sfd, buf)) == -1) {
    fprintf (stderr, "cmd_port: could not create the connection\n");
  } else {
    //Mesg tells us if the data connection is reachable.
    printf ("data connection established. Exiting program...\n");
  }

  //Close all sockets before exiting.
  close (c_listen_sfd);
  close (c_sfd);
  close (d_sfd);
  close (d2_sfd);

  return 0;
}
