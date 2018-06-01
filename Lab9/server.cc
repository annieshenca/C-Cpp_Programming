// Annie Shen (ashen7)
// CMPS 109 - Lab 9 Password Cracker

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

#include "src/crack.h"
/*
 * test.cc - act as client!
 * Sends encrypted packets and receives decrypted ones.
 * Receives on UDP

 * MULTIcast UPD datagram containing password hashes
 * waits for plain text to come back over TCP
 */

void error(const char* str) {
    fprintf(stderr, "ERROR: %s\n", str);
}


/*
 * Multicast UDP Sender - example code from lecture 15 pg23.
 */
void send (Message *m) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) error("open");
    int ttl = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl)) < 0)
        error("setsockopt");
    struct sockaddr_in multicastAddr;
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr("224.0.0.1");
    multicastAddr.sin_port = htons(8080);

    for (;;) {
        //printf("Sending: %s\n", m->passwds);
        printf("sending\n");
        int n = sendto(sockfd, (void*)&m, sizeof(Message), 0,
        (struct sockaddr *) &multicastAddr, sizeof(multicastAddr));
        if (n < 0) error("write");
        sleep(1);
    }
    close(sockfd);
}

/*
void receive (Message *m) {

}*/

/*
 * Check if the returned result is the same as the password sent.
 */
 /*
void check (Message *m) {
	
}
* */

int main (int argc, char *argv[]) {
    Message msg;

    while (true) {
        send(&msg);
        //receive(&msg);
        //check(&msg);
    }

    return 0;
}
