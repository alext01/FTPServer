//===============================================================================
//  Assignment #03 - FTP Server
//===============================================================================
//  Date:         November 2013
//  Course:       CMPT 361 - Introduction to Networks
//  Instructor:   Dr. Nicholas M. Boers
//  Students:     Evan Myers
//                Justin Slind
//                Alex Tai
//                James Yoo
//===============================================================================
//  Filename:
//    cmd_help.c
//===============================================================================
//  Associated Header File(s):
//    > cmd_help.h
//    > cmd_string_parser.h
//    > net.h
//    > session.h
//===============================================================================
//  Overview:
//    When HELP is invoked without an argument, it will retrun a list of all the
//    available FTP server commands that the client is able to use.
//    When HELP is invoked with an argument, it will return a short description
//    of the specified command and how it is to be used.
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.stackoverflow.com/
//===============================================================================




//C Library Reference(s)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Header File Reference(s)
#include "cmd_help.h"
#include "cmd_string_parser.h"
#include "net.h"
#include "session.h"


//Preprocessor Macro Define(s)
#define CMD_LINE_LENGTH 128
#define MAX_CMD_SIZE 4
#define MAX_NUM_ARGS 1
#define MIN_CMD_SIZE 3




//===============================================================================
//  Function Name:
//    command_help(session_info_t *si, char *arg)
//===============================================================================
//  Description:
//    Consult file "cmd_help.h"
//===============================================================================
//  Variable List (in alphabetic order):
//    {argUnrecognized}
//      > Type character pointer
//      > Contains the FTP server return code 501
//    {cmdInfo}
//      > Type character pointer
//      > Contains a short description pertaining to a specified command
//    {cmdLine}
//      > Type character pointer
//      > Contains information regarding how a specified command is used
//    {cmdUnimplemented}
//      > Type character pointer
//      > Contains the FTP server return code 502
//    {row#}
//      > Type character pointer
//      > Each row contains a list of available FTP commands
//    {strLength}
//      > Type integer
//      > Contains the length of the string 'cmdLine'
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

