#!/bin/bash

#######################################################################
#
# Copyright (C) 2018 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

if [ "$#" -ne 1 ]; then
    echo "USAGE: ./perf.sh numbers"
    exit;
fi
num=$1

for lists in 4 6 8 10 12
do
    ./perf $num $lists $lists | tee radix.single

    speedup=`grep 'Speedup:' radix.single | awk '{print $2}' | sed 's/\%//'`

    echo -n "Speedup $lists lists $lists cores: "

    if (( $(echo "$speedup > 85" | bc -l) ))
    then
        echo "PASS"
    else
        echo "FAIL"
    fi
done


for lists in 4 8 12 16 20 24
do
    (( cores = lists / 2 ))

    ./radix $num $lists $cores > radix.single &
    pid=$!

    cmax=0
    while true
    do
        cat /proc/$pid/status > radix.threads 2>/dev/null
        status=$?
        if [ $status -ne 0 ]
        then
            break
        fi

        grep "Generate time:" radix.single >/dev/null
        if [ $? -eq 0 ]
        then
            threads=`cat radix.threads | grep Threads | awk '{print $2'}`
            cmax=$(( threads > cmax ? threads : cmax ))
            echo -n .
        else
            echo -n -
        fi

        sleep 0.2
    done

    (( cmax -= 1 ))
    echo ""

    echo -n "Capped $lists lists $cores cores: "

    if [ $cmax -eq $cores ]
    then
        echo "PASS"
    else
        echo "FAIL"
    fi
done

rm radix.single radix.quad radix.threads 2>/dev/null
