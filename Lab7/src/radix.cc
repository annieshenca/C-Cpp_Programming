/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

/*
 * Sources:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=e8d75c88-e18b-45fc-b5b8-ddae7fcc3093 https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=b07033c4-f623-4907-9ab4-85e9fb3d79cb
 * https://www.geeksforgeeks.org/socket-programming-cc/
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


void error(const char* s){fprintf(stderr,"%s\n",s);}

void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, const unsigned int cores) {
    std::vector<std::thread*> threads;
    unsigned int num_threads = 0;

    // Iterating through the vectors in the vector
    for(std::vector<unsigned int> &list : lists) {
        // sort( vec.data(), vec.size() );
        threads.push_back(new std::thread{[&list] {
            std::sort(list.begin(), list.end(), [](const unsigned int &a, const unsigned int &b) {
                return std::to_string(a).compare(std::to_string(b)) < 0;
            });
        }});
        num_threads++;

        if (threads.size() == cores || num_threads == lists.size()) {
            for (std::thread *thread : threads) {
                thread->join();
            }
            threads.clear();
        }
    }
}




void radixSort(std::vector<unsigned int> &list) {

    std::thread *t = new std::thread{[&list] {
        std::sort(list.begin(), list.end(), [](const unsigned int &a, const unsigned int &b) {
            return std::to_string(a).compare(std::to_string(b)) < 0;
        });
    }};
    t->join();
}



RadixServer::RadixServer(const int port, const unsigned int cores) {
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


void RadixClient::msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
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
