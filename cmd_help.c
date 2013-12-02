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

  char *helpMsgEnd,
       *helpMsgStart;

  helpMsgStart = "\n214 - Help message.\n";
  helpMsgEnd = "214 - Help is OK.\n\n";

  if (arg == NULL) {

    char *row1,
         *row2,
         *row3,
         *row4,
         *row5,
         *row6,
         *row7,
         *row8;

    row1 = "The follwing commands may be abbreviated\nThe available commands are:\n";
    row2 = "\tABOR\tHELP\tPASV\tRMD\tSTOU\n";
    row3 = "\tACCT\tLIST\tPORT\tRNFR\tSTRU\n";
    row4 = "\tALLO\tMKD\tPWD\tRNTO\tSYST\n";
    row5 = "\tAPPE\tMODE\tQUIT\tSITE\tTYPE\n";
    row6 = "\tCDUP\tNLST\tREIN\tSMNT\tUSER\n";
    row7 = "\tCWD\tNOOP\tREST\tSTAT\n";
    row8 = "\tDELE\tPASS\tRETR\tSTOR\n";

    send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
    send_all(si->c_sfd, (uint8_t *)row1, strlen(row1));
    send_all(si->c_sfd, (uint8_t *)row2, strlen(row2));
    send_all(si->c_sfd, (uint8_t *)row3, strlen(row3));
    send_all(si->c_sfd, (uint8_t *)row4, strlen(row4));
    send_all(si->c_sfd, (uint8_t *)row5, strlen(row5));
    send_all(si->c_sfd, (uint8_t *)row6, strlen(row6));
    send_all(si->c_sfd, (uint8_t *)row7, strlen(row7));
    send_all(si->c_sfd, (uint8_t *)row8, strlen(row8));
    send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

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

	strLength = sprintf(cmdLine, "To use: %s <SP> <username> <CRLF>\n", arg);
	cmdInfo = "\tSend new user information\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* PASS <SP> <password> <CRLF> */
      } else if (strcmp(arg, "PASS") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <password> <CRLF>\n", arg);
	cmdInfo = "\tSend a user password\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* QUIT <CRLF> */
      } else if (strcmp(arg, "QUIT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tTerminate the FTP session and exit\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* PORT <SP> <host-port> <CRLF> */
      } else if (strcmp(arg, "PORT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <host-port> <CRLF>\n", arg);
	cmdInfo = "\tOpen a specified data port\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* PASV <CRLF> */
      } else if (strcmp(arg, "PASV") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tEnter passive transfer mode\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* TYPE <SP> <type-code> <CRLF> */
      } else if (strcmp(arg, "TYPE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <type-code> <CRLF>\n", arg);
	cmdInfo = "\tSet file transfer type\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* STRU <SP> <structure-code> <CRLF> */
      } else if (strcmp(arg, "STRU") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <structure-code> <CRLF>\n", arg);
	cmdInfo = "\tSet file transfer structure\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* MODE <SP> <mode-code> <CRLF> */
      } else if (strcmp(arg, "MODE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <mode-code> <CRLF>\n", arg);
	cmdInfo = "\tSet file transfer mode\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* RETR <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "RETR") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tRetrieve a file from the remote host\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* STOR <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "STOR") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tStore a file on the remote host\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* STOU <CRLF> */
      } else if (strcmp(arg, "STOU") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tStore a file uniquely\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* APPE <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "APPE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tAppend to a remote file\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* ABOR <CRLF> */
      } else if (strcmp(arg, "ABOR") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tAbort a file transfer\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* LIST [<SP> <pathname>] <CRLF> */
      } else if (strcmp(arg, "LIST") == 0) {

	strLength = sprintf(cmdLine, "To use: %s [<SP> <pathname>] <CRLF>\n", arg);
	cmdInfo = "\tList contents of remote path\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* NLIST [<SP> <pathname>] <CRLF> */
      } else if (strcmp(arg, "NLST") == 0) {

	strLength = sprintf(cmdLine, "To use: %s [<SP> <pathname>] <CRLF>\n", arg);
	cmdInfo = "\tName list of remote directory\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* SYST <CRLF> */
      } else if (strcmp(arg, "SYST") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tReturn remote system type\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* HELP [<SP> <string>] <CRLF> */
      } else if (strcmp(arg, "HELP") == 0) {

	strLength = sprintf(cmdLine, "To use: %s [<SP> <string>] <CRLF>\n", arg);
	cmdInfo = "\tPrint local help information on using the server\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* NOOP <CRLF> */
      } else if (strcmp(arg, "NOOP") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tDo nothing; server sends an OK reply\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* ACCT <SP> <account-information> <CRLF> */
      } else if (strcmp(arg, "ACCT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <account-information> <CRLF>\n", arg);
	cmdInfo = "\tSend account command to the remote server\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* CDUP <CRLF> */
      } else if (strcmp(arg, "CDUP") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tChange remote working directory to parent directory\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* SMNT <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "SMNT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tMount a different file system data structure without altering their login or accounting information\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* REIN <CRLF> */
      } else if (strcmp(arg, "REIN") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tReinitialize the connection\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* ALLO <SP> <decimal-integer> [<SP> R <SP> <decimal-integer>] <CRLF> */
      } else if (strcmp(arg, "ALLO") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <decimal-integer> [<SP> R <SP> <decimal-integer>] <CRLF>\n", arg);
	cmdInfo = "\tAllocate sufficient storage space to receive a file\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* REST <SP> <marker> <CRLF> */
      } else if (strcmp(arg, "REST") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <marker> <CRLF>\n", arg);
	cmdInfo = "\tRestart transfer at bytecount marker\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* RNFR <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "RNFR") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tUsed when renaming a file; rename from\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* RNTO <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "RNTO") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tUsed when renaming a file; rename to\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* DELE <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "DELE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tDelete a remote file\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* SITE <SP> <string> <CRLF> */
      } else if (strcmp(arg, "SITE") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <string> <CRLF>\n", arg);
	cmdInfo = "\tSend site specific command to the remote server\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* STAT [<SP> <pathname>] <CRLF> */
      } else if (strcmp(arg, "STAT") == 0) {

	strLength = sprintf(cmdLine, "To use: %s [<SP> <pathname>] <CRLF>\n", arg);
	cmdInfo = "\tReturn current server status\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

      } else {

	send_all(si->c_sfd, (uint8_t *)argUnrecognized, strlen(argUnrecognized));

      } //END statement 'if-else'

    } else if (strlen(arg) == MIN_CMD_SIZE) {

      /* CWD <SP> <pathname> <CRLF> */
      if (strcmp(arg, "CWD") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tChange the current working directory on the remote machine\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* RMD <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "RMD") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tRemove a directory on the remote machine\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* MKD <SP> <pathname> <CRLF> */
      } else if (strcmp(arg, "MKD") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <SP> <pathname> <CRLF>\n", arg);
	cmdInfo = "\tCreate a directory on the remote machine\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

	/* PWD <CRLF> */
      } else if (strcmp(arg, "PWD") == 0) {

	strLength = sprintf(cmdLine, "To use: %s <CRLF>\n", arg);
	cmdInfo = "\tPrint the current working directory on the remote machine\n";

	send_all(si->c_sfd, (uint8_t *)helpMsgStart, strlen(helpMsgStart));
	send_all(si->c_sfd, (uint8_t *)cmdLine, strLength);
	send_all(si->c_sfd, (uint8_t *)cmdInfo, strlen(cmdInfo));
	send_all(si->c_sfd, (uint8_t *)helpMsgEnd, strlen(helpMsgEnd));

      } else {

	send_all(si->c_sfd, (uint8_t *)argUnrecognized, strlen(argUnrecognized));

      } //END statement 'if-else'

    } else {

      send_all(si->c_sfd, (uint8_t *)argUnrecognized, strlen(argUnrecognized));

    } //END statement 'if-else'

    free(cmdLine);

  } //END statement 'if-else'

} //END function 'command_help'
