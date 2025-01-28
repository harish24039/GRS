Custom Shell for Graduate Systems Assignment
Creator: Harish Hatmode
Email: harish24039@iiitd.ac.in

Overview
This is a custom shell developed as part of the Graduate Systems course at IIIT-Delhi. It provides basic shell functionality by leveraging various system calls in C, such as fork(), exec(), waitpid(), and others.

Implementation Details
The shell is implemented using the following system calls:

fork(): Creates a child process that is an exact copy of the parent process.
execvp(): Replaces the current process with a new process specified by the command.
waitpid(): Waits for a specific child process (identified by its PID) to finish execution.
dup2(): Duplicates file descriptors, often used for input/output redirection.
chdir(): Changes the current working directory of the shell.
open(): Opens a file and returns a file descriptor if successful.
close(): Closes a file descriptor.
Implemented Functions
builtIN():

Executes built-in commands such as cd (change directory) and help.
formatRedirection():

Handles input and output redirection using < and > operators.
commandExecution():

Creates a child process using fork() and executes the given command using execvp().
Handles file descriptors for input/output redirection.
parseInput():

Tokenizes the input string entered by the user using strtok() and stores the tokens in a command array for execution.
startShell():

Implements the core shell logic to continuously run the shell until the user exits.
Calls the above functions to manage different functionalities.
main():

The entry point of the program. Initializes and starts the shell.
Usage Details
This custom shell allows you to:

Execute system commands such as ls, pwd, ps, top, etc.
Change directories using the cd command.
Get help information using the help command.
Execute user-created processes.
Handle input/output redirection through < and > operators.
