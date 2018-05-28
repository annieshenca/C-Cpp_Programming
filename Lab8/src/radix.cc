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

        msg.num_values = ntohl(msg.num_values);
        msg.sequence = ntohl(msg.sequence);
        msg.flag = ntohl(msg.flag);
        // For each number in this datagram, push all of numbers into the list.
        for (unsigned int i = 0; i < msg.num_values; i++) {
            msg.values[i] = ntohl(msg.values[i]);
            list.push_back(msg.values[i]);
        }


        // Once server receives the last datagram, start processing the list now.
        if (msg.flag == LAST) {
            // Call sorting algorithm on the received list numbers.
            radixSort(list);

            // Start sending the result list back to client.
            msg.num_values = 0;
            msg.sequence = 0;
            msg.flag = NONE;

            for (unsigned int &l : list) {
                msg.values[msg.num_values++] = l;
                if (msg.num_values == MAX_VALUES) {
                    backup = msg.sequence;

                    //Right before sending, change all of msg's struct to htonl()!
                    for (unsigned int i = 0; i < msg.num_values; i++) {
                        msg.values[i] = htonl(msg.values[i]);
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
            for (unsigned int i = 0; i < msg.num_values; i++) {
                msg.values[i] = htonl(msg.values[i]);
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
    Message send_msg;
    for (std::vector<unsigned int> &list: lists) {
        int n = 0;
        unsigned int backup = 0;
        send_msg.num_values = 0;
        send_msg.sequence = 0;
        send_msg.flag = NONE;

        // For each number l in this current list.
        for (unsigned int &l : list) {
            // Set current number into msg.values array until a datagram is full.
            send_msg.values[send_msg.num_values++] = l;
            if (send_msg.num_values == MAX_VALUES) {
                backup = send_msg.sequence;

                //Right before sending, change all of msg's struct to htonl()!
                for (unsigned int &num : send_msg.values) {
                    num = htonl(num);
                }
                send_msg.num_values = htonl(send_msg.num_values);
                send_msg.sequence = htonl(send_msg.sequence);
                send_msg.flag = htonl(send_msg.flag);

                // Send the datagram!
                n = sendto(sockfd, (void*)&send_msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, len);
                if (n < 0) exit(-1);

                // variables update
                send_msg.sequence = backup + 1;
                send_msg.num_values = 0;
            }
        }

        send_msg.flag = LAST; // Indicating this is the last datagram being sent.

        // Covert to Network byte order one last time.
        for (unsigned int &num : send_msg.values) {num = htonl(num);}
        unsigned int backup_send_seq = send_msg.sequence;

        send_msg.num_values = htonl(send_msg.num_values);
        send_msg.sequence = htonl(send_msg.sequence);
        send_msg.flag = htonl(send_msg.flag);
        n = sendto(sockfd, (void*)&send_msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, len);
        if (n < 0) exit(-1);
        list.clear();

        backup_send_seq++;

        // Receiving msg back from server!
        Message recv_msg;
        std::vector<unsigned int> arr; // Create an empty list for seq.

        do {
            n = recvfrom(sockfd, (void*)&recv_msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, &len);
            if (n < 0) exit(-1);;

            recv_msg.num_values = ntohl(recv_msg.num_values);
            recv_msg.sequence = ntohl(recv_msg.sequence);
            recv_msg.flag = ntohl(recv_msg.flag);

            // Knowing what seq I receive. For checking lost datagrams later.
            arr.push_back(recv_msg.sequence);

            for (unsigned int &l : recv_msg.values) {
                l = ntohl(l);
                list.push_back(l);
            }

            //t = select(sockfd, (fd_set *)0, &write_mask, (fd_set *)0, &tv);
        } while (recv_msg.flag == NONE);

        printf("***  back up send seq: %u\n", backup_send_seq);
        printf("*** recv_msg.sequence: %u\n", recv_msg.sequence);
        // int o =0;
        // for (unsigned int &h : arr) {
        //     printf("i: %i & h: %u\n", o++, h);
        // }

        // Meaning if Server missed a datagram.
        // Figure out which sequence client is missing, send a datagram requesting
        // a RESEND on that specific datagram.
        if (backup_send_seq != recv_msg.sequence) {
            unsigned i = 0;
            for (unsigned int &a : arr) {
                if (i != a) {
                    // NOW I know i is the seq number I'm missing!!!
                    // So! send SERVER a datagram with RESEND and an empty LAST.
                    send_msg.num_values = 1;
                    send_msg.num_values = htonl(send_msg.num_values);
                    send_msg.sequence = i;
                    send_msg.sequence = htonl(send_msg.sequence);
                    send_msg.flag = RESEND;
                    send_msg.flag = htonl(send_msg.flag);
                    // send_msg.values[0] = i;
                    send_msg.values[0] = htonl(i);
                    n = sendto(sockfd, (void*)&send_msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, len);
                    if (n < 0) exit(-1);
                    i = a+1;
                }
            }


            // New list for saving the old list to be able to add in the new seq.
            std::vector<unsigned int> backup_list;
            for (unsigned int &s : list) { backup_list.push_back(s); }
            list.clear();

            do {
                printf("&&&&&&&&&&&&&&&\n");
                n = recvfrom(sockfd, (void*)&recv_msg, sizeof(Message), 0, (struct sockaddr *) &serv_addr, &len);
                if (n < 0) exit(-1);;
                recv_msg.flag = ntohl(recv_msg.flag);
                for (unsigned int &r : recv_msg.values) {
                    r = ntohl(r);
                    list.push_back(r);
                }
            } while (recv_msg.flag == NONE);

            for (unsigned int &q : backup_list) { list.push_back(q); }
        }

        //DONE>!>!>?!??!

    } // End of for list in lists
    close(sockfd);
}
