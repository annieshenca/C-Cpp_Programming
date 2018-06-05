// Annie Shen (ashen7)

/*
 *  https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=071d09bc-7930-4bd4-b240-f7e1b346e90d
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=e8d75c88-e18b-45fc-b5b8-ddae7fcc3093
 *
 */

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

/* For printing out errors. */
void error(const char* str) {
    fprintf(stderr, "ERROR: %s\n", str);
    exit(-1);
}


/*
 * MULTIcast UDP Receiver.
 * Received UDP msg from server, ntohl() the msg, get the msg ready for
 * cracking the password.
 */
void receive (Message *m) {
    unsigned int port = get_multicast_port();
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) error("open");
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        error("bind");
    struct ip_mreq multicastRequest;

    char ip[16];
    memset(ip,0,16);
    in_addr_t src = get_multicast_address();
    if (inet_ntop(AF_INET,&src,ip,16) == NULL) {
        error("inet_ntop error");
    }

    multicastRequest.imr_multiaddr.s_addr = inet_addr(ip);
    multicastRequest.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicastRequest, sizeof(multicastRequest)) < 0)
        error("setsockopt");

    int n = 0;
    std::string my_cruzid = "ashen7";
    while (1) {
        printf("Receiving...\n");
        n = recvfrom(sockfd, m, sizeof(Message), 0, NULL, 0);
        if (n < 0) error("recvfrom()");
        // strings do not need to be converted
        m->num_passwds = ntohl(m->num_passwds);
        m->port = ntohl(m->port);
        printf("num of pwds: %u\n", m->num_passwds);
        // If receive msg for me! break and move on to cracking the password.
        if (m->cruzid == my_cruzid) {
            printf("Exiting Receive()...\n");
            break;
        }
    }
    close(sockfd);
}

/**********************************************************************************/

/*
 * UNIcast TCP Sender.
 * Since the msg has been decrypted, now it's time to send the msg back to
 * the server for grade.
 */
void send (Message *m) {
    m->port = htonl(m->port);
    int sockfd = socket(AF_INET,SOCK_STREAM ,0);
    if (sockfd < 0) error("open");
    struct hostent *server = gethostbyname(m->hostname);
    if (server == NULL) error("cracker server");
    struct sockaddr_in serv_addr;
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    printf("*** port: %u\n",m->port);

    serv_addr.sin_port = m->port;
    std::cout << serv_addr.sin_port << std::endl;
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("cracker connect");

    m->num_passwds = htonl(m->num_passwds);
    m->port = htonl(m->port);
    int n = sendto(sockfd, m, sizeof(Message), 0, NULL, 0);
    if (n < 0) error("sendto()");
    close(sockfd);
}

/**********************************************************************************/

/*
 * After cracker.cc receives msg, come here to crack the hash passwords. The
 * crackpwd() will store the decrypted passwords back into passwds[] and be
 * ready to be send back to the server.
 */
void crackpwd (Message *m) {
    // For loop - take each password one by one and place it into hash[],
    // then pass the password into crack() function provided by professor,
    // along with buffer[] for the result to be store in.
    char buffer[5];
    for (unsigned int i = 0; i < m->num_passwds; i++) {
        char* hash = m->passwds[i];
        // Pass in hash const char hash and char passwd
        crack(hash, buffer);
        printf("*** returned buffer: %s\n", buffer);
        // Store the decrypted password back into the passwds[]
        strcpy(m->passwds[i],buffer);
    }
}


int main (int argc, char *argv[]) {
    Message msg;

    while (true) {
        receive(&msg);
        crackpwd(&msg);
        send(&msg);
    }

    return 0;
}
