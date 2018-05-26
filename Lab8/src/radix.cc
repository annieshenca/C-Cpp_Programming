/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
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
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>

#include "radix.h"

/*
 * Start a UDP listener on PORT and accept lists of unsiged integers from
 * clients to be MSD RAdix sorted using no more that CORES cpu cores. After
 * sorting the lists are to be retiurned to the client.
 */
void RadixServer::start(const int port, const unsigned int cores) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) exit(-1);

    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        exit(-1);

    listen(sockfd,100);
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &len);
    if (newsockfd < 0) exit(-1);

    std::vector<unsigned int> list;

    for (;;) {
        unsigned int tran;
        unsigned int ftran;
        int n = 0;

        for (;;) {
            n = recv(newsockfd, (void*)&tran, sizeof(unsigned int), 0);
            if (n < 0) exit(-1);
            ftran = ntohl(tran);
            if (ftran == 0) break;
            list.push_back(ftran);
        }

        radixSort(list);

        for (unsigned int &l : list) {
            tran = htonl(l);
            n = write(newsockfd, (void*)&tran, sizeof(unsigned int));
            if (n < 0) exit(-1);
        }
        tran = htonl(0);
        n = write(newsockfd, (void*)&tran, sizeof(unsigned int));
        if (n < 0) exit(-1);
        list.clear();
        sleep(5);
    }
    close(newsockfd);
    close(sockfd);
}

/*
 * Shutdown the server. Typically this will involve closing the server socket.
 */
void RadixServer::stop() {
    throw "not implemented";
}

/*
 * Send the contents of the lists contained with LISTS to the server on HIOSTNAME
 * listening on PORT in datagrams containing batches of unsigned integers. These
 * will be returned to you MSD Radix sorted and should be retied to the caller
 * via LISTS.
 */
void RadixClient::msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
    // throw "not implemented";
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("open");

    struct hostent *server = gethostbyname(hostname);
    if (server == NULL) exit(-1);

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(port);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) exit(-1);

    for (std::vector<unsigned int> &list : lists) {
        unsigned int tran;
        int n = 0;
        for (unsigned int &l : list) {
            tran = htonl(l);
            n = write(sockfd, (void*)&tran, sizeof(unsigned int));
            if (n < 0) exit(-1);
        }
        tran = htonl(0);
        n = write(sockfd, (void*)&tran, sizeof(unsigned int));
        if (n < 0) exit(-1);
        list.clear();


        for(;;) {
            unsigned int tran = 0;
            n = recv(sockfd, (void*)&tran, sizeof(unsigned int), 0);
            if (n < 0) exit(-1);
            unsigned int v = ntohl(tran);
            if (v == 0){
                break;
            }
            list.push_back(v);
        }

    }
    close(sockfd);
}
