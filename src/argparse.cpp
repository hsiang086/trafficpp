#include "argparse.h"
#include "color.h"
#include <cstdio>

ArgParse::ArgParse(int argc, char **argv)
    : argc(argc), argv(argv) {
    std::string sourceFile = __FILE__;
    std::string sourceDir = sourceFile.substr(0, sourceFile.find_last_of("/\\"));
    this->config = YAML::LoadFile(sourceDir + "/static/config.yaml");
    this->argsConfig = YAML::LoadFile(sourceDir + "/static/argsconfig.yaml");

    for (const auto& option : argsConfig) {
        std::string name = option["name"].as<std::string>();
        std::string action = option["action"].as<std::string>();

        if (action == "store_true") {
            bool defaultValue = false;
            if (option["default"]) {
                defaultValue = option["default"].as<bool>();
            }
            flags[name] = defaultValue;
        }
    }
}

bool ArgParse::isValid() {
    bool foundValidArgument = false;
    
    if (argc < 2) {
        return true;
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        bool validArg = false;

        for (const auto& option : argsConfig) {
            auto args = option["args"].as<std::vector<std::string>>();
            for (const auto& opt : args) {
                if (arg == opt) {
                    validArg = true;
                    foundValidArgument = true;
                    break;
                }
            }
            if (validArg) break;
        }

        if (!validArg) {
            fprintf(stderr, "%s%sInvalid argument: %s%s\n", ColorText::RED.c_str(), ColorText::BOLD.c_str(), arg.c_str(), ColorText::RESET.c_str());
            return false;
        }
    }

    if (!foundValidArgument) {
        printHelp();
    }

    return true;
}

void ArgParse::parse() {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        processArg(arg);
    }
}

void ArgParse::processArg(const std::string& arg) {
    for (const auto& option : argsConfig) {
        std::string name = option["name"].as<std::string>();
        auto args = option["args"].as<std::vector<std::string>>();
        std::string help = option["help"].as<std::string>();
        std::string action = option["action"].as<std::string>();

        for (const auto& opt : args) {
            if (arg == opt) {
                if (action == "store_true") {
                    flags[name] = true;
                } else if (action == "store") {
                    // Handle store action
                } else if (action == "show_version") {
                    printVersion();
                } else if (action == "show_help") {
                    printHelp();
                }
                return;
            }
        }
    }

    fprintf(stderr, "%s%sInvalid argument: %s%s\n", ColorText::RED.c_str(), ColorText::BOLD.c_str(), arg.c_str(), ColorText::RESET.c_str());
}

void ArgParse::printHelp() {
    printf("%sTrafficpp v%-6s -> A cool traffic sniffer%s\n", ColorText::CYAN.c_str(), getVersion().c_str(), ColorText::RESET.c_str());
    printf("Usage: Trafficpp [options]\n");
    printf("Options:\n");

    for (const auto& option : argsConfig) {
        try {
            std::string name = option["name"].as<std::string>();
            auto args = option["args"].as<std::vector<std::string>>();
            std::string help = option["help"].as<std::string>();

            std::string args_str;
            if (args.size() > 1) {
                args_str = args[0] + " " + args[1];
            } else {
                args_str = args[0];
            }

            printf("  %s%-15s%s -> %s%s%s\n", ColorText::YELLOW.c_str(), args_str.c_str(), ColorText::RESET.c_str(), ColorText::GREEN.c_str(), help.c_str(), ColorText::RESET.c_str());
        } catch (const YAML::Exception& e) {
            fprintf(stderr, "%sError printing help: %s%s\n", ColorText::RED.c_str(), e.what(), ColorText::RESET.c_str());
        }
    }

    exit(1);
}

void ArgParse::printVersion() {
    printf("%sTrafficpp v%-6s -> A cool traffic sniffer%s\n", ColorText::CYAN.c_str(), getVersion().c_str(), ColorText::RESET.c_str());
    exit(1);
}

std::string ArgParse::getVersion() {
    return config["version"].as<std::string>();
}
