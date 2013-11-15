/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: response.h
 * Date: November 2013
 *
 * Description:
 *    The functions found in this file send a response message to the client
 *    over the control connection socket. They were created to remove
 *    repitition in places where a response message is sent. Also, this will
 *    help ensure that all messages that are meant to send the same data will
 *    do so.
 *****************************************************************************/
#ifndef __RESPONSE_H__
#define __RESPONSE_H__

/******************************************************************************
 * This function generates a posotive response message for the PASV command
 * as specified in 'rfc 959'. The code of this message is 227.
 *
 * Create the PASV command response message, then send the feedback to the
 * client on the control connection. The response will be sent in the following
 * format: "Entering Passive Mode (h1,h2,h3,h4,p1,p2).\n".
 *
 * NOTE: The response message is sent to the control connection socket (c_sfd),
 *       and this function generates this message using the data creation
 *       socket (d_sfd). Both are required for this function.
 *
 * Arguments:
 *   c_sfd - The file descriptor of the control connection socket. The response
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
int send_mesg_227 (int c_sfd, int d_sfd);


/******************************************************************************
 * This function generates a negative response message for when the command
 * was not recognized. The code of this message is 500.
 *
 * Arguments:
 *   c_sfd - The file descriptor of the control connection socket. The response
 *           message will be sent to this socket.
 *
 * Return values:
 *   0    The message was successfuly sent to the socket.
 *  -1    The message was not sent to the socket.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int send_mesg_500 (int c_sfd);


/******************************************************************************
 * This function generates a negative response message for when errors have
 * been detected in the argument of a command. The code of this message is 501.
 *
 * Arguments:
 *   c_sfd - The file descriptor of the control connection socket. The response
 *           message will be sent to this socket.
 *
 * Return values:
 *   0    The message was successfuly sent to the socket.
 *  -1    The message was not sent to the socket.
 *
 * Original author: Evan Myers
 *****************************************************************************/
int send_mesg_501 (int c_sfd);


#endif //__RESPONSE_H__
