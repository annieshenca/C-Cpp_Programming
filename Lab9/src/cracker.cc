// Annie Shen (ashen7)

#include <iostream>
#include <strings.h>
#include <string.h>
#include <thread>
#include <algorithm>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>

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
 * MULTIcast UDP Receiver.
 * Received UDP msg from server, ntohl() the msg, get the msg ready for
 * cracking the password.
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
    multicastRequest.imr_multiaddr.s_addr = inet_addr("124.0.01");
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicastRequest, sizeof(multicastRequest)) < 0)
        error("setsockopt");

    printf("Listening...\n");
    while (true) {
        printf("got into while true loop.\n");

        int n = recvfrom(sockfd, (void*)&m, sizeof(m),0,NULL,0);
        if (n < 0) error("read");

        printf("n: %d\n", n);

        // strings do not need to be converted
        m->num_passwds = ntohl(m->num_passwds);
        m->port = ntohl(m->port);

        printf("Received on port: %d\n", m->port);
    }
    close(sockfd);
}


/*
 * UNIcast TCP Sender
 */
/*
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

}*/


void crackpwd (Message *m) {
    printf("In crackpwd about to crack the password!\n");
    // Pass in hash const char hash and char passwd
    //const char hash = '1';
    //crack(&hash, m->passwds);
}


int main (int argc, char *argv[]) {
    Message msg;
    /*
    while (true) {
        receive(&msg);
        crackpwd(&msg);
        //send(&msg);
    }*/

    return 0;
}
