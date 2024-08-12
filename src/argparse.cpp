#include "argparse.h"
#include "color.h"
#include <cstdio>

ArgParse::ArgParse(int argc, char **argv)
    : argc(argc), argv(argv) {
    std::string executablePath = std::filesystem::current_path().string();
    this->config = YAML::LoadFile(executablePath + "/../src/static/config.yaml");
    this->argsConfig = YAML::LoadFile(executablePath + "/../src/static/argsconfig.yaml");
}

bool ArgParse::isValid() {
    if (argc < 2) {
        printHelp();
        return false;
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
}

void ArgParse::printVersion() {
    printf("%sTrafficpp v%-6s -> A cool traffic sniffer%s\n", ColorText::CYAN.c_str(), getVersion().c_str(), ColorText::RESET.c_str());
}

std::string ArgParse::getVersion() {
    return config["version"].as<std::string>();
}
