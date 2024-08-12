#ifndef TRAFFIC_H
#define TRAFFIC_H
#pragma once

#include "argparse.h"
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string>

class TrafficppSniffer {
public:
    TrafficppSniffer(ArgParse &argparse);
    ~TrafficppSniffer();
    void startSniffing();

private:
    pcap_t *pcd;
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];

    static void callback(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);
    static void printPayload(const u_char *payload, int len);
    static void printPayloadHex(const u_char *payload, int len);
};

#endif
