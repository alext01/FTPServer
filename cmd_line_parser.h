//

#ifndef _CMD_PARSE_H_
#define _CMD_PARSE_H_

char *command_extract_cmd(const char *cmdString);
char *command_extract_arg(const char *cmdString);
void command_arg_parser();
int command_arg_count(const char *cmdString);
char *strdup(const char *string);
void convert_to_upper(char *string);
void trim_whitespace(char *string);

#endif /* _CMD_PARSE_H_  */
