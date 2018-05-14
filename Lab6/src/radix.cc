/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

/*
 * Sources:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=1a952c7b-5ebd-4215-a514-78fb6adfdb29
 * https://gist.github.com/chadluo/9eaebd6eac20c19c8fbb
 * http://www.codebytes.in/2015/12/msd-string-sort-java.html
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=bdacac7d-79ee-4ed5-a25b-6f1f3af9cb27
 */

#include "radix.h"
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <vector>


static int R = 2 << 8;

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

static void sort_alg (unsigned int s[], int len) {
    unsigned int aux[len];
    int low = 0;
    int high = len - 1;
    int at = 0;
    sort(s, aux, low, high, at);
}

void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int cores) {
    std::vector<std::thread*> threads; // Create a new vector to store threads.
    unsigned int num_threads = 0; // Total number of threads created.

    for(std::vector<unsigned int> &list : lists) {
        threads.push_back(new std::thread{[&list] {
            // std::sort(list.begin(), list.end(), [](const unsigned int &a, const unsigned int &b) {
            //     return std::to_string(a).compare(std::to_string(b)) < 0;
            // });
            // auto i = list.size();

            sort_alg(&list[0], list.size());
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

