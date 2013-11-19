/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment: Assignment #3 - ftp server
 * File: confmain.c
 * Date: November 2013
 *
 * Description: A program used to test the configuration file. This file must
 *              not be included in the final submission.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../config.h"

char *config_path;

int main (int argc, char *argv[])
{
  char *target = "INTERFACE_CONFIG";
  char *target2 = "DEFAULT_PORT_CONFIG";
  char *result;

  printf ("%s: %s: the setting=%s\n", argv[0], __FUNCTION__, target);
  //Set the configuration file pathname.
  if ((result = get_config_value ((const char *)target)) == NULL) {
    return -1;
  }

  printf ("%s: %s: the value=%s\n", argv[0], __FUNCTION__, result);

  free (result);
  result = NULL;

  printf ("%s: %s: ----->break<-----\n", argv[0], __FUNCTION__);
  printf ("%s: %s: the setting=%s\n", argv[0], __FUNCTION__, target2);
  //Set the default port.
  if ((result = get_config_value ((const char *)target2)) == NULL) {
    printf ("error caught\n");
  }

  printf ("%s: %s: the value=%s\n", argv[0], __FUNCTION__, result);
  free (result);

  return 0;
}
