//Libraries
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>


//Constants for program
#define MAX_COMMAND_SIZE 100
#define MAX_COMMAND_STRINGS 10

//Functions
//Builtin function to handle builtin commands which are not present in /usr/bin i.e. commands which are integreted in the system shell
void builtIN(char *command[])
{
    if(strcmp(command[0],"cd") == 0)
        {
            if(chdir(command[1]) == -1)
            {
                fprintf(stderr,"Error in cd: %s\n",strerror(errno));
                return;
            }
            return;
        }

    else if(strcmp(command[0],"help") == 0)
            {
                printf("This is custom shell built using c language. Find below the instruction how to use the shell and commands.\n 1.ls: To list the files in current directory \n 2.pwd: to get the current working directory \n 3.exit: to exit the shell \n 4.cd: to change the directory \n 5.echo: to print on terminal \n 6.vim: to edit files \n7.top: to show running processes \n Apart From This, You can also use input/output redirection command using > and <\n");
                return;
            }        
      
}

//Handling Input and Output Redirection
int formatRedirection(char *command[],int *input_file_pos,int *output_file_pos,char **input_file,char **output_file)
{       
        int count_redirection = 0;
        while(command[count_redirection] != NULL)
        {
            if(strcmp(command[count_redirection],"<") == 0)
            {
                *input_file_pos = count_redirection;
                *input_file = command[count_redirection+1];
                if(*input_file == NULL)
                {
                    printf("Enter Valid Command\n");
                    return 1;
                }
                
            }
            if(strcmp(command[count_redirection],">") == 0)
            {
               *output_file_pos = count_redirection;
               *output_file = command[count_redirection+1];
               if(*output_file == NULL)
                {
                    printf("Enter Valid Command\n");
                    return 1;
                }
            }
            count_redirection++;
        }
        return 0;
}

//Handles forking and execution of commands
void commandExecution(char *command[],int input_file_pos,int output_file_pos,char *input_file,char *output_file)
{
            pid_t pid = fork();
            if(pid<0)
            {
                perror("fork failed!\n");
                exit(1);
            }
            if(pid == 0)
            {
                if(input_file != NULL)
                {
                    int fd_input = open(input_file,O_RDONLY | S_IRUSR);
                    if(fd_input<0)
                    {
                        fprintf(stderr,"Error Opening File:%s \n",strerror(errno));
                        exit(1);
                    }
                    dup2(fd_input,STDIN_FILENO);
                    close(fd_input);
                }

                if(output_file != NULL)
                {
                    int fd_output = open(output_file,O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
                    if(fd_output<0)
                    {
                        fprintf(stderr,"Error Opening File:%s \n",strerror(errno));
                        exit(1);
                    }
                    dup2(fd_output,STDOUT_FILENO);
                    close(fd_output);
                }

                command[input_file_pos] = NULL;
                command[output_file_pos] = NULL;

                if(execvp(command[0],command) == -1)
                {
                    fprintf(stderr,"Invalid command Entered: %s \n",strerror(errno));
                    exit(1);
                }
            }
            if(pid>0)
            {
                waitpid(pid,NULL,0);
            }    
}

//Parsing input to get each string from terminal
int parseInput(char buf[],char *command[])
{
    char *token = strtok(buf," ");
    int itr = 0;
    while(token!=NULL && itr<MAX_COMMAND_SIZE)
    {

        if(token[0] == '"')
        {
            char temp[MAX_COMMAND_SIZE];
            strcpy(temp,token+1);

            while(token[strlen(token)-1]!= '"' && (token = strtok(NULL," "))!=NULL)
            {
                strcat(temp," ");
                strcat(temp,token);
            }

            if(token!=NULL && token[strlen(token)-1]=='"')
            {
                temp[strlen(temp)-1] = '\0';
            }
            else{
                printf("Error in Command\n");
                return itr;
            }

            command[itr] = malloc(strlen(temp)+1);
            if(command[itr] == NULL)
            {
                printf("Error while allocating Memory\n");
                return itr;
            }
            strcpy(command[itr],temp);
        }
       else{
         command[itr] = malloc(strlen(token)+1);
        if(command[itr] == NULL)
        {
            printf("Error while allocating Memory\n");
            return itr;
        }
        strcpy(command[itr],token);
       }
        itr++;
        token = strtok(NULL," ");
    }
    command[itr] = NULL;
    return itr;
}

//to flush the memory
void flushCommand(char *command[], int noOfArgs)
{
    for(int i=0;i<noOfArgs;i++)
        {
            free(command[i]);
        }   
}

//implementation of shell
void startShell()
{
    char buf[MAX_COMMAND_SIZE];
    char *command[MAX_COMMAND_SIZE];
    int noOfArgs = 0;
    int io_redirection_errorcheck;
    while(1)
    {
        printf("Welcome To noobshell!\n");
        printf("noobshell~/:=");
        if(fgets(buf,MAX_COMMAND_SIZE,stdin) == NULL)
        {
            printf("\tError While Reading the Input");
            continue;
        }
        if (buf[0]=='\n') {
            continue;
        }
        buf[strcspn(buf, "\n")] = 0;
        if(strcmp(buf, "exit") == 0) {
            printf("Bye!\n");
            exit(0);  
        }
        noOfArgs = parseInput(buf,command);

        int count_redirection = 0;
        int input_file_pos = -1;
        int output_file_pos = -1;
        char *input_file = NULL;
        char *output_file = NULL;
        
        io_redirection_errorcheck = formatRedirection(command,&input_file_pos,&output_file_pos,&input_file,&output_file);

        if(io_redirection_errorcheck == 1)
        {
            flushCommand(command,noOfArgs);
            continue;
        }
        if(strcmp(command[0],"cd") == 0 || strcmp(command[0],"help") == 0)
        {
            builtIN(command);
            continue;
        }

        else
        {
            commandExecution(command,input_file_pos,output_file_pos,input_file,output_file);    
        }

        flushCommand(command,noOfArgs);
    }
}

int main()
{
    startShell();
    return 0;
}
