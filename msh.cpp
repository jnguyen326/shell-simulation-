
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

// The shell's prompt string
const std::string PROMPT = "cssc1451% ";

// This function splits a string by whitespace and returns the parts
std::vector<std::string> getArguments(const std::string& userInput) {
    std::vector<std::string> arguments;
    std::istringstream iss(userInput);
    for (std::string s; iss >> s;) {
        arguments.push_back(s);
    }
    return arguments;
}

// This is called when execvp fails
void handleExecutionError(const std::string& command) {
    std::cerr << "Execution failed for command: " << command << "\n";
    exit(EXIT_FAILURE);
}

// This executes a single command without a pipeline
void executeCommand(const std::vector<std::string>& args) {
    std::vector<char*> argv;
    for (const auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.data()));
    }
    argv.push_back(nullptr);

    if (access(argv[0], F_OK) != 0) {
        std::cerr << "File does not exist: " << argv[0] << "\n";
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed\n";
        return;
    }
    if (pid == 0) { // child
        if (execvp(argv[0], argv.data()) < 0) {
            handleExecutionError(args[0]);
        }
    } else { // parent
        wait(NULL);
    }
}

// Function to execute a pipeline of commands
void executePipeline(const std::vector<std::string>& commands) {
    // Get the number of commands
    size_t numCommands = commands.size();

    // Initialize a vector of arguments for each command
    std::vector<std::vector<std::string>> argsList(numCommands);

    // Initialize a vector of argv for each command
    std::vector<std::vector<char*>> argvList(numCommands);

    // Initialize a variable to store the read end of the previous pipe
    int prevPipeRead = -1;

    // Loop over all commands
    for (size_t i = 0; i < numCommands; ++i) {
        
        // Get the command
        const std::string& command = commands[i];

        // Split the command into arguments
        argsList[i] = getArguments(command);

        // Convert the arguments into argv
        std::vector<char*>& argv = argvList[i];
        for (const auto& arg : argsList[i]) {
            argv.push_back(const_cast<char*>(arg.data()));
        }
        
        argv.push_back(nullptr); // argv must be null-terminated

        // If this is not the last command, create a pipe
        int pipefd[2];
        if (i < numCommands - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // Create a new process
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { // child process
            // If this is not the first command, set the standard input to the read end of the previous pipe
            if (prevPipeRead != -1) {
                if (dup2(prevPipeRead, STDIN_FILENO) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(prevPipeRead); // close the original file descriptor as it's no longer needed
            }

            // If this is not the last command, set the standard output to the write end of the current pipe
            if (i < numCommands - 1) {
                if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(pipefd[1]); // close the original file descriptor as it's no longer needed
                close(pipefd[0]); // close the read end of the pipe as it's not used in this process
            }

            // Replace the current process with the command to be executed
            if (execvp(argv[0], argv.data()) < 0) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            
        } else { // parent
            if (prevPipeRead != -1) {
                close(prevPipeRead);
            }

            // If not the last command, set up the read end of the pipe for the next iteration
            if (i < numCommands - 1) {
                prevPipeRead = pipefd[0];
                close(pipefd[1]); // close write end of the pipe in the parent
            }
        }
    }

    // Wait for all children to finish
    while (wait(NULL) > 0);
}

// This function is called every time the user enters a command
void handleUserInput(const std::string& userInput) {
    if (userInput.find('|') != std::string::npos) {
        // If the command contains a pipe, split it into individual commands
        std::vector<std::string> commands;
        std::istringstream iss(userInput);
        for (std::string command; std::getline(iss, command, '|');) {
            commands.push_back(command);
        }

        // And execute them as a pipeline
        executePipeline(commands);
    } else {
        // If no pipe is found, execute it as a single command
        auto arguments = getArguments(userInput);
        if (arguments.empty()) {
            return;
        }

        const std::string& command = arguments[0];

        if (command == "exit") {
            // If the command is "exit", terminate the shell
            exit(EXIT_SUCCESS);
        } else {
            // Otherwise, execute the command
            executeCommand(arguments);
        }
    }
}

int main() {
    while (true) {
        // Display the prompt and get a line of input from the user
        std::cout << PROMPT;
        std::string userInput;
        std::getline(std::cin, userInput);

        // If the input is empty, display an error message and continue to the next iteration
        if (userInput.empty()) {
            std::cout << "Invalid input format\n";
            continue;
        }

        // Handle the user's input
        handleUserInput(userInput);
    }

    return 0;
}
       
