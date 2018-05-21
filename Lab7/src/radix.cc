/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <vector>
#include "radix.h"

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


RadixServer::RadixServer(const int port, const unsigned int cores) {
    int port = atoi(argv[1]);

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

    std::vector<std::thread*> threads;

    for (int id = 0; id < 10; id++) {
        threads.push_back(new std::thread{[&sockfd,id] {
            char buffer[256];
            for (;;) {
                struct sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);
                int newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &len);
                if (newsockfd < 0) exit(-1);

                bzero(buffer,256);
                int n = recv(newsockfd,buffer,255,0);
                if (n < 0) exit(-1);

                printf("Thread %d Received: %s\n", id, buffer);

                n = send(newsockfd,buffer,strlen(buffer),0);
                if (n < 0) exit(-1);

                close(newsockfd);
            }
        }});
    }

    for (std::thread *thread : threads)
        thread->join();

    close(sockfd);

}


void RadixClient::msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("open");

    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) exit(-1);

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    int port = atoi(argv[2]);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) exit(-1);

    int n = write(sockfd,argv[3],strlen(argv[3]));
    if (n < 0) exit(-1);

    char buffer[256];
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) exit(-1);

    printf("Received: %s\n", buffer);

    close(sockfd);

}



//
