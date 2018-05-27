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


/*
 * Start a UDP listener on PORT and accept lists of unsigned integers from
 * clients to be MSD RAdix sorted using no more that CORES cpu cores. After
 * sorting the lists are to be retiurned to the client.
 */
void RadixServer::start(const int port, const unsigned int cores) {
    throw "not implemented";
    // int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // if (sockfd < 0) exit(-1);
    //
    // struct sockaddr_in server_addr;
    // bzero((char *) &server_addr, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    // server_addr.sin_port = htons(port);
    //
    // if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    //     exit(-1);
    //
    // int n = 0;
    // Message msg;
    // std::vector<unsigned int> list;
    //
    // while (true) {
    //     n = recvfrom(sockfd, (void*)&msg, sizeof(msg), 0, (struct sockaddr *) &serv_addr, &len);
    //     if (n < 0) exit(-1);
    //
    //     // For each number in this datagram, push all of numbers into the list.
    //     for (unsigned int &l : msg.values) {
    //         list.push_back(l);
    //     }
    //
    //     if (msg.flag == LAST) {
    //
    //     }
    // }
}



/*
Message msg
vector list

while true {
    rcv(msg)
    for (each number in msg.values) {
        list.push_back(number)
    }
    if (msg.flag == LAST) {
        sort(list)
        msg.num_values = 0
        msg.sequence = 0
        msg.flag = NONE

        for (each number in list) {
            msg.values[msg.num_values++] = number
            if (msg.num_values == MAX_VALUES) {
                send(msg)
                msg.sequence++
                msg.num_values = 0
            }
        }
        msg.flag = LAST
        send(msg)
        list.clear()
    }
}
*/



/*****************************************************************************/

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

    Message msg;
    for (std::vector<unsigned int> &list: lists) {
        int n = 0;
        msg.num_values = 0;
        msg.sequence = 0;
        msg.flag = NONE; // 0 stands for NONE.

        for (unsigned int &l : list) {
            msg.values[msg.num_values++] = l;
            if (msg.num_values == MAX_VALUES) {
                //Right before sending, change all of msg's struct to htonl()!



                // try sizeof(wire) if this does not work.
                n = sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, len);
                if (n < 0) exit(-1);
                msg.sequence++;
                msg.num_values = 0;
            }
        }
        msg.flag = LAST; // Indicating this is the last datagram being sent
         n = sendto(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, len);
        if (n < 0) exit(-1);
        list.clear();

        do {
            printf("****** right before recvfrom.\n");
            n = recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, &len);
            // if (n < 0) exit(-1);;
            printf("****** after recvfrom. val of n = %d\n", n);
            for (unsigned int &l : msg.values) {
                // printf("number after recv: %d\n", l);
                list.push_back(l);
            }
            printf("****** end of do-while loop.\n");
        } while (msg.flag == NONE);

        printf("****** exited the whole do-while loop.\n");
        // Once out of do-while loop, the last datagram would be the one with flag == 1.
        n = recvfrom(sockfd, (void*)&msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, &len);
        if (n < 0) exit(-1);
        for (unsigned int &l : msg.values) {
            list.push_back(l);
        }

    }

    close(sockfd);
}

//
// Message msg
//
// for (each list in lists) {
//     msg.num_values = 0
//     msg.sequence = 0
//     msg.flag = NONE
//
//     for (each number in list) {
//         msg.values[msg.num_values++] = number
//         if (msg.num_values == MAX_VALUES) {
//             send(msg)
//             msg.sequence++
//             msg.num_values = 0
//         }
//     }
//     msg.flag = LAST
//     send(msg)
//
//     list.clear()
//     do {
//         rcv(msg)
//         for (each number in msg.values) {
//             list.push_back(number)
//         }
//     } while (msg.flag == NONE)
// }
