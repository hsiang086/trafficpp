#include <stdio.h>
#include <filesystem>
#include <yaml-cpp/yaml.h>

#include "argparse.h"

std::string setExecutablePath() {
    return std::filesystem::current_path().string();
}

bool ArgParse::isValid() {
    if (argc < 2) {
        ArgParse::printHelp();
        return false;
    }
    return true;
}

void ArgParse::parse() {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            ArgParse::printHelp();
            break;
        } else if (arg == "-v" || arg == "--version") {
            ArgParse::printVersion();
            break;
        } else if (arg == "-r" || arg == "--run") {
            run = true;
        } else {
            printf("Invalid option: %s\n", argv[i]);
            ArgParse::printHelp();
            break;
        }
    }
}

void ArgParse::printHelp() {
    printf("Usage: traffic [OPTIONS]\n");
    printf("Options:\n");
    printf("  -r, --run      Run the program\n");
    printf("  -h, --help     Print this help message\n");
    printf("  -v, --version  Print the version\n");
}

void ArgParse::printVersion() {
    std::string executablePath = setExecutablePath();
    YAML::Node config = YAML::LoadFile(executablePath + "/../src/static/config.yaml");
    printf("Traffic v%s\n", config["version"].as<std::string>().c_str());
}