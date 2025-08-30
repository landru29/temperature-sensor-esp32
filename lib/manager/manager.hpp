#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "command.hpp"
#ifndef TESTING
#include "Print.h"
#else
class Print {
    public:
        void print(const char* data) {
            // Do nothing
        }
        void println(const char* data) {
            // Do nothing
        }
};
#endif

typedef void (*CommandFunction)(Command* cmd);

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
        CommandManager(Print *writer);
        void setCommand(Command *cmd);
        void displayHelp();
        void on(const char* command, const char* description, CommandFunction callback);
    
    private:
        CommandHandler *commandHandler;
        Print *writer;
        int maxCommandSize;
};

#endif