#include "manager.hpp"
#include "string.h"


CommandHandler::CommandHandler(const char* command, const char* description, CommandFunction callback) {
    this->trigger = command;
    this->description = description;
    this->function = callback;
    this->next = NULL;
}

CommandManager::CommandManager(OutputWriter writer) {
    this->writer = writer;
    this->maxCommandSize=4;
}

void CommandManager::on(const char* command, const char* description, CommandFunction callback) {
    if (strlen(command)>this->maxCommandSize) {
        this->maxCommandSize = strlen(command);
    }
    
    if (this->commandHandler == NULL) {
        this->commandHandler = new CommandHandler(command, description, callback);

        return;
    }

    CommandHandler* currentHandler = this->commandHandler;

    for (;currentHandler->next != NULL; currentHandler=currentHandler->next);
    currentHandler->next = new CommandHandler(command, description, callback);
}

void CommandManager::displayHelp() {
    this->writer("help");
    for(int i=4; i<this->maxCommandSize; i++) {
        this->writer(" ");
    }
    this->writer(" : Display this help\n");

    for (CommandHandler* currentHandler = this->commandHandler;currentHandler != NULL; currentHandler=currentHandler->next) {
        this->writer(currentHandler->trigger);
        for(int i=strlen(currentHandler->trigger); i<this->maxCommandSize; i++) {
            this->writer(" ");
        }
        this->writer(" : ");
        this->writer(currentHandler->description);
        this->writer("\n");
    }
    this->writer("READY\n");
}

void CommandManager::setCommand(Command *cmd) {
    char* command = cmd->getCommand();

    if (strcasecmp("help", command)==0) {
        this->displayHelp();
        return;
    }

    for (CommandHandler* currentHandler = this->commandHandler;currentHandler != NULL; currentHandler=currentHandler->next) {
        if (strcasecmp(currentHandler->trigger, command)==0) {
            (currentHandler->function)(cmd);
            this->writer("READY\n");
            return;
        }

        if (currentHandler->next == NULL) {
            this->writer("Command not found: [");
            this->writer(command);
            this->writer("]\nType 'help' for the full command list.\n");
            this->writer("READY\n");
        }
    }
}