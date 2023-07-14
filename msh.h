

#ifndef main_H
#define main_H

#include <string>
#include <vector>

// Function to parse input and get arguments.
std::vector<std::string> getArguments(const std::string& command);

// Function to handle execution error.
void handleExecutionError(const std::string& command);

// Function to handle 'cd' command.
void handleCdCommand(const std::vector<std::string>& args);

// Function to execute a command.
void executeCommand(const std::string& command);

// Function to execute a pipeline of commands.
void executePipeline(const std::vector<std::string>& commands);

#endif 
