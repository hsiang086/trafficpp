#include "argparse.h"
#include "traffic.h"

int main(int argc, char **argv) {
    ArgParse argParse(argc, argv);

    if (argParse.isValid()) {
        argParse.parse();
        TrafficppSniffer trafficppSniffer(argParse);
        trafficppSniffer.startSniffing();
    }

    return 0;
}
