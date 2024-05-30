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

int main()
{
    setup_environment();
    shell();
    signal(SIGCHLD,on_child_exit(pid));
}

/* function definitions */
on_child_exit(int pid)
{
    wait(NULL);
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL)
    {
        printf("Couldn't open log file.\n");
        return;
    }
    fprintf(fptr, "Child process terminated");
    fclose(fptr);
}
shell()
{
    while (1)
    {

        printf("%s:~$",cwd);
        char num_of_tokens = tokenize();
        if (strcmp("cd", command) == 0 || strcmp("echo", command) == 0 || strcmp("export", command) == 0)
        {
            execute_shell_bultin(num_of_tokens);
        }
        else
        {
            execute_command(num_of_tokens);
        }
    }

}


void execute_command(char num_of_tokens)
{

    pid_t child_pid = fork();
    char *args[] = {tokens[0], tokens[1], NULL};

    if (child_pid == 0)
    {
        dollar_sign_handler();
        if (1 == num_of_tokens){
            execvp(args[0], NULL);
        }
        else{
            execvp(args[0], args);
        }
        return 1;
    }
    else if (child_pid)
    {
        waitpid(child_pid,NULL,0);
        pid = child_pid;
    }
    return 1;
}


void execute_shell_bultin(char num_of_tokens)
{
    dollar_sign_handler();
    if (strcmp("cd", command) == 0)
    {
        if (num_of_tokens == 1){
            setup_environment();
            getcwd(cwd,100);
            //printf("CWD is %s\n",cwd);
            return;
        }

        {
            chdir(tokens[1]);
        }
        getcwd(cwd, 100);
        //printf("%s\n", cwd);
    }
    else if (strcmp("echo", command) == 0)
    {
        {
            printf("%s", tokens[1]);
        }
    }
    else if (strcmp("export", command) == 0)
    {

        char equal_sign_index;
        char variable_name[40];
        for (equal_sign_index = 0; equal_sign_index < strlen(tokens[1]); equal_sign_index++)
        {

            if ('=' == tokens[1][equal_sign_index])
            {
                variable_name[equal_sign_index] = '\0';
                break;
            }
            variable_name[equal_sign_index] = tokens[1][equal_sign_index];
        }
        strcpy(variables_names[env_variable_number], variable_name);
        strcpy(variables_values[env_variable_number], &(tokens[1][equal_sign_index + 1]));
        env_variable_number++;

    }
}
char tokenize()
{
    char num_tokens = 0;
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';
    char *token = strtok(command, " ");

    while (token != NULL && num_tokens < MAX_TOKENS)
    {
        strcpy(tokens[num_tokens], token); // Copy token to the array
        num_tokens++;
        token = strtok(NULL, " "); // Pass NULL to continue from previous

    }
    return num_tokens;

}

setup_environment(){
    chdir("/home/alaa_abdelmoneam");
}

void dollar_sign_handler(){
    for (int iterator = 0; (iterator < (sizeof(tokens) / sizeof(tokens[0]))) && tokens[iterator] != NULL; iterator++)
        {
            if (tokens[iterator][0] == '$')
            {
                for (int counter = 0; counter < (sizeof(variables_names) / sizeof(variables_names[0])); counter++)
                {
                    if (strcmp(&(tokens[iterator][1]), variables_names[counter]) == 0)
                    {
                        strcpy((tokens[iterator]), variables_values[counter]);
                    }
                }
            }
        }
}
