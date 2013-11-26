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
#include "../session.h"

//Network testing main. Do not confuse this with "main.c" function main().
//Read the file header.
int main (int argc, char *argv[])
{
  int c_listen_sfd;
  session_info_t session;

  //Temporary, recv a command from the client over a socket.
  char buf[80];
  int nread;

  //Get my IP.
  char my_ip[INET_ADDRSTRLEN];
  char *setting = "INTERFACE_CONFIG";
  char *interface;

  //Get the interface from the config file.
  if ((interface = get_config_value (setting,
				     FTP_CONFIG_FILE)) == NULL)
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
    if ((session.c_sfd = accept_connection (c_listen_sfd, 
					    ACCEPT_CONTROL,
					    NULL)) == -1) {
      fprintf (stderr, "%s: error accepting control connection\n", argv[0]);
      continue;
    }
    break;
  }
  printf ("control connection accepted\n");



  //Read a message, no guarantee the entire message is read.
  nread = 0;
  if ((nread = recv (session.c_sfd, buf, 80, 0)) == -1) {
    fprintf (stderr, "%s: recv: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }
  buf[nread] = '\0';


  //Create a data connnection socket.
  /*if ((session.d_sfd = cmd_pasv (&session, buf)) == -1) {
    printf ("%s: error while creating data connection\n", argv[0]);
    return -1;
  } else {
    printf ("passive socket connection accepted\n");
    } */


  //Read a message, no guarantee the entire message is read.
  /*nread = 0;
  if ((nread = recv (session.c_sfd, buf, 80, 0)) == -1) {
    fprintf (stderr, "%s: recv: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }
  buf[nread] = '\0';
  */


  //Testing PORT command.
  while ((session.d_sfd = cmd_port (&session, buf)) == -1) {
    fprintf (stderr, "cmd_port: could not create the connection\n");
  }
  //Mesg tells us if the data connection is reachable.
  printf ("data connection established. Exiting program...\n");

  //Close all sockets before exiting.
  close (c_listen_sfd);
  close (session.c_sfd);
  close (session.d_sfd);

  return 0;
}
