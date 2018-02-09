#
# File: Makefile
# Creator: George Ferguson
# Created: Thu Jun 30 11:00:49 2016
# Time-stamp: <Tue Aug  8 10:34:37 EDT 2017 ferguson>
#

PROGRAMS = IntSet_test LinkedList_test auto dfa nfa

CFLAGS = -g -std=c99 -Wall -Werror

programs: $(PROGRAMS)

IntSet_test: IntSet_test.o IntSet.o
	$(CC) -o $@ $^

LinkedList_test: LinkedList_test.o LinkedList.o
	$(CC) -o $@ $^

auto: auto.o IntSet.o LinkedList.o dfa.o nfa.o
	$(CC) -o auto auto.o IntSet.o LinkedList.o dfa.o nfa.o

dfa: dfa.o IntSet.o 
	$(CC) -o dfa dfa.o IntSet.o

nfa: nfa.o IntSet.o LinkedList.o
	$(CC) -o nfa nfa.o IntSet.o LinkedList.o

clean:
	-rm $(PROGRAMS) *.o
