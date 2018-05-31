// Annie Shen (ashen7@ucsc.edu)
// CMPS 109 - Lab 9 Distributed Password Cracker

#include <cstdlib>
#include <iostream>
#include <string>
#include "crack.h"

/*
 * main.cc - act as CRACKER!
 * Acts as the server of test.cc.
 * Listens for packets and decrypts them.
 * Sends out on multicast, receive on unicast
 */


void error(const char* str) {
    fprintf(stderr, "ERROR: %s\n", str);
}


/*
 * MULTIcast UDP Receiver
 */
void receive (Message *m) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) error("open");
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(get_multicast_port());
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        error("bind");
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicastRequest, sizeof(multicastRequest)) < 0)
        error("setsockopt");

    for (;;) {
        int n = recvfrom(sockfd, m,sizeof(Message),0,NULL,0);
        if (n < 0) error("read");
        printf("Received: %s\n", m);
    }
    close(sockfd);

}


/*
 * UNIcast TCP Sender
 */
void send (Message *m) {
    int port = atoi(argv[1]);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) error("open");
    int ttl = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl)) < 0)
        error("setsockopt");
    struct sockaddr_in multicastAddr;
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr("224.0.0.1");
    multicastAddr.sin_port = htons(get_unicast_port());

    for (;;) {
        printf("Sending: %s\n", argv[2]);
        int n = sendto(sockfd,argv[2], strlen(argv[2]), 0,
        (struct sockaddr *) &multicastAddr, sizeof(multicastAddr));
        if (n < 0) error("write");
        sleep(1);
    }
    close(sockfd);

}


void crack (Message *m) {
    // Pass in hash const char hash and char passwd
    //crack(hasn, passwd);
}


int main (int argc, char *argv[]) {
    Message msg;

    while (true) {
        receive(&msg);
        crack(&msg);
        send(&msg);
    }

    return 0;
}





//need hostname to send the TCP back