/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: net.c
 * Date: November 2013
 *
 * Description: 
 *****************************************************************************/
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "net.h"

#define BACKLOG 10      //Maximum number of clients queued for accept(2).
#define DEF_PORT "2222" //The default port for the control connection.
#define MAXSOCK 100     //Maximum number of sockets for the control connection.
#define BITS_IN_BYTE 8  //The number of bits in a byte.
#define STD_TERM_SZ 80  //The standard number of characters in a terminal line.


/******************************************************************************
 * get_control_sock
 *****************************************************************************/
int get_control_sock (int *sock, int *nsock)
{
  struct addrinfo hints, *result, *iter;  //getaddrinfo() variables
  int gai;           //getaddrinfo() error string
  int optval;        //used to set sock options with setsockopt()
  int sfd;           //the current socket file descriptor

  //Ensure all bits are first set to zero for the hints to be accurate.
  bzero (&hints, sizeof (hints));
  hints.ai_family = AF_UNSPEC;       //IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;   //stream socket
  hints.ai_flags = AI_PASSIVE;       //use local IP address

  //Populate a linked list of 'struct addrinfo' elements.
  if ((gai = getaddrinfo (NULL, DEF_PORT, &hints, &result)) != 0) {
    fprintf (stderr, "\ngetaddrinfo: %s\n", gai_strerror (gai));
    return -1;
  }

  /* Iterate through the linked list returned from getaddrinfo(). Attempt to
   * create a listening socket for each iteration. */
  *nsock = 0;
  for (iter = result; iter != NULL && *nsock < MAXSOCK; iter = iter->ai_next) {
    //Create a new unnamed (unbound) socket.
    if ((sfd = socket (iter->ai_family, 
		       iter->ai_socktype,
		       iter->ai_protocol)) == -1)
      continue; //failed, try the next element

    /* Set socket option to reuse the port while the port is in a TIME_WAIT 
     * state. This will allow the server to be restarted faster. */
    optval = 1;
    if (setsockopt (sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval))
	== -1) {
      close (sfd);
      continue;
    }

    //Only create an IPv6 socket when using an IPv6 address.
    if (iter->ai_family == AF_INET6) {
      if (setsockopt (sfd, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof (optval))
	  == -1) {
	//failed, close the socket and try the next element
	close (sfd);
	continue;
      }
    }
      
    //Bind the unnamed socket to a local address, to allow connections.
    if (bind (sfd, iter->ai_addr, iter->ai_addrlen) == -1) {
      //failed, close the socket and try the next element
      close (sfd);
      continue;
    }

    //Set the bound socket to accept incoming connection requests.
    if (listen (sfd, BACKLOG) == -1) {
      //failed, close the socket and try the next element
      close (sfd);
      continue;
    }

    /* The socket is now listening to the default port. Store the socket
     * descriptor, and increase the socket count. */
    sock[*nsock] = sfd;
    *nsock += 1;
  }

  freeaddrinfo (result);  //Free the getaddrinfo() result

  //Return error if no listening sockets were created.
  if (*nsock == 0) {
    fprintf (stderr, "%s: no control sockets were created\n", __FUNCTION__);
    return -1;
  }

  return 0;
}

/******************************************************************************
 * Accept a control connection. This function is TEMPORARY, it is only designed
 * to accept() ONE connection per call to this function. In the current form 
 * none of the socket file descriptors are closed. If we decide to keep this
 * function, all comments and code in this function should be carefuly checked
 * and re-written if necessary.
 *
 * Not knowing how we will accept the connections fully I have created this
 * temporary design to be used as follows:
 * in function main:
 *    while (TRUE) {
 *       get_con_sock();
 *       sfd = con_accept();
 *       do something (create a pthread...) with the sfd here;
 *    }
 *
 * Arguments:
 *   sock  - An array of listening sockets ready to accept connections.
 *   nsock - The number of listening sockets that is found in the array passed
 *           in the first argument.
 *
 * Return values:
 *   >0   The socket file descriptor that accepted a connection.
 *   -1   error
 *
 * Original author (this function is temporary): Evan Myers
 *****************************************************************************/
