/**
 * Name: Amin Ghasembeigi
 * ZID:  z5555555
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "FlightDb.h"
#include "AVLTree.h"

// assuming flight names will only contain numbers and letters
#define MAX_DUMMY_FLIGHT_NUMBER "zzzzzzzz"
#define MIN_DUMMY_FLIGHT_NUMBER "0"

#define MAX_DEPRATURE_DAY 6
#define MIN_DEPRATURE_DAY 0

#define MAX_DEPRATURE_HOUR 23
#define MIN_DEPRATURE_HOUR 0

#define MAX_DEPATURE_MINUTES 59
#define MIN_DEPATURE_MINUTES 0

// for parameters not compared in record parameters
#define FILLER " "
#define DURATION_MINUTES_FILLER 0

struct flightDb {
    Tree byFlightNumber;
    Tree byDepartureAirportDay;
    Tree byTime;
};

///////////////////////////////////////////////////////////////////////////////////
//                              comparison functions                             //
///////////////////////////////////////////////////////////////////////////////////

/**
 * Compares two records by flight number, then by depature day, then by
 * departure hour and then by departure minute.
 * 
 * if the records are equal, returns:
 * - A negative number if the first record is less than the second
 * - Zero if the records are equal
 * - A positive number if the first record is greater than the second
 */
static int compareByFlightNumber(Record r1, Record r2) {
    int cmp1 = strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2));
    if (cmp1 != 0) return cmp1;

    int cmp2 = RecordGetDepartureDay(r1) - RecordGetDepartureDay(r2);
    if (cmp2 != 0) return cmp2;

    int cmp3 = RecordGetDepartureHour(r1) - RecordGetDepartureHour(r2);
    if (cmp3 != 0) return cmp3;

    return RecordGetDepartureMinute(r1) - RecordGetDepartureMinute(r2);
}

/**
 * Compares two records by departure airport, then by depature day,
 * then by departure hour, then by departure minute and then by
 * flight number.
 * 
 * if the records are equal, returns:
 * - A negative number if the first record is less than the second
 * - Zero if the records are equal
 * - A positive number if the first record is greater than the second
 */
static int compareByDepatureAirportDay(Record r1, Record r2) {
    int cmp1 = strcmp(RecordGetDepartureAirport(r1), RecordGetDepartureAirport(r2));
    if (cmp1 != 0) return cmp1;

    int cmp2 = RecordGetDepartureDay(r1) - RecordGetDepartureDay(r2);
    if (cmp2 != 0) return cmp2;

    int cmp3 = RecordGetDepartureHour(r1) - RecordGetDepartureHour(r2);
    if (cmp3 != 0) return cmp3;

    int cmp4 = RecordGetDepartureMinute(r1) - RecordGetDepartureMinute(r2);
    if (cmp4 != 0) return cmp4;

    return strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2));
}

/**
 * Compares two records by depature day, then by depature hour,
 * then by depature minute and then by flight number.
 * 
 * if the records are equal, returns:
 * - A negative number if the first record is less than the second
 * - Zero if the records are equal
 * - A positive number if the first record is greater than the second
 */
static int compareByTime(Record r1, Record r2) {
    int cmp1 = RecordGetDepartureDay(r1) - RecordGetDepartureDay(r2);
    if (cmp1 != 0) return cmp1;

    int cmp2 = RecordGetDepartureHour(r1) - RecordGetDepartureHour(r2);
    if (cmp2 != 0) return cmp2;

    int cmp3 = RecordGetDepartureMinute(r1) - RecordGetDepartureMinute(r2);
    if (cmp3 != 0) return cmp3;

    return strcmp(RecordGetFlightNumber(r1), RecordGetFlightNumber(r2));
}

/////////////////////////////////////////////////////////////////////////////////
//                              task 2 functions                               //
/////////////////////////////////////////////////////////////////////////////////


/**
 * Creates a new flight DB. 
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
FlightDb DbNew(void) {
    FlightDb db = malloc(sizeof(*db));
    if (db == NULL) {
        fprintf(stderr, "couldn't allocate memory\n");
        exit(EXIT_FAILURE);
    }

    db->byFlightNumber = TreeNew(compareByFlightNumber);
    db->byDepartureAirportDay = TreeNew(compareByDepatureAirportDay);
    db->byTime = TreeNew(compareByTime);
    return db;
}

/**
 * Frees all memory allocated to the given flight DB
 */
void DbFree(FlightDb db) {
    TreeFree(db->byFlightNumber, false);
    TreeFree(db->byDepartureAirportDay, false);
    TreeFree(db->byTime, true);
    free(db);
}

