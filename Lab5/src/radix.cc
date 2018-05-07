/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */
#include <iostream>
#include <string>
#include <thread>
#include "radix.h"

static int R = 2<<8;

static int charAt (std::string s, int i) {
    if ( (unsigned int)i < s.length() ) {
        return s.at(i);
    }
    return -1;
}

static void sort (unsigned int s[], unsigned int aux[], int low, int high, int at) {

    if(high <= low)
        return;

    int count[R+2];

    for (int i = 0; i <= R+2; i++)
        count[i] = 0;

    for (int i = low; i <= high; ++i)
        count[charAt(std::to_string(s[i]), at)+2]++;

    for (int i = 0; i < R+1; ++i)
        count[i+1] += count[i];

    for (int i = low; i <= high; ++i)
        aux[count[charAt(std::to_string(s[i]), at)+1]++] = s[i];

    for (int i = low; i <= high; ++i)
        s[i] = aux[i-low];

    for (int r = 0; r < R; ++r)
        sort(s, aux, low+count[r], low+count[r+1]-1, at+1);
}

static void sort (unsigned int s[], int len) {
    unsigned int aux[len];
    int low = 0;
    int high = len - 1;
    int at = 0;
    sort(s, aux, low, high, at);
}

RadixSort::RadixSort(const unsigned int cores) {
    int unused = 0;
}


void RadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
    std::vecotr<std::thread> threads;

    // Iterating through the vectors in the vector
    for(std::vector<unsigned int> &vec : lists) {
        sort( vec.data(), vec.size() );

        if ( < 24) {
            
        }

        if () {

        }
    }
    // for (each list)
    // if ( # of threads currently running < cores )
    //     create new thread
    // if ( # of threads currently running == cores || # threads created in total == # of lists )
    //     for ( each thread currently running )
    //         join and remove thread from currently running threads collection

}

// void RadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
//     // Iterating through the vectors in the vector
//     for(std::vector<unsigned int> &vec : lists) {
//         sort( vec.data(), vec.size() );
//     }
//
// }
