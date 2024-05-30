/**
  * author: Alaa Abdelmoneam
  * date  : 3/7/2024
  * brief : implementation of simple kernel functions
**/

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
void setup_environment();
char tokenize();
void on_child_exit(int sig);
void shell();
void execute_command(char num_of_tokens);
void execute_shell_builtin(char num_of_tokens);
void dollar_sign_handler();
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
    
    // Set up the signal handler for SIGCHLD
    struct sigaction sa;
    sa.sa_handler = &on_child_exit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, 0) == -1) {
        perror("sigaction");
        exit(1);
    }

    shell();
    return 0;
}

/* function definitions */
void on_child_exit(int sig)
{
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        fptr = fopen("logs.txt", "a");
        if (fptr == NULL)
        {
            printf("Couldn't open log file.\n");
            return;
        }
        fprintf(fptr, "Child process terminated\n");
        fclose(fptr);
    }
    errno = saved_errno;
}

void shell()
{
    while (1)
    {
        printf("%s:~$", cwd);
        char num_of_tokens = tokenize();
        if (strcmp("cd", tokens[0]) == 0 || strcmp("echo", tokens[0]) == 0 || strcmp("export", tokens[0]) == 0)
        {
            execute_shell_builtin(num_of_tokens);
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
    if (child_pid == 0)
    {
        dollar_sign_handler();
        if (num_of_tokens == 1)
        {
            execlp(tokens[0], tokens[0], NULL);
        }
        else
        {
            char *args[MAX_TOKENS + 1] = {NULL};
            for (int i = 0; i < num_of_tokens; i++) {
                args[i] = tokens[i];
            }
            execvp(args[0], args);
        }
        perror("exec");
        exit(1);
    }
    else if (child_pid > 0)
    {
        pid = child_pid;
    }
    else
    {
        perror("fork");
    }
}

void execute_shell_builtin(char num_of_tokens)
{
    dollar_sign_handler();
    if (strcmp("cd", tokens[0]) == 0)
    {
        if (num_of_tokens == 1)
        {
            setup_environment();
        }
        else
        {
            chdir(tokens[1]);
        }
        getcwd(cwd, sizeof(cwd));
    }
    else if (strcmp("echo", tokens[0]) == 0)
    {
        for (int i = 1; i < num_of_tokens; i++)
        {
            printf("%s ", tokens[i]);
        }
        printf("\n");
    }
    else if (strcmp("export", tokens[0]) == 0)
    {
        char equal_sign_index;
        char variable_name[40];
        for (equal_sign_index = 0; equal_sign_index < strlen(tokens[1]); equal_sign_index++)
        {
            if (tokens[1][equal_sign_index] == '=')
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

void setup_environment()
{
    chdir("/home/alaa_abdelmoneam");
}

void dollar_sign_handler()
{
    for (int i = 0; i < MAX_TOKENS && tokens[i][0] != '\0'; i++)
    {
        if (tokens[i][0] == '$')
        {
            for (int j = 0; j < env_variable_number; j++)
            {
                if (strcmp(&tokens[i][1], variables_names[j]) == 0)
                {
                    strcpy(tokens[i], variables_values[j]);
                    break;
                }
            }
        }
    }
}
