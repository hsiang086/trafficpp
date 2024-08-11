#pragma once

class ArgParse {
    private:
        int argc;
        char **argv;
    public:
        ArgParse(int argc, char **argv) {
            this->argc = argc;
            this->argv = argv;
        }

        bool isValid();
        void parse();
        void printHelp();
        void printVersion();

        bool run = false;
};