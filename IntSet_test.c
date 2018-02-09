/*
 * File: IntSet_test.c
 * Creator: George Ferguson
 * Created: Wed Sep  7 17:58:24 2016
 * Time-stamp: <Fri Aug  4 09:57:44 EDT 2017 ferguson>
 *
 * Test program for IntSet library.
 */

#include <stdio.h>
#include "IntSet.h"

void
myfunc(int value) {
    printf("myfunc: %d\n", value);
}

int
main(int argc, char **argv) {
    IntSet set1 = IntSet_new();

    printf("empty set1 contains 2? %d\n", IntSet_contains(set1, 2));

    printf("adding 1, 2, 3 to set1\n");
    IntSet_add(set1, 1);
    IntSet_add(set1, 2);
    IntSet_add(set1, 3);
    printf("set1 = ");
    IntSet_print(set1);
    printf("\n");

    printf("set1 contains 2? %d\n", IntSet_contains(set1, 2));

    printf("iterating over members of set1:\n");
    IntSet_iterate(set1, myfunc);

    IntSet set2 = IntSet_new();
    IntSet_add(set2, 3);
    printf("created set2 = ");
    IntSet_print(set2);
    printf("\n");
    printf("set1 contains_all set2? %d\n", IntSet_contains_all(set1, set2));
    printf("set2 contains_all set1? %d\n", IntSet_contains_all(set2, set1));
    printf("set1 equals set2? %d\n", IntSet_equals(set1, set2));
    printf("set2 equals set1? %d\n", IntSet_equals(set2, set1));

    printf("adding 2 and 1 to set2\n");
    IntSet_add(set2, 2);
    IntSet_add(set2, 1);
    printf("set2 = ");
    IntSet_print(set2);
    printf("\n");
    printf("set1 equals set2? %d\n", IntSet_equals(set1, set2));
    printf("set2 equals set1? %d\n", IntSet_equals(set2, set1));

    printf("freeing both sets\n");
    IntSet_free(set1);
    IntSet_free(set2);
}
