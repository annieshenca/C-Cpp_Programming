/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include "radix.h"
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <vector>

void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int cores) {
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