/**
 * Inserts  a  flight  record  into the given DB if there is not already
 * record with the same flight number, departure airport, day, hour  and
 * minute.
 * If  inserted successfully, this function takes ownership of the given 
 * record (so the caller should not modify or free it). 
 * Returns true if the record was successfully inserted,  and  false  if
 * the  DB  already  contained  a  record  with  the same flight number,
 * departure airport, day, hour and minute.
 * The time complexity of this function must be O(log n).
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
bool DbInsertRecord(FlightDb db, Record r) {
    if (TreeInsert(db->byFlightNumber, r)) {
        TreeInsert(db->byDepartureAirportDay, r);
        TreeInsert(db->byTime, r);
        return true;
    }
    return false;
}

/**
 * Searches  for  all  records with the given flight number, and returns
 * them all in a list in increasing order of  (day, hour, min).  Returns
 * an empty list if there are no such records. 
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List DbFindByFlightNumber(FlightDb db, char *flightNumber) {
    Record dummyLower = RecordNew(flightNumber, FILLER, FILLER, MIN_DEPRATURE_DAY, MIN_DEPRATURE_HOUR, MIN_DEPATURE_MINUTES, DURATION_MINUTES_FILLER);
    Record dummyUpper = RecordNew(flightNumber, FILLER, FILLER, MAX_DEPRATURE_DAY, MAX_DEPRATURE_HOUR, MAX_DEPATURE_MINUTES, DURATION_MINUTES_FILLER);     
    List l = TreeSearchBetween(db->byFlightNumber, dummyLower, dummyUpper);

    RecordFree(dummyLower);
    RecordFree(dummyUpper);

    return l;
}

/**
 * Searches  for all records with the given departure airport and day of
 * week (0 to 6), and returns them all in a list in increasing order  of
 * (hour, min, flight number).
 * Returns an empty list if there are no such records.
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List DbFindByDepartureAirportDay(FlightDb db, char *departureAirport,
                                     int day) {
    Record dummyLower = RecordNew(MIN_DUMMY_FLIGHT_NUMBER, departureAirport, FILLER, day, MIN_DEPRATURE_HOUR, MIN_DEPATURE_MINUTES, DURATION_MINUTES_FILLER);
    Record dummyUpper = RecordNew(MAX_DUMMY_FLIGHT_NUMBER, departureAirport, FILLER, day, MAX_DEPRATURE_HOUR, MAX_DEPATURE_MINUTES, DURATION_MINUTES_FILLER);     
     
    List l = TreeSearchBetween(db->byDepartureAirportDay, dummyLower, dummyUpper);

    RecordFree(dummyLower);
    RecordFree(dummyUpper);

    return l;
}

/**
 * Searches  for  all  records  between  (day1, hour1, min1)  and (day2,
 * hour2, min2), and returns them all in a list in increasing  order  of
 * (day, hour, min, flight number).
 * Returns an empty list if there are no such records.
 * The  records  in the returned list should not be freed, but it is the
 * caller's responsibility to free the list itself.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
List DbFindBetweenTimes(FlightDb db, 
                            int day1, int hour1, int min1, 
                            int day2, int hour2, int min2) {
    Record dummyLower = RecordNew(MIN_DUMMY_FLIGHT_NUMBER, FILLER, FILLER, day1,  hour1,  min1,  DURATION_MINUTES_FILLER);
    Record dummyUpper = RecordNew(MAX_DUMMY_FLIGHT_NUMBER, FILLER, FILLER, day2,  hour2,  min2,  DURATION_MINUTES_FILLER);
     
    List l = TreeSearchBetween(db->byTime, dummyLower, dummyUpper);

    RecordFree(dummyLower);
    RecordFree(dummyUpper);

    return l;
}

/**
 * Searches  for  and  returns  the  earliest next flight from the given
 * departure airport, on or after the given (day, hour, min).
 * The returned record must not be freed or modified. 
 * The time complexity of this function must be O(log n).
 * You MUST use the AVLTree ADT (from Task 1) in your implementation.
 */
Record DbFindNextFlight(FlightDb db, char *departureAirport, 
                          int day, int hour, int min) {
    Record dummy = RecordNew(FILLER, departureAirport, FILLER, day,  hour,  min, DURATION_MINUTES_FILLER);
    Record rec = TreeNext(db->byDepartureAirportDay, dummy);

    RecordFree(dummy);
    if (rec != NULL && strcmp(RecordGetDepartureAirport(rec), departureAirport) == 0) {
        // found next flight
        return rec;
    }

    // didn't find next flight
    // wrap around day via minimum record
    Record minDummy = RecordNew(FILLER, departureAirport, FILLER, MIN_DEPRATURE_DAY,  MIN_DEPRATURE_HOUR,  MIN_DEPATURE_MINUTES, DURATION_MINUTES_FILLER);
    Record recWrapAround = TreeNext(db->byDepartureAirportDay, minDummy);

    RecordFree(minDummy);
    
    if (rec != NULL && strcmp(RecordGetDepartureAirport(recWrapAround), departureAirport) != 0) {
        // didn't find next flight even after wrapping around day
        return NULL;
    }

    // found next flight after wrapping to around day or TreeNext returned null
    return recWrapAround;
}
