/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

/*
 * Sources:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=1a952c7b-5ebd-4215-a514-78fb6adfdb29
 */

#include "radix.h"
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <vector>


/*
 * Vectors within vectors!!!
 * Each vector list contains lists of things to operate on, which requires
 * creating threads to perform.
 */
void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int cores) {
    std::vector<std::thread*> threads; // Create a new vector to store threads.
    unsigned int num_threads = 0; // Total number of threads created.

    // For each vector-lists(list) within a vector-list(lists).
    // Call by references, never by value.
    for(std::vector<unsigned int> &list : lists) {

        // Push the newly created thread to the back vector-threads list.
        threads.push_back(new std::thread{[&list] {

            // Calling sorting algorithm. C++ has std::sort but need to specify
            // type, which in this case is unsigned int.
            std::sort(list.begin(), list.end(), [](const unsigned int &a, const unsigned int &b) {
                return std::to_string(a).compare(std::to_string(b)) < 0;
            });
        }});
        num_threads++; // Increment number of threads

        // Either when the number of threads equal to how many cores we have, or
        // the number of threads is the same as the vector-lists size, join the
        // threads to have them perform in order of thread creation.
        if (threads.size() == cores || num_threads == lists.size()) {
            for (std::thread *thread : threads) {
                thread->join();
                //thread.clear(); // Clear memory to pervent segimentation fault.
            }
            threads.clear(); // Clear memory to pervent segimentation fault.
        }
    }
}

/* Sudocode - Joining threads one by one */
// split list into sub-lists
//
// for each sub-list
//     create new thread {
//         sort sub-list
// }
//
// while at least one thread sitll running
//     join one thread
//
// merge sorted sub-list back into list.
