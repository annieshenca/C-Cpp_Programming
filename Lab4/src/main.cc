/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>

#define USAGE "USAGE %s test-file\n"

int main(int argc, char *argv[]) 
{
    int ununsed; // to fail warning test, remove

    if (argc < 2) {
        printf(USAGE, argv[0]);
        return -1;
    }
    
    std::cout << "Test file: " << argv[1] << std::endl;
    return 0;
}
