#!/bin/sh

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET_COLOR="\033[0m"
DIR="output"

test_case_names=("testDbNewDbFree", "testDbInsertRecord", "testDbFindByFlightNumber",
    "testDbFindByDepartureAirportDay", "testDbFindBetweenTimes", "testDbFindNextFlight")

fail_count=0
quiet="-s"
dd_quiet="--silent"

make > /dev/null || exit 1

if [[ $1 == $quiet ]] || [[ $1 == $dd_quiet ]]; then
    # quiet mode
    for i in {1..6}; do
        echo "case $i:" ${test_case_names[i-1]}
        ./testAss1 $i > $DIR/$i.out

        if diff $DIR/$i.exp $DIR/$i.out > /dev/null; then
            printf "${GREEN}Tests passed\n$RESET_COLOR"
        else
            printf "${RED}Tests failed\n$RESET_COLOR"
            ((++fail_count))
        fi
    done

else 
    # show differences
    for i in {1..6}; do
        echo "case $i:" ${test_case_names[i-1]}
        ./testAss1 $i > $DIR/$i.out

        if diff $DIR/$i.exp $DIR/$i.out; then
            printf "${GREEN}Tests passed\n\n$RESET_COLOR"
        else
            printf "${RED}Tests failed\n\n$RESET_COLOR"
            ((++fail_count))
        fi
    done
fi

if [[ $fail_count -eq 0 ]]; then
    printf "${GREEN}All Tests passed!\n$RESET_COLOR"
else
    printf "${RED}$fail_count Tests failed\n$RESET_COLOR"
fi