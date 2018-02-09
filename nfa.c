#ifndef _nfa_h
#define _nfa_h

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IntSet.h"
#include "nfa.h"
#include "LinkedList.h"

#define INITIAL_STATE 0
#define NO_TRANSITION -1
#define TRUE 1;
#define FALSE 0;

typedef struct NFA *NFA;
struct NFA {
    int nstates;
    int Initial_State;
    IntSet **Transition_Function;
    IntSet Final_States;
    LinkedList *Useful_Char_Set;
    LinkedList *Useful_Input_Set;
} Nfa;

NFA NFA_new(int nstates) {
    NFA newNFA = (NFA) malloc(sizeof(struct NFA));
    newNFA->nstates = nstates;
    newNFA->Initial_State = INITIAL_STATE;
    newNFA->Final_States = IntSet_new();
    newNFA->Useful_Char_Set = LinkedList_new();
    newNFA->Useful_Input_Set = LinkedList_new();

    newNFA->Transition_Function = (IntSet **) malloc(nstates * sizeof(IntSet *));
    for (int i = 0; i < nstates; i++) {
        newNFA->Transition_Function[i] = (IntSet *) malloc(128 * sizeof(IntSet));
    }

    for (int i = 0; i < nstates; i++)
        for (int j = 0; j < 128; j++)
            newNFA->Transition_Function[i][j] = IntSet_new();

    return newNFA;

}

void NFA_free(NFA nfa) {
    free(nfa);
}

int NFA_get_size(NFA nfa) {
    return nfa->nstates;
}

IntSet NFA_get_transitions(NFA nfa, int state, char sym) {
    return nfa->Transition_Function[state][(int) sym];
}

void NFA_add_transition(NFA nfa, int src, char sym, int dst) {
    IntSet_add(nfa->Transition_Function[src][(int) sym], dst);

    LinkedListIterator *itr = LinkedList_iterator(nfa->Useful_Char_Set);
    bool notAdded = true;
    while (LinkedListIterator_has_next(itr)) {
        if (*(char *) LinkedListIterator_next(itr) == sym) {
            notAdded = false;
            break;
        }
    }
    if (notAdded) {
        char *symp = (char *) malloc(sizeof(char));
        *symp = sym;
        LinkedList_add_at_end(nfa->Useful_Char_Set, symp);
    }
}

void NFA_add_transition_str(NFA nfa, int src, char *str, int dst) {
    char *ptr = str;
    while (*ptr != '\0') {
        IntSet_add(nfa->Transition_Function[src][(int) *ptr], dst);
        ptr++;
    }
}

void NFA_add_transition_all(NFA nfa, int src, int dst) {
    for (int i = 0; i < 128; i++) {
        //printf("%d\n", i);
        IntSet_add(nfa->Transition_Function[src][i], dst);
    }

}

void NFA_set_accepting(NFA nfa, int state, bool value) {
    IntSet_add(nfa->Final_States, state);
}


bool NFA_get_accepting(NFA nfa, int state) {
    return IntSet_contains(nfa->Final_States, state);
}

bool NFA_execute(NFA nfa, char *input) {
    int src, sym;
    char *ptr;
    IntSet srcSet, dst,visited, temp;

    ptr = input;
    visited = IntSet_new();
    temp = IntSet_new();
    src = INITIAL_STATE;
    sym = (int) *ptr;
    dst = IntSet_new();
    srcSet = IntSet_new();

    IntSet_union(srcSet , nfa->Transition_Function[src][sym]);

    if(strlen(input)==0)
        return 0;

    while (*ptr != '\0') {

        printf("input:%c\t",*ptr);

        printf("Src: ");
        IntSet_print(srcSet);
        IntSet_union(visited, srcSet);
        if (!IntSet_is_empty(srcSet)) {

            IntSetIterator itr = IntSet_iterator(srcSet);
            temp = IntSet_new();
            while (IntSetIterator_has_next(itr)) {
                if(*(ptr+1)!='\0') {
                    sym = *(ptr + 1);
                    src = IntSetIterator_next(itr);

                    dst = nfa->Transition_Function[src][sym];

                    IntSet_union(visited, dst);
                    IntSet_union(temp, dst);
                }else{
                    break;
                }

            }


            srcSet = temp;
            free(itr);
        }
        printf("\tDst: ");
        IntSet_print(temp);
        printf("\tStates Visted: ");
        IntSet_print(visited);
        printf("\n");
        ptr++;
    }

    temp = IntSet_new();
    IntSet_add(temp,0);

    if(IntSet_is_empty(dst)|| IntSet_equals(dst,temp))
        return false;

    IntSetIterator itr1 = IntSet_iterator(nfa->Final_States);
    while (IntSetIterator_has_next(itr1)) {
        if (IntSet_contains(visited, IntSetIterator_next(itr1)))
            return true;
    }
    free(itr1);
    return false;

}

void NFA_print(NFA nfa) {
    for (int i = 0; i < nfa->nstates; i++) {
        printf("$state %d$: ", i);

        for (int j = 0; j < 128; j++) {
            if (!IntSet_is_empty(nfa->Transition_Function[i][j])) {
                printf("(sym: %c,", (char) j);
                IntSet_print(nfa->Transition_Function[i][j]);
                printf(") ");
            }
        }
        printf("\n");
    }

    printf("Accepting States: ");
    IntSet_print(nfa->Final_States);
    printf("\n");
}

IntSet NFA_get_Final_States(NFA nfa) {
    return nfa->Final_States;
}

LinkedList *NFA_get_Useful_Chars(NFA nfa) {
    return nfa->Useful_Char_Set;
}

//int main(){
//    NFA nfa = NFA_new(12);
//    NFA_add_transition_all(nfa,0,0);
//    NFA_add_transition_all(nfa,1,1);
//    NFA_add_transition_all(nfa,3,3);
//    NFA_add_transition_all(nfa,5,5);
//    NFA_add_transition_all(nfa,7,7);
//    NFA_add_transition_all(nfa,9,9);
//    NFA_add_transition_all(nfa,10,10);
//
//    NFA_add_transition(nfa,0,'a',1);
//    NFA_add_transition(nfa,0,'g',3);
//    NFA_add_transition(nfa,0,'h',5);
//    NFA_add_transition(nfa,0,'i',7);
//    NFA_add_transition(nfa,0,'n',9);
//    NFA_add_transition(nfa,1,'a',2);
//    NFA_set_accepting (nfa,2,1);
//    NFA_add_transition(nfa,3,'g',4);
//    NFA_set_accepting (nfa,4,1);
//    NFA_add_transition(nfa,5,'h',6);
//    NFA_set_accepting (nfa,6,1);
//    NFA_add_transition(nfa,7,'i',8);
//    NFA_set_accepting (nfa,8,1);
//    NFA_add_transition(nfa,9,'n',10);
//    NFA_add_transition(nfa,10,'n',11);
//    NFA_set_accepting (nfa,11,1);
//
//    //NFA_print(nfa);
//
////    printf("Accepting State: %d\n", NFA_execute(nfa, "aa"));
////    printf("Accepting State: %d\n", NFA_execute(nfa, "hiag"));
//    printf("Accepting State: %d\n", NFA_execute(nfa, "shitty fucking CSC173"));
//
//
//}





#endif