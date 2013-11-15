/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: net.c
 * Date: November 2013
 *
 * Description:
 *   The functions found in this file create, connect, or accept sockets or
 *   socket connections. Functions that send or recieve data from a socket are
 *   also found in this file. The PASV and PORT commands, which create and/or
 *   connect a socket to establish a data connection are found in this file.
 *
 * Acknowledgements:
 *   ONE - I have discovered that the printf/scanf family has a conversion
 *         specifier for type uinN_t: http://stackoverflow.com/a/6993177
 *         I have used the ideas in this example in my code. Search for "ONE"
 *         in this file for the code blocks that use the macro.
 *****************************************************************************/
#include <arpa/inet.h>
#include <errno.h>
#include <inttypes.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "net.h"
#include "response.h"
///////////////////////////////////////////////////////////////////////////////
/* These are functions Evan will be creating in the near futer. One should not
 * expect these functions to be fully tested or functional before the end of
 * the weekend.
 *
 * TODO list:
 *   1) Create a function to read a command from the control connection.
 *
 *   2) Create a function, or modify control_accept(), to accept a data
 *      connection on the socket created by cmd_pasv().
 *
 *   3) Create a function that will monitor the control connection for data
 *      to read. If the PASV command has been called, and is not yet connected
 *      to the client, a read signal (FD_ISSET) will call the function (2) in
 *      this TODO list.
 *          If the data connection has already been established, then this 
 *      function will monitor the socket for read so as to preform a STORE
 *      command for example. This will require an (integer?) value to be passed
 *      to the function from main() that tells the function whether it should
 *      be expected to recieve data from the data connection socket. */
///////////////////////////////////////////////////////////////////////////////


/******************************************************************************
 * Socket connection/creation constants.
 *****************************************************************************/
#define BACKLOG 10      //Maximum number of clients queued for accept(2).
#define DEF_PORT "2222" //The default port for the control connection.


/******************************************************************************
 * PORT command error checking constants.
 *****************************************************************************/
/* The maximum string length of a PORT command (including the arguments).
 * (PORT + space) + (6 three digit fields) + (5 commas) + (newline + null) */
#define MAX_PORT_STR_LEN (5 + (6*3) + 5 + 2)

/* The minimum string length of a PORT command (includeing the arguments).
 * (PORT + space) + (6 one digit fields) + (5 commas) + (newline + null) */
#define MIN_PORT_STR_LEN (5 + (6*1) + 5 + 2)

//The number of byte values in the PORT command argument. PORT h1,h2,h3,h4,h5,h6
#define PORT_BYTE_ARGS 6


/******************************************************************************
 * Various maximum value constants.
 *****************************************************************************/
#define MAX_8_BIT  255  //The maximum 8-bit value


/******************************************************************************
 * Local function prototypes.
 *****************************************************************************/
//used by cmd_pasv()
static int get_pasv_sock (void);

//used by cmd_port()
static int get_port_address (int c_sfd,
			     char (*hostname)[INET_ADDRSTRLEN], 
			     char (*service)[MAX_PORT_STR], 
			     char *cmd_str);

//used by cmd_port()
static int port_connect (char *hostname, char *service);


/******************************************************************************
 * get_control_sock - see net.h
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
  for (iter = result; iter != NULL && *nsock < MAX_SOCK; iter = iter->ai_next) {
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
 * control_accept - see net.h
 *****************************************************************************/