int control_accept (int *sock, int nsock)
{
  int sfd;              //the current socket file descriptor
  int max_sfd;          //select() variable
  fd_set master, rfds;  //rfds set to master set
  int i;

  /* Set the master fd_set for all the sockets to read (accept) from. The read
   * socket file descriptors that select monitors will be set to the master
   * list of socket file descriptors before each call of select, removing this
   * block from the loop. 
   *
   * NOT A FINAL COMMENT: this block may be removed if we decide to only
   *                      accept one connection per function call. */
  FD_ZERO (&master);
  max_sfd = 0;
  for (i = 0; i < nsock; i++) {
    FD_SET (sock[i], &master);
    /* Keep track of the largest sfd, to be used in the first argument field 
     * of select(). */
    if (sock[i] > max_sfd)
      max_sfd = sock[i];
  }

  while (1) {  //This loop can be converted into a "goto" statement. NOT FINAL.
    rfds = master;
    if (select (max_sfd + 1, &rfds, NULL, NULL, NULL) == -1) {
      //Restart the loop if the select error is not fatal.
      if (errno == EINTR)
	continue;
      fprintf (stderr, "%s: select: %s\n", __FUNCTION__, strerror (errno));
      return -1;
    }
    
    //Attempt to accept a connection with the named socket.
    for (i = 0; i < nsock; i++) {
      if (FD_ISSET (sock[i], &rfds)) {
	if ((sfd = accept (sock[i], NULL, NULL)) == -1) {
	  // Retry accept() for this descriptor if a non-fatal error.
	  if (errno == EINTR) {
	    i--;
	    continue;  //restart the for loop with the same sfd
	  }
	  fprintf (stderr, "%s: accept: %s\n", __FUNCTION__, strerror (errno));
	  return -1;
	}
	break;  //for loop
      }
    }
    break; //while loop
  }

  return sfd;  //Return the accepted socket file descriptor.
}

/******************************************************************************
 * cmd_pasv
 *****************************************************************************/
int cmd_pasv (int c_sfd)
{
  int d_sfd; //data connection socket file descriptor

  /* Create a data connection socket that is ready to accept a connection from
   * the client. */
  if ((d_sfd = get_pasv_sock()) == -1)
    return -1;

  if (send_msg_227 (c_sfd, d_sfd) == -1)
    return -1;

  return d_sfd;
}

/******************************************************************************
 * get_pasv_sock
 *****************************************************************************/