void command_help(session_info_t *si, char *arg)
{ //BEGIN function 'command_help'

  if (arg == NULL) {

    char *row1,
         *row2,
         *row3,
         *row4,
         *row5,
         *row6,
         *row7,
         *row8;

    row1 = "\nCommands may be abbreviated.\nThe commands are:\n";
    row2 = "\tABOR\tHELP\tPASV\tRMD\tSTOU\n";
    row3 = "\tACCT\tLIST\tPORT\tRNFR\tSTRU\n";
    row4 = "\tALLO\tMKD\tPWD\tRNTO\tSYST\n";
    row5 = "\tAPPE\tMODE\tQUIT\tSITE\tTYPE\n";
    row6 = "\tCDUP\tNLST\tREIN\tSMNT\tUSER\n";
    row7 = "\tCWD\tNOOP\tREST\tSTAT\n";
    row8 = "\tDELE\tPASS\tRETR\tSTOR\n\n";

    send_all(si->c_sfd, (uint8_t *)row1, strlen(row1));
    send_all(si->c_sfd, (uint8_t *)row2, strlen(row2));
    send_all(si->c_sfd, (uint8_t *)row3, strlen(row3));
    send_all(si->c_sfd, (uint8_t *)row4, strlen(row4));
    send_all(si->c_sfd, (uint8_t *)row5, strlen(row5));
    send_all(si->c_sfd, (uint8_t *)row6, strlen(row6));
    send_all(si->c_sfd, (uint8_t *)row7, strlen(row7));
    send_all(si->c_sfd, (uint8_t *)row8, strlen(row8));

    return;

  } else {

    int strLength;

    char *argUnrecognized,
         *cmdInfo,
         *cmdLine,
         *cmdUnimplemented;

    cmdLine = (char *)calloc(CMD_LINE_LENGTH, sizeof(char));
    cmdInfo = (char *)calloc(CMD_LINE_LENGTH, sizeof(char));

    argUnrecognized = "501 - Syntax error, command unrecognized.\n";
    cmdUnimplemented = "502 - Command is not currently implemented.\n";

    convert_to_upper(arg);

    if (strlen(arg) == MAX_CMD_SIZE) {

      /* USER <SP> <username> <CRLF> */
      if (strcmp(arg, "USER") == 0) {

	strLength = sprintf(cmdLine, "\nTo use: %s <SP> <username> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tSend new user information\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* PASS <SP> <password> <CRLF> */
      } else if (strcmp(arg, "PASS") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <password> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tSend a user password\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* QUIT <CRLF> */
      } else if (strcmp(arg, "QUIT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tTerminate the FTP session and exit\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* PORT <SP> <host-port> <CRLF> */
      } else if (strcmp(arg, "PORT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <host-port> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tOpen a specified data port\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* PASV <CRLF> */
      } else if (strcmp(arg, "PASV") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tEnter passive transfer mode\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* TYPE <SP> <type-code> <CRLF> */
      } else if (strcmp(arg, "TYPE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <type-code> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tSet file transfer type\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* STRU <SP> <structure-code> <CRLF> */
      } else if (strcmp(arg, "STRU") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <structure-code> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tSet file transfer structure\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* MODE <SP> <mode-code> <CRLF> */
      } else if (strcmp(arg, "MODE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <mode-code> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tSet file transfer mode\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* RETR <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "RETR") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tRetrieve a file from the remote host\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* STOR <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "STOR") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tStore a file on the remote host\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* STOU <CRLF> */
      } else if (strcmp(arg, "STOU") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tStore a file uniquely\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* APPE <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "APPE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tAppend to a remote file\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* ABOR <CRLF> */
      } else if (strcmp(arg, "ABOR") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tAbort a file transfer\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* LIST [<SP> <pathname>] <CRLF> */
      } else if (strcmp(arg, "LIST") == 0) {

	strLength = sprintf(cmdLine, "To use: %s [<SP> <pathname>] <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tList contents of remote path\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* NLIST [<SP> <pathname>] <CRLF> */
      } else if (strcmp(arg, "NLST") == 0) {

	strLength = sprintf(cmdLine, "To use: %s [<SP> <pathname>] <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tName list of remote directory\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* SYST <CRLF> */
      } else if (strcmp(arg, "SYST") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tReturn remote system type\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* HELP [<SP> <string>] <CRLF> */
      } else if (strcmp(arg, "HELP") == 0) {

	strLength = sprintf(cmdLine, "To use: %s [<SP> <string>] <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tPrint local help information on using the server\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* NOOP <CRLF> */
      } else if (strcmp(arg, "NOOP") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tDo nothing; server sends an OK reply\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* ACCT <SP> <account-information> <CRLF> */
      } else if (strcmp(arg, "ACCT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <account-information> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tSend account command to the remote server\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* CDUP <CRLF> */
      } else if (strcmp(arg, "CDUP") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tChange remote working directory to parent directory\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* SMNT <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "SMNT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tMount a different file system data structure without altering their login or accounting information\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* REIN <CRLF> */
      } else if (strcmp(arg, "REIN") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tReinitialize the connection\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* ALLO <SP> <decimal-integer> [<SP> R <SP> <decimal-integer>] <CRLF> */
      } else if (strcmp(arg, "ALLO") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <decimal-integer> [<SP> R <SP> <decimal-integer>] <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tAllocate sufficient storage space to receive a file\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* REST <SP> <marker> <CRLF> */
      } else if (strcmp(arg, "REST") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <marker> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tRestart transfer at bytecount marker\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* RNFR <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "RNFR") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tUsed when renaming a file; rename from\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* RNTO <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "RNTO") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tUsed when renaming a file; rename to\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* DELE <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "DELE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tDelete a remote file\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* SITE <SP> <string> <CRLF> */
      } else if (strcmp(arg, "SITE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <string> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tSend site specific command to the remote server\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* STAT [<SP> <pathname>] <CRLF> */
      } else if (strcmp(arg, "STAT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s [<SP> <pathname>] <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tReturn current server status\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

      } else {

	send_all(si->c_sfd, (uint8_t *)argUnrecognized, strlen(argUnrecognized));

      } //END statement 'if-else'

    } else if (strlen(arg) == MIN_CMD_SIZE) {

      /* CWD <SP> <pathname> <CRLF> */
      if (strcmp(arg, "CWD") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tChange the current working directory on the remote machine\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* RMD <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "RMD") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	//send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tRemove a directory on the remote machine\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* MKD <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "MKD") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tCreate a directory on the remote machine\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

	/* PWD <CRLF> */
      } else if (strcmp(arg, "PWD") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	cmdInfo = "\tPrint the current working directory on the remote machine\n\n";
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));

      } else {

	send_all(si->c_sfd, (uint8_t *)argUnrecognized, strlen(argUnrecognized));

      } //END statement 'if-else'

    } else {

      send_all(si->c_sfd, (uint8_t *)argUnrecognized, strlen(argUnrecognized));

    } //END statement 'if-else'

    free(cmdLine);

  } //END statement 'if-else'

} //END function 'command_help'
