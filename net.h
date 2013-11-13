/******************************************************************************
 * Student: Evan Myers
 * Course: CMPT-361
 * Assignment: Assignment #3 - ftp server
 * File: net.h
 * Date: November 2013
 *
 * Description: 
 ******************************************************************************/
#ifndef NET_H
#define NET_H

#include <stdint.h> //Required for 'uint8_t' in function prototype: send_all()
#include <arpa/inet.h>  //INETADDR_STRLEN

//The maximum number of characters a 16-bit integer can be converted to.
#define MAX_PORT_STR 6

/******************************************************************************
 * Create listening sockets on all available interfaces. The created sockets
 * will be used to establish a control connection with the client.
 *
 * Arguments: 
 *   sock  - Each listening socket will be set to an element of this array on 
 *           function return.
 *   nsock - Set the number of listening sockets on function return. 
 *
 * Return:  
 *   0   Success
 *  -1   Error
 *
 * Original author: Evan Myers
 *
 * Acknowledgements:
 *    -This function is similar to the function "serv_fb_setup" written by
 *     Evan Myers in assignment #2 file "net.c".
 *****************************************************************************/
int get_control_sock (int *sock, int *nsock);

/******************************************************************************
 * Accept a control connection. This function is TEMPORARY, it is only designed
 * to accept() ONE connection per call to this function. In the current form 
 * none of the socket file descriptors are closed. If we decide to keep this
 * function, all comments and code in this function should be carefuly checked
 * and re-written if necessary.
 *
 * We may wish to have the function add the accepted socket file descriptor to
 * a linked list.
 *
 * Not knowing how we will accept the connections fully I have created this
 * temporary design to be used as follows:
 * 
 * In function main():
 *    get_con_sock();
 *    while (TRUE) {
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
 * Original author (this function is temporary for now): Evan Myers
 *****************************************************************************/
int control_accept (int *sock, int nsock);

/******************************************************************************
 * Create a TCP data connection socket for the PASV server command. Then send
 * the address information of the newly created socket to the client over the
 * control connection. The socket that is created will be ready to accept() a
 * connection from the client to establish a data connection.
 *
 * Arguments:
 *   c_sfd - The control connection socket file descriptor.
 *
 * Return values:
 *  >0    The socket file descriptor of the data connection socket.
 *  -1    Error, the socket could not be created.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int cmd_pasv (int c_sfd);

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
int get_pasv_sock (void);

/******************************************************************************
 * Create the PASV command feedback message, then send the feedback to the
 * client on the control connection.
 *
 * Arguments:
 *   c_sfd - The file descriptor of the control connection socket. The feedback
 *           message will be sent to this socket.
 *   d_sfd - The file descriptor of the data connection socket. The address
 *           information sent to the control connection socket will be created
 *           with this socket.
 *
 * Return values:
 *    0   The message was successfuly sent to the socket.
 *   -1   The message was not sent to the socket.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int send_msg_227 (int c_sfd, int d_sfd);

/******************************************************************************
 * Connect a TCP socket to the address argument of the HOST command. This
 * will create a data connection to the client if successful.
 *
 * Arguments:
 *   c_sfd    - The control connection socket file descriptor.
 *   cmd_str  - The string of the port command. PORT h1,h2,h3,h4,p1,p2
 *
 * Return values:
 *   >0   The socket file descriptor of the data connection socket.
 *   -1   Error, the data connection could not be created.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int cmd_port (int c_sfd, char *cmd_str);

/******************************************************************************
 * Convert the argument recieved with the PORT command to a hostname and
 * service string that can be used as arguments to getaddrinfo(). 
 *
 * The port command is entered as: PORT h1,h2,h3,h4,p1,p2 where h1-h4 are the
 * decimal values of each byte in the hostname and p1-p2 are the high and low
 * order bytes of the 16bit integer port.
 *
 * The argument 'cmd_str' is the command string, it may include the command PORT
 * or just the values h1 through p2 that are seperated by comma's (','). The 
 * syntax of the PORT command arguments must be checked before passing the
 * command string to this function.
 *
 * Arguments:
 *   hostname - The hostname string, passed as a pointer to this function, will
 *              be set to the IPv4 dot notation hostname on function return.
 *   service  - The service string, passed as a pointer to this function, will
 *              be set to the port integer value expressed as a string on
 *              function return.
 *
 * Return values:
 *   0    The hostname and service strings have been successfuly set.
 *  -1    Error, hostname and service strings are not set.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int get_port_address (char (*hostname)[INET_ADDRSTRLEN],
		      char (*service)[MAX_PORT_STR],
		      char *cmd_str);

/******************************************************************************
 * port_connect
 *****************************************************************************/
int port_connect (char *hostname, char *service);

/******************************************************************************
 * Send the entire message found in the second argument to the socket passed
 * in the first argument. This function was created to handle partial sends.
 *
 * Arguments:
 *   sfd - The socket file descriptor to send the message to.
 *
 * Return values:
 *   0    The full message was successfuly sent.
 *  -1    Error, the message was not sent in full.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int send_all (int sfd, uint8_t *mesg, int mesg_len);

#endif /* NET_H */
