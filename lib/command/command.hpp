#ifndef __command_hpp__
#define __command_hpp__ 

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS_LENGTH 300

class Command {
public:
    Command(const char *line);
    char* getCommand();
    char* nextArgument();
    bool hasArgument();
    int argumentCount();

private:
    char command[MAX_COMMAND_LENGTH];
    char arguments[MAX_ARGUMENTS_LENGTH];
    char currentArgument[MAX_COMMAND_LENGTH];
    int argumentIndex = 0;
};
#endif