#include "command.hpp"


Command::Command(const char *line) {
    int token = 0;

    // skip spaces
    if (line[token] == ' ') {
        for (; line[token] != 0 && line[token] == ' '; token++) {}
    }

    // find next space and copy command.
    int idx = 0;
    for (; line[token] != 0 && line[token] != ' ' && idx<MAX_COMMAND_LENGTH; token++) {
        this->command[idx++] = line[token];
    }
    this->command[idx] = 0;

    // copy arguments
    idx = 0;
    for (; line[token] != 0 && idx<MAX_ARGUMENTS_LENGTH; token++) {
        this->arguments[idx++] = line[token];
    }
    this->arguments[idx] = 0;
    this->argumentIndex = 0;

    // skip spaces before
    if (this->arguments[this->argumentIndex] == ' ') {
        for (; this->arguments[this->argumentIndex] != 0 && this->arguments[this->argumentIndex] == ' '; this->argumentIndex++) {}
    }

    // skip spaces after
    token = 0;
    for(; this->arguments[token] != 0; token++) {}
    token--;
    for (; token >= 0 && this->arguments[token] == ' '; token--) {
        this->arguments[token] = 0; 
    }
}

int Command::argumentCount() {
    int count = 0;
    bool hasChar = false;


    int token = this->argumentIndex;

    // skip spaces
    if (this->arguments[token] == ' ') {
        for (; this->arguments[token] != 0 && this->arguments[token] == ' '; token++) {}
    }

    // count spaces
    bool curSpace = false;
    for (; this->arguments[token] != 0; token++) {
        if (this->arguments[token] == ' ') {
            if (!curSpace) {
                count++;
            }

            curSpace = true;
        }

        if (!hasChar && this->arguments[token] != ' ') {
            hasChar = true;
            curSpace = false;
        }
    }

    if (hasChar) {
        count++;
    }

    return count;
}

char* Command::getCommand() {
    return this->command;
}

char* Command::nextArgument() {
    int token = this->argumentIndex;

    // skip spaces
    if (this->arguments[token] == ' ') {
        for (; this->arguments[token] != 0 && this->arguments[token] == ' '; token++) {}
    }

    // find next space and copy command.
    int idx = 0;
    for (; this->arguments[token] != 0 && this->arguments[token] != ' ' && idx<MAX_COMMAND_LENGTH; token++) {
        this->currentArgument[idx++] = this->arguments[token];
    }
    this->currentArgument[idx] = 0;

    this->argumentIndex = token;

    return this->currentArgument;
}

bool Command::hasArgument() {
    int token = this->argumentIndex;

    // skip spaces
    if (this->arguments[token] == ' ') {
        for (; this->arguments[token] != 0 && this->arguments[token] == ' '; token++) {}
    }

    return this->arguments[token] != 0;
}
