// test AVLTree operations

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "Record.h"
#include "AVLTree.h"
#include "FlightDb.h"

struct record {
    char flightNumber[MAX_FLIGHT_NUMBER + 1];
    char departureAirport[MAX_AIRPORT_NAME + 1];
    char arrivalAirport[MAX_AIRPORT_NAME + 1];
    int departureDay;    // 0 for Monday, ... , 6 for Sunday
    int departureHour;   // Between 0 and 23 (inclusive)
    int departureMinute; // Between 0 to 59 (inclusive)
    int durationMinutes; // Duration of the flight, in minutes
};

// In Tree.c
int compareByFlightNumber(Record r1, Record r2);
int compareByDepatureAirportDay(Record r1, Record r2);
void RecordShow(Record r);
void PrintTreePrefix(Tree t);
Record TreeNext(Tree t, Record r);

////////////////////////////////////////////////////////////////////////
// Comparison tests
void testComparisonZero() {
    // test zero
    Record r1 = RecordNew("QF409", "SYD", "MEL", 0, 8, 00, 90);
    Record r2 = RecordNew("QF409", "SYD", "MEL", 0, 8, 00, 90);
    int cmp = compareByFlightNumber(r1, r2);
    assert(cmp == 0);
}

void testComparisonPositive() {
    // test positive
    Record r1 = RecordNew("QF409", "SYD", "MEL", 0, 8, 00, 90);
    Record r2 = RecordNew("ZZ409", "SYD", "MEL", 5, 8, 00, 90);
    int cmp = compareByFlightNumber(r1, r2);
    assert(cmp < 0);
}

void testComparisonNegative() {
    // test negative
    Record r1 = RecordNew("ZZ409", "SYD", "MEL", 0, 8, 00, 90);
    Record r2 = RecordNew("QF409", "SYD", "MEL", 5, 8, 00, 90);
    int cmp = compareByFlightNumber(r1, r2);
    assert(cmp > 0);
}

////////////////////////////////////////////////////////////////////////
// AVLTree insert tests
void testAVLTreeInsertNone() {
    Tree t = TreeNew(compareByFlightNumber);
    printf("Insert None:\n");
    PrintTreePrefix(t);

    TreeFree(t, true);
}

void testAVLTreeInsertSame() {
    Record r1 = RecordNew("a", "SYD", "MEL", 0, 8, 00, 90);
    Record r2 = RecordNew("b", "SYD", "MEL", 0, 8, 00, 90);
    Record r3 = RecordNew("a", "SYD", "MEL", 0, 9, 00, 90);
    Record r4 = RecordNew("a", "SYD", "MEL", 0, 9, 00, 90);
    Tree t = TreeNew(compareByFlightNumber);
    printf("Insert Same:\n");
    TreeInsert(t, r1);
    PrintTreePrefix(t);
    TreeInsert(t, r2);
    PrintTreePrefix(t);
    TreeInsert(t, r3);
    PrintTreePrefix(t);
    TreeInsert(t, r4);
    PrintTreePrefix(t);

    TreeFree(t, true);
}

void testAVLTreeInsertMany() {
    Record r1 = RecordNew("Y", "SYD", "MEL", 0, 8, 00, 90);
    Record r2 = RecordNew("D", "SYD", "MEL", 0, 8, 00, 90);
    Record r3 = RecordNew("C", "SYD", "MEL", 0, 8, 00, 90);
    Record r4 = RecordNew("B", "SYD", "MEL", 0, 8, 00, 90);
    Record r5 = RecordNew("Z", "SYD", "MEL", 0, 8, 00, 90);
    Record r6 = RecordNew("ZZ", "SYD", "MEL", 0, 8, 00, 90);
    Tree t = TreeNew(compareByFlightNumber);
    printf("Insert Many:\n");
    PrintTreePrefix(t);
    TreeInsert(t, r1);
    PrintTreePrefix(t);
    TreeInsert(t, r2);
    PrintTreePrefix(t);
    TreeInsert(t, r3);
    PrintTreePrefix(t);
    TreeInsert(t, r4);
    PrintTreePrefix(t);
    TreeInsert(t, r5);
    PrintTreePrefix(t);
    TreeInsert(t, r6);
    PrintTreePrefix(t);

    TreeFree(t, true);
}

void testAVLTreeFindNext() {
    //25, 4, 3, 2, 26, 99 
    Record r1 = RecordNew("VA323", "MEL", "MEL", 4, 11, 0, 90); // 1
    Record r2 = RecordNew("QF409", "SYD", "MEL", 3, 7, 5, 90); // 6
    Record r3 = RecordNew("QF409", "SYD", "MEL", 1, 7, 5, 90); // 5
    Record r4 = RecordNew("QF409", "SYD", "MEL", 0, 7, 5, 90); // 2
    Record r5 = RecordNew("QF530", "SYD", "MEL", 0, 13, 5, 90); // 4
    Record r6 = RecordNew("QF409", "SYD", "MEL", 0, 8, 00, 90); // 3
    Tree t = TreeNew(compareByDepatureAirportDay);
    TreeInsert(t, r1);
    TreeInsert(t, r2);
    TreeInsert(t, r3);
    TreeInsert(t, r4);
    TreeInsert(t, r5);
    TreeInsert(t, r6);

    printf("\nTree Find Next:\n");
    Record next_r2 = TreeNext(t, r2);
    printf("D: Z = %s \n", next_r2->flightNumber);

    Record next_r3 = TreeNext(t, r3);
    printf("C: null = %s \n", next_r3->flightNumber);

    Record next_r4 = TreeNext(t, r4);
    printf("B: null = %s \n", next_r4->flightNumber);

    Record next_r5 = TreeNext(t, r5);
    printf("Z: ZZ = %s \n", next_r5->flightNumber);

    Record next_r6 = TreeNext(t, r6);
    printf("ZZ: null = %s \n", next_r6->flightNumber);

    Record next_r1 = TreeNext(t, r1);
    printf("Y: null = %s \n", next_r1->flightNumber);


    TreeFree(t, true);
}

int main(int argc, char *argv[]) {
    testComparisonZero();
    testComparisonPositive();
    testComparisonNegative();

    //testAVLTreeInsertNone();
    //testAVLTreeInsertMany();
    //testAVLTreeInsertSame();

    testAVLTreeFindNext();

	printf("All tests passed!\n");
}