int control_accept (int *sock, int nsock)
{
  int sfd;           //the current socket file descriptor
  int max_sfd;       //select() argument
  fd_set rfds;
  int i;

  while (1) {
    //Set the read fd_set bits.
    FD_ZERO (&rfds);
    max_sfd = 0;
    for (i = 0; i < nsock; i++) {
      FD_SET (sock[i], &rfds);
      /* Keep track of the largest sfd, to be used in the first argument field 
       * of select(). */
      if (sock[i] > max_sfd)
	max_sfd = sock[i];
    }

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
 * cmd_pasv - see net.h
 *****************************************************************************/
int cmd_pasv (int c_sfd, char *cmd_str)
{
  int d_sfd; //data connection socket file descriptor
  char expected_str[] = "PASV\n";

  if (strcmp (cmd_str, expected_str) != 0) {
    send_mesg_500 (c_sfd);
    return -1;
  }

  /* Create a data connection socket that is ready to accept a connection from
   * the client. */
  if ((d_sfd = get_pasv_sock()) == -1)
    return -1;

  if (send_mesg_227 (c_sfd, d_sfd) == -1)
     return -1;
 
  return d_sfd;
}


/******************************************************************************
 * Create an TCP socket for the PASV server command. Only the address returned
 * from gethostname() is used to make the socket, so this function should not
 * be used for creating the sockets of the control connection.
 *
 * Return values:
 *    > 0   The file descriptor for the newly created data connection socket.
 *     -1   Error while creating the socket.
 *
 * Original author: Evan Myers
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
 * cmd_port - see net.h
 *****************************************************************************/
int cmd_port (int c_sfd, char *cmd_str)
{
  int d_sfd;        //The data connection socket file descriptor.
  int cmd_str_len;  //The length of the command string.
  //The data connection address to connect to.
  char hostname[INET_ADDRSTRLEN];  //Maximum size of an IPv4 dot notation addr.
  char service[MAX_PORT_STR];

  /* Filter invalid PORT arguments by comparing the length of the argument. Too
   * many or too little number of characters in the string means that the
   * argument is invalid. */
  if ((cmd_str_len = strlen (cmd_str)) < (MIN_PORT_STR_LEN - 1)) {
    fprintf (stderr, "%s: PORT argument too short\n", __FUNCTION__);
    send_mesg_501 (c_sfd);
    return -1;
  } else if (cmd_str_len > (MAX_PORT_STR_LEN - 1)) {
    fprintf (stderr, "%s: PORT argument too long\n", __FUNCTION__);
    send_mesg_501 (c_sfd);
    return -1;
  }    

  //Convert the address h1,h2,h3,h4,p1,p2 into a hostname and service.
  if (get_port_address (c_sfd, &hostname, &service, cmd_str) == -1) {
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
 * Convert the argument recieved with the PORT command to a hostname and
 * service string that can be used as arguments to getaddrinfo(). 
 *
 * The port command is entered as: PORT h1,h2,h3,h4,p1,p2 where h1-h4 are the
 * decimal values of each byte in the hostname and p1-p2 are the high and low
 * order bytes of the 16bit integer port.
 *
 * The argument 'cmd_str' is the command recieved by the server on the control
 * connection. The command string must be passed to this function in its
 * entirety.
 *
 * Arguments:
 *      c_sfd - The socket file descriptor for the control connection.
 *   hostname - The hostname string, passed as a pointer to this function, will
 *              be set to the IPv4 dot notation hostname on function return.
 *   service  - The service string, passed as a pointer to this function, will
 *              be set to the port integer value expressed as a string on
 *              function return.
 *    cmd_str - The string of the port command. "PORT h1,h2,h3,h4,p1,p2\n"
 *
 * Return values:
 *   0    The hostname and service strings have been successfuly set.
 *  -1    Error, hostname and service strings are not set.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int get_port_address (int c_sfd,
		      char (*hostname)[INET_ADDRSTRLEN], 
		      char (*service)[MAX_PORT_STR], 
		      char *cmd_str)
{
  /* Used to collect each byte of the hostname and port that was passed to the
   * function in the string argument 'cmd_str'.
   *
   * The bytes that will be collected from the command string and stored in the
   * elements of this array have been stored in a value that is larger than 8
   * bits. This has been done so that any byte value that is too large (an
   * invalid IPv4 address or port) can be found. */
  uint16_t h[PORT_BYTE_ARGS];    //h1,h2,h3,h4,p1,p2 see the function header.
  int h_index;

  int cmd_str_len;  //Length of the command string (function argument 3).
  uint16_t port;    //Stores the combined p1 and p2 value.
  int i;            //Loop counter.

  int char_counter; /* Used to ensure only one comma character is present 
		     * between each byte field in the command string. */
  
  //Initialize counters and compare values.
  cmd_str_len = strlen (cmd_str);
  char_counter = 0;
  h_index = 0;
  i = 0;

  /* Process all characters in the command string. This includes the command
   * portion "PORT " and the argument portion "h1,h2,h3,h4,p1,p2\n". */
  while (i < cmd_str_len) {
    /* Enter this block if the current character is not a digit. All characters
     * that are not digits (0-9) will be processed in this block, and the loop
     * will be restarted with the continue statement to process the next
     * character.  */
    if ((cmd_str[i] < 48) || (cmd_str[i] > 57)) {
      /* Only one comma may seperate a byte field in the argument. Keep track
       * of the number of number of characters between byte fields when in the
       * argument portion of the command string. */
      if (h_index > 0) {
	/* Only a comma may seperate a byte field. A newline is expected as the
	 * last character in the string, followed by a NULL character. */ 
	if ((cmd_str[i] != ',')&&(cmd_str[i] != '\n')&&(cmd_str[i] != '\0')) {
	  printf ("char = %c\ni = %d\n", cmd_str[i], i); 
	  fprintf (stderr, "%s: illegal character in argument\n", __FUNCTION__);
	  send_mesg_501 (c_sfd);
	  return -1;
	}
	char_counter++;
      }
      i++;
      continue;
    }
    //All characters that move past this point are digits.

    /* When an integer is found, store the integer. See acknowledgement ONE in
     * the file header for the meaning of "SCNu16". */
    if (sscanf (cmd_str + i, "%"SCNu16, &h[h_index]) == -1) {
      //Check for errors, EINTR is non-fatal so try sscanf again in that case.
      if (errno == EINTR) {
	i--; //So that the same character will be passed to sscanf()
	continue;
      }
      fprintf (stderr, "%s: sscanf: %s\n", __FUNCTION__, strerror (errno));
      return -1;
    }

    //Determine if the integer is too large to be stored in 1-byte.
    if (h[h_index] > MAX_8_BIT) {
      fprintf (stderr, "%s: invalid PORT argument, %"PRIu16" is larger than"
	       " a byte\n", __FUNCTION__, h[h_index]);
      send_mesg_501 (c_sfd);
      return -1;
    }

    //Determine the length of characters the integer is composed of.
    if (h[h_index] > 99) {
      i += 3;
    } else if (h[h_index] > 9) {
      i += 2;
    } else {
      i++;
    }

    //Store the next integer on the next pass.
    h_index++;
  }

  //Ensure that the correct number of integers were present in the string.
  if (h_index < (PORT_BYTE_ARGS - 1)) {
    fprintf (stderr, "%s: improper PORT argument\n", __FUNCTION__);
    send_mesg_501 (c_sfd);
    return -1;
  }

  //Store the hostname in IPv4 dot notation. See acknowledgement ONE.
  sprintf (*hostname, "%"PRIu16".%"PRIu16".%"PRIu16".%"PRIu16,
	   h[0], h[1], h[2], h[3]);

  /* Multiply the value of the high order port byte by 256, to shift this byte
   * into the correct position. This essentially shifts the value by 8 bits
   * for both big and little endian systems. */
  h[4] = (h[4] * 256);
  //Combine the two port bytes to create one integer.
  port = (h[4] | h[5]);
  //Store the integer as a string. See acknowledgement ONE in the file header.
  sprintf (*service, "%"PRIu16, port);

  //The hostname and service have been set, return from the function.
  return 0;
}
  

/******************************************************************************
 * Connect to the address and port specified in the arguments recieved with the
 * PORT command.
 *
 * Arguments:
 *   hostname - The IPv4 address to connect to, represented in a dot notation
 *              string. (eg. "xxx.2.0.13")
 *    service - The service (port) to connect to, represented as a string.
 *              (eg. "56035")
 *
 * Return values:
 *   >0    The socket file descriptor of the newly created data connection.
 *   -1    Error, the connection could not be made.
 *
 * Original author: Evan Myers
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
    fprintf (stderr, "%s: getaddrinfo: %s\n", __FUNCTION__, gai_strerror(gai));
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
 * send_all - see net.h
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
