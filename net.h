/******************************************************************************
 * Student: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment: Assignment #3 - ftp server
 * File: net.h
 * Date: November 2013
 *
 * Description:
 *   The functions found in this file create, connect, or accept sockets or
 *   socket connections. Functions that send or recieve data from a socket are
 *   also found in this file. The PASV and PORT commands, which create and/or
 *   connect a socket to establish a data connection are found in this file.
 ******************************************************************************/
#ifndef __NET_H__
#define __NET_H__


#include <stdint.h>    //Required for 'uint8_t' in function prototype
#include <arpa/inet.h> //required for the INETADDR_STRLEN in function prototype


//The maximum number of characters a 16-bit integer can be converted to.
#define MAX_PORT_STR 6

/* The maximum number of sockets that will be listened to when establishing
 * control connections with any client. */
#define MAX_SOCK 100

#define BITS_IN_BYTE 8  //The number of bits in a byte.

/******************************************************************************
 * Create listening sockets on all available interfaces. The created sockets
 * will be used to establish a control connection with the client. The sockets
 * created are not closed on function return.
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
 *     Evan Myers in his assignment #2 file "net.c".
 *****************************************************************************/
int get_control_sock (int *sock, int *nsock);


/******************************************************************************
 * Establish a control connection from the client on any available interface.
 * Read standard input from the server console for commands such as "shutdown"
 * or "help".
 *
 * Arguments:
 *   sock  - An array of listening sockets ready to accept connections.
 *   nsock - The number of listening sockets that is found in the array passed
 *           in the first argument.
 *
 * Return values:
 *   >0   The socket file descriptor of the newly created control connection.
 *   -1   Error, the connection could not be established with the client.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int control_accept (int *sock, int nsock);


/******************************************************************************
 * Create a TCP data connection socket for the PASV server command. 
 *
 * The interface that this socket is created on will be the value which is set
 * for the INTERFACE_CONFIG setting in the configuration file.
 *
 * Send the address information of the newly created socket to the client over
 * the control connection. The socket that is created will be ready to accept()
 * a connection from the client to establish a data connection.
 *
 * Arguments:
 *   c_sfd - The control connection socket file descriptor.
 *   cmd_str  - The string of the pasv command. "PASV\n"
 *
 * Return values:
 *  >0    The socket file descriptor of the data connection socket.
 *  -1    Error, the socket could not be created.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int cmd_pasv (int c_sfd, char *cmd_str);


/******************************************************************************
 * Find the address of an interface.
 *
 * Set the string pointer passed as argument two of this function to the dot
 * notation (eg. 127.0.1.1) IPv4 address of the interface passed as the first
 * argument to this function.
 *
 * Arguments:
 *   interface - Retrieve the address of this interface.
 *   address   - A string pointer to be set to the retrieved interface address
 *               on function return.
 *
 * Return values:
 *   0    Success, the address was found and the value set in the second
 *        argument.
 *  -1    Error, the address has not been set in the second argument. 
 *
 * Original author: Evan Myers
 *
 * Acknowledgements:
 *   This function was created as the result of advice recieved from
 *   Dr. Nicholas Boers to retrieve the external IP address of a computer. In
 *   this advice, the link which follows was suggested. When creating this
 *   function, I was following the example code found in this link:
 *   
 *   http://stackoverflow.com/a/265978
 *****************************************************************************/
int get_interface_address (const char *interface,
			   char (*address)[INET_ADDRSTRLEN]);


/******************************************************************************
 * Connect a TCP socket to the address argument of the PORT command. This
 * will create a data connection to the client if successful.
 *
 * Arguments:
 *   c_sfd    - The control connection socket file descriptor.
 *   cmd_str  - The string of the port command. "PORT h1,h2,h3,h4,p1,p2\n"
 *
 * Return values:
 *   >0   The socket file descriptor of the data connection socket.
 *   -1   Error, the data connection could not be created.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int cmd_port (int c_sfd, char *cmd_str);


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

#endif //__NET_H__