int get_pasv_sock (void)
{
  struct addrinfo hints, *result;   //getaddrinfo()
  int gai;         //getaddrinfo error string.
  int sfd;         //the file descriptor of the data connection socket

  //Used to extract the external IP address. +1 for a null terminator.
  char hostname[_SC_HOST_NAME_MAX + 1];
  // http://permalink.gmane.org/gmane.comp.lib.gnulib.bugs/18258

  //set the gettaddrinfo() hints
  bzero (&hints, sizeof(hints));
  hints.ai_family = AF_INET;        //IPv4, four byte address
  hints.ai_socktype = SOCK_STREAM;  //the data connection is stream
  hints.ai_flags = AI_PASSIVE;

  /* Get the name of the local host, use this value as the node argument in
   * getaddrinfo(). */
  gethostname (hostname, sizeof (hostname));

  /* Populate the linked list found in result. Use the host name entry on the
   * routing table for the node argument. "0" in the second argument will
   * allow  the operating system to choose an available port when bind() is
   * called. */
  if ((gai = getaddrinfo (hostname, "0", &hints, &result)) == -1) {
    fprintf (stderr, "%s: getaddrinfo: %s\n", __FUNCTION__, gai_strerror (gai));
    return -1;
  }

  /* Iterate through all elements of the linked list returned by getaddrinfo().
   * If any command required to create the data connection socket fails, try
   * the next element returned by getaddrinfo(). */
  if ((sfd = socket (result->ai_family,
		     result->ai_socktype,
		     result->ai_protocol)) == -1) {
    fprintf (stderr, "%s: socket: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }

  /* Bind to socket to a port. The port is chosen by the operating system
   * with the call to bind() because "0" was passed to the getaddrinfo()
   * service argument. */
  if (bind (sfd, result->ai_addr, result->ai_addrlen) == -1) {
    fprintf (stderr, "%s: bind: %s\n", __FUNCTION__, strerror (errno));
    close (sfd);
    return -1;
  }

  //Allow the socket to accept() connections.
  if (listen (sfd, BACKLOG) == -1) {
    fprintf (stderr, "%s: listen: %s\n", __FUNCTION__, strerror (errno));
    close (sfd);
    return -1;
  }

  freeaddrinfo (result);  //Free the getaddrinfo() result
  return sfd;             //Return the listening data connection socket
}

/******************************************************************************
 * send_msg_227
 *****************************************************************************/
int send_msg_227 (int c_sfd, int d_sfd)
{
  struct sockaddr_in my_addr;
  socklen_t addr_len;
  uint8_t mesg[STD_TERM_SZ];
  int mesg_len;

  /* Used to collect the the decimal value of each byte, which will be sent to
   * the client as specified rfc 959. */
  int h1, h2, h3, h4;         //The bytes of the IPv4 address
  int p1, p2;                 //The bytes of the port

  /* Initialize addr_len to the size of an IPv4 address. If getsockname()
   * modifies this value, an IPv4 address was not used to create the PASV
   * socket. */
  addr_len = sizeof(my_addr);

  //Collect the address info
  if (getsockname (d_sfd, (struct sockaddr *)&my_addr, &addr_len) == -1)
    return -1;

  //Ensure the passive socket has a 4-byte address.
  if (addr_len > sizeof(my_addr)) {
    //The socket is not IPv4, the socket is invalid for the command PASV.
    return -1;
  }

  /* Ensure the IPv4 address bytes will be sent to the client in the same order
   * on all systems by converting to network-byte-order before calculating
   * the value of each byte field. */
  my_addr.sin_addr.s_addr = htonl (my_addr.sin_addr.s_addr);
  /* Store each byte of the IPv4 address as a decimal value in preperation for
   * sending the passive mode message. */
  h1 = (my_addr.sin_addr.s_addr & 0xFF000000) >> (3 * BITS_IN_BYTE);
  h2 = (my_addr.sin_addr.s_addr & 0x00FF0000) >> (2 * BITS_IN_BYTE);
  h3 = (my_addr.sin_addr.s_addr & 0x0000FF00) >> (BITS_IN_BYTE);
  h4 = (my_addr.sin_addr.s_addr & 0x000000FF);

  /* Ensure the port bytes will be sent to the client in the same order
   * on all systems by converting to network-byte-order before calculating
   * the value of each byte field. */
  my_addr.sin_port = htons (my_addr.sin_port);
  /* Store each byte of the port as a decimal value in preperation for
   * sending the passive mode message. */
  p1 = (my_addr.sin_port & 0xFF00) >> BITS_IN_BYTE;
  p2 = (my_addr.sin_port & 0x00FF);

  //Create the feedback message, code 227.
  sprintf ((char *)mesg, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n", 
	   h1, h2, h3, h4, p1, p2);

  mesg_len = strlen ((char *)mesg) + 1;
  mesg[mesg_len] = '\0';

  //Send the feedback message to the control socket.
  if (send_all (c_sfd, mesg, mesg_len) == -1) {
    return -1;
  }

  return 0;
}

/******************************************************************************
 * cmd_port
 *****************************************************************************/
int cmd_port (int c_sfd, char *addr)
{
  int d_sfd;   //The data connection socket file descriptor

  //The address to connect to for the data connecti
  char hostname[INET_ADDRSTRLEN];  //Maximum size of an IPv4 dot notation addr.
  char service[MAX_PORT_STR];      

  //Convert the address h1,h2,h3,h4,p1,p2 into a hostname and service.
  if (get_port_address (&hostname, &service, addr) == -1) {
    //Error message
    return -1;
  }

  //Create a data connection to the hostname and service provided by the client.
  if ((d_sfd = port_connect (hostname, service)) == -1) {
    //Error message
    return -1;
  }
  
  return d_sfd;
}

/******************************************************************************
 * get_port_address
 *****************************************************************************/
int get_port_address (char (*hostname)[INET_ADDRSTRLEN], 
		      char (*service)[MAX_PORT_STR], 
		      char *cmd_str)
{
  /* Used to collect each byte of the hostname that was passed to the function
   * in the string argument 'addr'. */
  int h1, h2, h3, h4;

  /* The port variables have been set to uint16_t so that they can be converted
   * to network-byte-order using htons(). Each port byte is collected and
   * converted to network-byte-order, then the bits are shifted to create one
   * integer. After the shift is complete the value is converted to 
   * host-byte-order before being converted to a string. */
  uint16_t p1, p2;
  
  //Collect each byte from the address string passed to the function.
  while (1) {
    if (sscanf (cmd_str, "%d%d%d%d%hu%hu", &h1, &h2, &h3, &h4,
		(unsigned short *)&p1, (unsigned short *)&p2) == -1) {
      //Check for errors, errno is non-fatal so restart the loop in that case.
      if (errno == EINTR)
	continue;
      fprintf (stderr, "%s: sscanf: %s\n", __FUNCTION__, strerror (errno));
      return -1;
    }
    break; //If sscanf() returned no errors break from the loop.
  }

  //Store the hostname in IPv4 dot notation.
  sprintf (*hostname, "%d.%d.%d.%d", h1, h2, h3, h4);

  /* Convert the port to network-byte-order so that the bit shifting will have
   * the correct effect on all systems. */
  p1 = htons (p1);
  p2 = htons (p2);
  //Combine the two port bytes to create one integer.
  p2 = p1 >> BITS_IN_BYTE;
  p2 = p1 | p2;
  // Convert the integer to host-byte-order before creating the string.
  p2 = ntohs (p2);
  //Store the integer as a string.
  sprintf (*service, "%hu", (unsigned short)p2);

  //The hostname and service have been set, return from the function.
  return 0;
}
  
/******************************************************************************
 * port_connect
 *****************************************************************************/
int port_connect (char *hostname, char *service)
{
  struct addrinfo hints, *result;   //getaddrinfo()
  int sfd;         //the file descriptor of the data connection socket
  int gai;         //getaddrinfo() error string

  //set the gettaddrinfo() hints
  bzero (&hints, sizeof(hints));
  hints.ai_family = AF_INET;        //IPv4, four byte address
  hints.ai_socktype = SOCK_STREAM;  //the data connection is stream

  //Create the address information using the hostname and service.
  if ((gai = getaddrinfo (hostname, service, &hints, &result)) == -1) {
    fprintf (stderr, "%s: getaddrinfo: %s\n", __FUNCTION__, gai_strerror (gai));
    return -1;
  }

  //Create the socket with the values returned by getaddrinfo().
  if ((sfd = socket (result->ai_family,
		     result->ai_socktype,
		     result->ai_protocol)) == -1) {
    fprintf (stderr, "%s: socket: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }

  //Connect to the client.
  if (connect (sfd, result->ai_addr, result->ai_addrlen) == -1) {
    fprintf (stderr, "%s: connect: %s\n", __FUNCTION__, strerror (errno));
    return -1;
  }

  freeaddrinfo (result);  //Free the getaddrinfo() result.
  return sfd;             //Return the data connection socket file descriptor.
}

/******************************************************************************
 * send_all
 *****************************************************************************/
int send_all (int sfd, uint8_t *mesg, int mesg_len)
{
  int nsent = 0;
  int to_send = mesg_len;

  while (to_send > 0) {
    if ((nsent = send (sfd, mesg, to_send, 0)) == -1) {
      //handle errors, EINTR is non-fatal
      if (errno == EINTR)
	continue;
      fprintf (stderr, "%s: %s\n", __FUNCTION__, strerror (errno));
      return -1;
    }
    //Update the number of bytes to send over the connection.
    to_send -= nsent;
    nsent = 0;
  }

  return 0;
}


