/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: config.h
 * Date: November 2013
 *
 * Description:
 *   Functions that operate on the server configuration file, ftp.conf, are
 *   found in this file.
 *
 * Acknowledgements:
 *   Evan - The decision to include the configuration file was made after a
 *          conversation with Dr. Nicholas Boers.
 *****************************************************************************/


//The name of the server configuration file.
#define FTP_CONFIG_FILE "ftp.conf"
//The name of the user configuration file.
#define USER_CONFIG_FILE "user.conf"


/******************************************************************************
 * This function will open the configuration file, search for the setting
 * passed in the first argument, and return the value that has been set for
 * this target setting. 
 *
 * The function caller must be sure to free the string returned by this
 * function (the returned string was made using malloc).
 *
 * Arguments:
 *   config_setting - Retrieve the value of this setting from the config file.
 *
 * Returns:
 *   Return a pointer to the string of all characters found after the target
 *   setting in the config file.
 *
 *   This string begins after the space character found after the target
 *   setting. The string ends at the newline character. This string is
 *   null-terminated.
 *
 *   If NULL is returned, there was an error. View the standard error log for
 *   the cause of this error.
 *
 * Original author: Evan Myers
 *****************************************************************************/
char *get_config_value (const char *config_setting, const char *filen);

