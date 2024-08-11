#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include "traffic.h"

void test() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs;
    pcap_if_t *device;
    pcap_t *handle;

    // Get the list of available devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Error finding devices: %s\n", errbuf);
        exit(1);
    }

    // Select the first device
    device = alldevs;

    // Open the device for capturing
    handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "Error opening device: %s\n", errbuf);
        exit(1);
    }

    // Capture a packet
    struct pcap_pkthdr header;
    const u_char *packet = pcap_next(handle, &header);

    printf("Captured a packet with length of [%d]\n", header.len);

    printf("Packet content:\n");

    for (int i = 0; i < header.len; i++) {
        printf("%02x ", packet[i]);
    }

    printf("\n");

    // Clean up
    pcap_close(handle);
    pcap_freealldevs(alldevs);
}