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


void error(const char* s){fprintf(stderr,"ERROR: %s\n",s);}


// void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, const unsigned int cores) {
//     std::vector<std::thread*> threads;
//     unsigned int num_threads = 0;
//     // Iterating through the vectors in the vector
//     for(std::vector<unsigned int> &list : lists) {
//         // sort( vec.data(), vec.size() );
//         threads.push_back(new std::thread{[&list] {
//             std::sort(list.begin(), list.end(), [](const unsigned int &a, const unsigned int &b) {
//                 return std::to_string(a).compare(std::to_string(b)) < 0;
//             });
//         }});
//         num_threads++;
//
//         if (threads.size() == cores || num_threads == lists.size()) {
//             for (std::thread *thread : threads) {
//                 thread->join();
//             }
//             threads.clear();
//         }
//     }
// }

void radixSort(std::vector<unsigned int> &list) {

    std::thread *t = new std::thread{[&list] {
        std::sort(list.begin(), list.end(), [](const unsigned int &a, const unsigned int &b) {
            return std::to_string(a).compare(std::to_string(b)) < 0;
        });
    }};
    t->join();
}


/*****************************************************************************/
/*****************************************************************************/


/*
 * Start a UDP listener on PORT and accept lists of unsigned integers from
 * clients to be MSD RAdix sorted using no more that CORES cpu cores. After
 * sorting the lists are to be retiurned to the client.
 */
void RadixServer::start(const int port, const unsigned int cores) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) exit(-1);

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        exit(-1);

    struct sockaddr_in remote_addr;
    socklen_t len = sizeof(remote_addr);


    int n = 0;
    unsigned int backup = 0;
    Message msg;
    std::vector<unsigned int> list;

    while (true) {
        n = recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &remote_addr, &len);
        if (n < 0) exit(-1);

        // For each number in this datagram, push all of numbers into the list.
        for (unsigned int &n : msg.values) {
            n = ntohl(n);
            //printf("****** num recved: %d\n", n);
            list.push_back(n);
        }
        printf("****** out of for loop.\n");
        msg.num_values = ntohl(msg.num_values);
        msg.sequence = ntohl(msg.sequence);
        msg.flag = ntohl(msg.flag);


        // Once server receives the last datagram, start processing the list now.
        if (msg.flag == LAST) {
            radixSort(list);
            msg.num_values = 0;
            msg.sequence = 0;
            msg.flag = NONE;

            for (unsigned int &l : list) {
                msg.values[msg.num_values++] = l;
                if (msg.num_values == MAX_VALUES) {
                    backup = msg.sequence;

                    //Right before sending, change all of msg's struct to htonl()!
                    for (unsigned int &num : msg.values) {
                        num = htonl(num);
                    }
                    msg.num_values = htonl(msg.num_values);
                    msg.sequence = htonl(msg.sequence);
                    msg.flag = htonl(msg.flag);

                    // Send the datagram!
                    n = sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &remote_addr, len);
                    if (n < 0) exit(-1);

                    // variables update
                    msg.sequence = backup + 1;
                    msg.num_values = 0;
                }
            }

            // Now sending the last datagram to client.
            msg.flag = LAST;
            for (unsigned int &num : msg.values) {
                num = htonl(num);
            }
            msg.num_values = htonl(msg.num_values);
            msg.sequence = htonl(msg.sequence);
            msg.flag = htonl(msg.flag);

            n = sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &remote_addr, len);
            if (n < 0) exit(-1);
            list.clear();
        }
    }
    close(sockfd);
}


/*
1. Message msg
2. vector list
3.
4. while true {
5. rcv(msg)
6. for (each number in msg.values) {
7. list.push_back(number)
8. }
9. if (msg.flag == LAST) {
10. sort(list)
11. msg.num_values = 0
12. msg.sequence = 0
13. msg.flag = NONE
14.
15. for (each number in list) {
16. msg.values[msg.num_values++] = number
17. if (msg.num_values == MAX_VALUES) {
18. send(msg)
19. msg.sequence++
20. msg.num_values = 0
21. }
22. }
23. msg.flag = LAST
24. send(msg)
25. list.clear()
26. }
27. }
*/


/*****************************************************************************/
/*****************************************************************************/


/*
 * Shutdown the server. Typically this will involve closing the server socket.
 */
void RadixServer::stop() {
}


/*****************************************************************************/
/*****************************************************************************/


/*
 * Send the contents of the lists contained with LISTS to the server on HIOSTNAME
 * listening on PORT in datagrams containing batches of unsigned integers. These
 * will be returned to you MSD Radix sorted and should be retied to the caller
 * via LISTS.
 */
void RadixClient::msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
    // Datagram and sockets set up!
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) error("open");
    struct hostent *server = gethostbyname(hostname);
    if (server == NULL) exit(-1);
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) exit(-1);
    socklen_t len = sizeof(serv_addr);

    /* Client sending and receiving! */
    Message msg;
    for (std::vector<unsigned int> &list: lists) {
        int n = 0;
        unsigned int backup = 0;
        msg.num_values = 0;
        msg.sequence = 0;
        msg.flag = NONE;


        for (unsigned int &i : list) {
            printf("****** list sending: %u\n", i);
        }


        // For each number l in this current list.
        for (unsigned int &l : list) {
            // Set current number into msg.values array until a datagram is full.
            msg.values[msg.num_values++] = l;
            if (msg.num_values == MAX_VALUES) {
                backup = msg.sequence;

                //Right before sending, change all of msg's struct to htonl()!
                for (unsigned int &num : msg.values) {
                    num = htonl(num);
                }
                msg.num_values = htonl(msg.num_values);
                msg.sequence = htonl(msg.sequence);
                msg.flag = htonl(msg.flag);

                // Send the datagram!
                n = sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, len);
                if (n < 0) exit(-1);

                // variables update
                msg.sequence = backup + 1;
                msg.num_values = 0;
            }
        }

        msg.flag = LAST; // Indicating this is the last datagram being sent.

        // Covert to Network byte order one last time.
        for (unsigned int &num : msg.values) {
            num = htonl(num);
        }
        msg.num_values = htonl(msg.num_values);
        msg.sequence = htonl(msg.sequence);
        msg.flag = htonl(msg.flag);

        n = sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, len);
        if (n < 0) exit(-1);
        list.clear();

        do {
            n = recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, &len);
            if (n < 0) exit(-1);;

            for (unsigned int &l : msg.values) {
                l = ntohl(l);
                list.push_back(l);
            }


            for (unsigned int &i : list) {
                printf("****** list received: %u\n", i);
            }


            msg.num_values = ntohl(msg.num_values);
            msg.sequence = ntohl(msg.sequence);
            msg.flag = ntohl(msg.flag);
        } while (msg.flag == NONE);
    }
    close(sockfd);
}
