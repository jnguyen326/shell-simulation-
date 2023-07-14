/*Authors:
Yan Ho[RedID: 826654745]
Yan Ho Edoras: cssc1429
Jenny Nguyen[RedID: 826622622]
Jenny Nguyen Edoras: cssc1451

Course
- CS 480 Summer 2023 Section 01
- Programming Assignment #2 (a2)
*/

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
