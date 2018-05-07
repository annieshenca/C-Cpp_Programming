/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */
#include <iostream>
#include <string>
#include "radix.h"

static int R = 2<<8;

static int charAt (std::string s, int i) {
    if (i < s.length()) {
        return s.at(i);
    }
    return -1;
}

static void sort(unsigned int s[], unsigned int aux[], int low, int high, int at) {

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
    int ununsed = 0; // to fail complier warming test, remove
}

void RadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) {
    std::cout << "myvector contains:";

    for (std::vector<std::reference_wrapper<std::vector<unsigned int>>>::iterator it = lists.begin() ; it != lists.end(); ++it)
        std::cout << ' ' << *it;
    
    std::cout << '\n';

}

int main() {
    unsigned int s[] = {43, 102, 11, 21, 37, 110, 34, 99, 745};
    sort(s, 9);
    for (int i = 0; i < 9; ++i)
        std::cout << std::to_string(s[i]) << std::endl;


}
