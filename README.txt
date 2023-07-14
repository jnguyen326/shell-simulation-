/*Authors:
Yan Ho[RedID: 826654745]
Yan Ho Edoras: cssc1429
Jenny Nguyen[RedID: 826622622]
Jenny Nguyen Edoras: cssc1451

Course
- CS 480 Summer 2023 Section 01
- Programming Assignment #2 (a2)
*/

README.txt

Files:
    - README.txt
    - msh.cpp
    - Makefile
    - msh.h
    - 3 sample exe files for testing
    
Code Summary:
Our program, named "msh", is a simplified shell implementation in C++. It allows users to enter commands, creates a child process to execute the command, and returns control to the shell prompt after the command execution is finished. The shell also supports pipelines, allowing users to connect the output of one command to the input of another.

Compile Instructions:
    - The command to compile the program in terminal: 
        - g++ msh.cpp -o msh -std=c++11
            - This command will create an executable named `msh`
        - Alternatively, you can use the provided Makefile for compilation. Run 'make' in the terminal.

Operating Instructions:
    - After compiling, you can run the program by typing `./msh`.
    - The shell will display a prompt "cssc1451% " and wait for the user's input.
    - You can execute a single command by typing it after the prompt, e.g., "cssc1451% ls".
    - You can create a pipeline by connecting commands with the pipe symbol, e.g., "cssc1451% ls | sort".
    - It should work for commands like ls -la, ls -la | sort -fi, ls -la | sort -fi | grep .bashrc, and /usr/bin/echo printme, etc
    - You can run other executable files by running executablefilename | executablefilename. There are3 sample files to test this, they are called hello, 	greeting, and compliment
    - To exit the shell, type "exit" after the prompt.

Code Details:
    - Our shell implementation uses UNIX system calls such as fork(), execvp(), and pipe() to create child processes and pipelines.
    - We use the fork() function to create a new child process to execute the user's command, and the execvp() function to replace the current process image with the new process image.
    - When executing a pipeline, the shell creates a new process for each command, and uses the pipe() function to connect the output of one process to the input of the next.
    - The executePipeline function is responsible for executing a series of commands separated by pipes '|'. The function first creates a child process for each command using fork(). For each child, it checks if the command is the first in the series or not, if it's not, it replaces the standard input with the read end of the pipe from the previous command. Then it checks if the command is the last in the series or not, if it's not, it replaces the standard output with the write end of the current pipe. It will try to execute the command using execvp(). The parent process closes the ends of the current pipe that it doesn't need and then waits for all its child processes to finish.
    - The executeCommand Function takes a vector of arguments (the command and its parameters), checks if the command file exists, and if so, creates a child process using fork(). The child process attempts to execute the command using execvp(), if this fails, handleExecutionError is called. The parent process waits for the child to finish execution using wait().

Known Issues:
    - As of now, we believe there are no known bugs in the program. If there are issues, we apologize in advance!

Lessons Learned:
    - We gained a deeper understanding of UNIX system calls and how they can be used to create child processes and handle inter-process communication.
    - This assignment gave us hands-on experience with pipe() and fork()
    - We learned how to create a program that outputs the username, waits for command, parses, and returns to wait for next command. 
    - We learned how to use the execvp() function to execute commands in a new process image.
    - We learned how to use one of the pipe() system calls to send output of one process to input of another.
    - We learned how to write a simple shell in C++ and handle the complexities of command parsing and pipeline execution.
    - We learned to create Makefiles for efficient compilation of our C++ programs.