#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "command.hpp"

typedef void (*CommandFunction)(Command* cmd);
typedef void (*OutputWriter)(const char* data);

class CommandHandler {
    public:
        CommandHandler(const char* command, const char* description, CommandFunction callback);
        const char* trigger;
        const char* description;
        CommandFunction function;
        CommandHandler* next;
};

class CommandManager {
    public:
        CommandManager(OutputWriter writer);
        void setCommand(Command *cmd);
        void displayHelp();
        void on(const char* command, const char* description, CommandFunction callback);
    
    private:
        CommandHandler *commandHandler;
        OutputWriter writer;
        int maxCommandSize;
};

#endif