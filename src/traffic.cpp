#include "traffic.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>

#define ETHERTYPE_IP 0x0800

TrafficppSniffer::TrafficppSniffer(ArgParse& argparse) {
    if (argparse.flags["run"]) {
        pcap_if_t *alldevs;
        pcap_if_t *d;

        // Find all available devices
        if (pcap_findalldevs(&alldevs, errbuf) == -1) {
            fprintf(stderr, "%s%sCouldn't find default device: %s%s\n", ColorText::RED.c_str(), ColorText::BOLD.c_str(), errbuf, ColorText::RESET.c_str());
            exit(1);
        }

        dev = alldevs->name;

        printf("%sUsing device: %s%s\n", ColorText::CYAN.c_str(), dev, ColorText::RESET.c_str());

        pcd = pcap_open_live(dev, BUFSIZ, 1, 700, errbuf);
        if (pcd == NULL) {
            fprintf(stderr, "%s%sCouldn't open device %s: %s%s\n", ColorText::RED.c_str(), ColorText::BOLD.c_str(), dev, errbuf, ColorText::RESET.c_str());
            pcap_freealldevs(alldevs); // Free the device list before exiting
            exit(1);
        }

        pcap_freealldevs(alldevs);
    }
}

TrafficppSniffer::~TrafficppSniffer() {
    pcap_close(pcd);
}

void TrafficppSniffer::startSniffing() {
    pcap_loop(pcd, 0, TrafficppSniffer::callback, NULL);
}

void TrafficppSniffer::callback(u_char *useless, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    struct ether_header *eth_header;
    struct ip *ip_header;
    struct tcphdr *tcp_header;
    unsigned short ether_type;
    int ip_header_len;
    int total_len = pkthdr->len;

    // Decode Ethernet header
    eth_header = (struct ether_header *)packet;
    ether_type = ntohs(eth_header->ether_type);

    printf("%sEthernet Header:%s\n", ColorText::MAGENTA.c_str(), ColorText::RESET.c_str());
    printf("   |-Destination MAC: %s%02x:%02x:%02x:%02x:%02x:%02x%s\n",
           ColorText::YELLOW.c_str(),
           eth_header->ether_dhost[0], eth_header->ether_dhost[1], eth_header->ether_dhost[2],
           eth_header->ether_dhost[3], eth_header->ether_dhost[4], eth_header->ether_dhost[5],
           ColorText::RESET.c_str());
    printf("   |-Source MAC: %s%02x:%02x:%02x:%02x:%02x:%02x%s\n",
           ColorText::YELLOW.c_str(),
           eth_header->ether_shost[0], eth_header->ether_shost[1], eth_header->ether_shost[2],
           eth_header->ether_shost[3], eth_header->ether_shost[4], eth_header->ether_shost[5],
           ColorText::RESET.c_str());
    printf("   |-EtherType: %s0x%04x%s\n", ColorText::BLUE.c_str(), ether_type, ColorText::RESET.c_str());

    // Decode IP header if EtherType is IP
    if (ether_type == ETHERTYPE_IP) {
        ip_header = (struct ip *)(packet + sizeof(struct ether_header));
        ip_header_len = ip_header->ip_hl * 4;

        printf("%sIP Header:%s\n", ColorText::MAGENTA.c_str(), ColorText::RESET.c_str());
        printf("   |-Source IP: %s%s%s\n", ColorText::CYAN.c_str(), inet_ntoa(ip_header->ip_src), ColorText::RESET.c_str());
        printf("   |-Destination IP: %s%s%s\n", ColorText::CYAN.c_str(), inet_ntoa(ip_header->ip_dst), ColorText::RESET.c_str());
        printf("   |-Protocol: %s%d%s\n", ColorText::GREEN.c_str(), ip_header->ip_p, ColorText::RESET.c_str());
        printf("   |-IP Header Length: %s%d%s bytes\n", ColorText::GREEN.c_str(), ip_header_len, ColorText::RESET.c_str());

        // Check if protocol is TCP
        if (ip_header->ip_p == IPPROTO_TCP) {
            tcp_header = (struct tcphdr *)(packet + sizeof(struct ether_header) + ip_header_len);

            printf("%sTCP Header:%s\n", ColorText::MAGENTA.c_str(), ColorText::RESET.c_str());
            printf("   |-Source Port: %s%u%s\n", ColorText::GREEN.c_str(), ntohs(tcp_header->th_sport), ColorText::RESET.c_str());
            printf("   |-Destination Port: %s%u%s\n", ColorText::GREEN.c_str(), ntohs(tcp_header->th_dport), ColorText::RESET.c_str());
            printf("   |-Sequence Number: %s%u%s\n", ColorText::GREEN.c_str(), ntohl(tcp_header->th_seq), ColorText::RESET.c_str());
            printf("   |-Acknowledgment Number: %s%u%s\n", ColorText::GREEN.c_str(), ntohl(tcp_header->th_ack), ColorText::RESET.c_str());
            printf("   |-Header Length: %s%d%s bytes\n", ColorText::GREEN.c_str(), tcp_header->th_off * 4, ColorText::RESET.c_str());

            // Decode payload
            int tcp_header_len = tcp_header->th_off * 4;
            int payload_offset = sizeof(struct ether_header) + ip_header_len + tcp_header_len;
            int payload_len = total_len - payload_offset;

            if (payload_len > 0) {
                const u_char *payload = packet + payload_offset;

                TrafficppSniffer::printPayload(payload, payload_len);

                // TrafficppSniffer::printPayloadHex(payload, payload_len);
            }
        }
    }
    printf("\n");
}

void TrafficppSniffer::printPayload(const u_char *payload, int len) {
    printf("%sPayload (%d bytes):%s\n", ColorText::MAGENTA.c_str(), len, ColorText::RESET.c_str());

    for (int i = 0; i < len; i++) {
        if (isprint(payload[i])) {
            printf("%c", payload[i]);
        } else {
            printf(".");
        }
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

void TrafficppSniffer::printPayloadHex(const u_char *payload, int len) {
    printf("%sPayload in Hex (%d bytes):%s\n", ColorText::MAGENTA.c_str(), len, ColorText::RESET.c_str());

    for (int i = 0; i < len; i++) {
        printf("%02x ", payload[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
