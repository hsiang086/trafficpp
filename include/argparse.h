#ifndef ARGPARSE_H
#define ARGPARSE_H
#pragma once

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <map>

class ArgParse {
public:
    ArgParse(int argc, char **argv);

    bool isValid();
    void parse();
    std::string getVersion();
    std::map<std::string, bool> flags;

private:
    int argc;
    char **argv;
    
    YAML::Node config;
    YAML::Node argsConfig;
    std::map<std::string, std::string> options;

    void processArg(const std::string& arg);
    
    void printVersion();
    void printHelp();
};

#endif