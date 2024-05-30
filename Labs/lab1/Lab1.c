/** 
  * author: Alaa Abdelmoneam
  * date  : 3/7/2024
  * breif : implementation of simple kernel functions
/**

/* Includes Section Start */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>

/* Includes Section End */

/* Definition Section Start*/

#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 100

/* Definition Section End*/

/* functions & variable declarations Start*/
setup_environment();
char tokenize();
on_child_exit(int pid);
shell();
void execute_command(char num_of_tokens);
void execute_shell_bultin(char num_of_tokens);
char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH] = {};
int pid = 0; // dummy
char command[100];
char cwd[100] = "/home/alaa_abdelmoneam";
char *token;
char env_variable_number = 0;
char variables_names[20][40] = {};
char variables_values[20][40] = {};
FILE *fptr;

/* functions & variable declarations End*/

/*main Section*/
