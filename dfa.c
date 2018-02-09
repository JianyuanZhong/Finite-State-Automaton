#ifndef _dfa_h
#define _dfa_h

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IntSet.h"


#define INITIAL_STATE 0
#define NO_TRANSITION -1


typedef struct DFA *DFA;
struct DFA{

    int nStates;
    int Initial_State;
    int **Transition_Function;
    IntSet *State_Match_Table;
    bool* Final_States;
    bool NFA_Translated;
    //IntSet dstSet;

}Dfa;


DFA DFA_new(int nstates){

    DFA newDFA = (DFA)malloc(sizeof(struct DFA));
    newDFA->nStates=nstates;

    newDFA->Initial_State = INITIAL_STATE;

    //newDFA->Final_States = IntSet_new();

    newDFA->Transition_Function = (int**) malloc(nstates * sizeof(int *));
    newDFA->NFA_Translated = false;

    for(int i=0; i<nstates; i++)
        newDFA->Transition_Function[i] = (int*) malloc(128 * sizeof(int));


    for(int i=0; i < nstates; i++){

        for(int j=0; j<128; j++){

            newDFA->Transition_Function[i][j] = NO_TRANSITION;
        }

    }

    newDFA->State_Match_Table = (IntSet *)malloc(nstates * sizeof(IntSet));
    for(int i=0; i< nstates; i++)
        newDFA ->State_Match_Table[i] = IntSet_new();

    newDFA-> Final_States = (bool*)malloc(nstates * sizeof(bool));
    for(int i=0; i<nstates; i++)
        newDFA->Final_States[i] = false;


    return newDFA;

}

void DFA_free(DFA dfa){
    for(int i =0 ;i<dfa->nStates; i++){
        free(dfa->Transition_Function[i]);

    }
    free(dfa->Transition_Function);
    free(dfa->State_Match_Table);

    free(dfa);
}

int DFA_get_size(DFA dfa){
    return dfa->nStates;
}

int DFA_get_transition(DFA dfa, int src, char sym){
    return dfa -> Transition_Function[src][(int)sym];
}

void DFA_set_transition_str(DFA dfa, int src, char *str, int dst){
    char *ptr = str;
    while(*ptr != '\0' ){

        dfa->Transition_Function[src][(int)*ptr] = dst;
        ptr++;
    }
}

void DFA_set_transition_all(DFA dfa, int src, int dst){
    for (int i=0 ; i<128 ;i++){
        dfa->Transition_Function[src][i] = dst;
    }

}

void DFA_set_transition(DFA dfa, int src, char sym, int dst){

    dfa->Transition_Function[src][(int)sym] = dst;

}

void DFA_set_accepting(DFA dfa, int state, bool value){

    dfa -> Final_States[state] = value;

}

bool DFA_get_accepting(DFA dfa, int state){

    return dfa->Final_States[state];

}

/**
 * Run the given DFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
bool DFA_execute(DFA dfa, char *input){
    int src, dst, sym;
    IntSet dstSet = IntSet_new();

    if(strlen(input)==0)
        return 0;

    if(dfa -> NFA_Translated){

        src = dfa->Initial_State;
        while (*input != '\0') {
            printf("input:%c\t",*input);
            printf("Src: ");
            IntSet_print(dstSet);
            char temp = *input;

            sym = (int) temp;
            dst = dfa->Transition_Function[src][sym];



            if (dst != NO_TRANSITION){
                src = dst;
                IntSet_union(dstSet, dfa->State_Match_Table[dst]);
            }

            printf("\tDst: ");

            if(dst != NO_TRANSITION)
                IntSet_print(dfa->State_Match_Table[dst]);

            printf("\tStates Visted: ");
            IntSet_print(dstSet);
            printf("\n");

            input++;

        }


        return dfa->Final_States[dst];

    }else {

        src = dfa->Initial_State;
        while (*input != '\0') {
            char temp = *input;
            sym = (int) temp;
            dst = dfa->Transition_Function[src][sym];
            //IntSet_add(dstSet,dst);
            if (dst != NO_TRANSITION)
                src = dst;

            input++;

        }

        //IntSet_print(dfa->State_Match_Table[dst]);
        return dfa->Final_States[dst];
    }

    return  false;
}



/**
 * Print the given DFA to System.out.
 */
void DFA_print(DFA dfa){
    for(int i=0; i<dfa->nStates; i++){
        printf("$state %d$:\t",i);
        for(int j = 0; j<128; j++){
            if(dfa ->Transition_Function[i][j] != NO_TRANSITION){
                printf("(sym:%c,transition:%d),\t",(char)j,dfa ->Transition_Function[i][j]);
            }
        }
        printf("\n");

    }

}

IntSet* DFA_get_State_Match_Table(DFA dfa){
    return dfa -> State_Match_Table;
}

int** DFA_get_Transition_Function(DFA dfa){
    return  dfa->Transition_Function;
}

int DFA_get_State_Match(DFA dfa, IntSet set){
    for(int i=0; i<dfa->nStates; i++){
        if(IntSet_equals(dfa -> State_Match_Table[i],set))
            return i;
    }
    return -1;
}

IntSet DFA_get_State_Set(DFA dfa, int i ){
    return dfa -> State_Match_Table[i];
}

void DFA_set_State_Match(DFA dfa, IntSet state, int i){
    dfa -> State_Match_Table[i] = state;
}

void DFA_Set_NFA_Translated(DFA dfa, bool value){
    dfa -> NFA_Translated = value;
}






//int main(){
//    DFA p1 = DFA_new(6);
//    printf("%d",p1->nStates);
//    DFA_set_transition_all(p1,0,5);
//    DFA_set_accepting(p1,0,1);
//    DFA_set_transition(p1,0,'a',1);
//    DFA_set_transition(p1,0,'1',3);
//    DFA_set_transition(p1,0,'0',4);
//    DFA_set_transition(p1,1,'b',2);
//    DFA_set_transition_all(p1,2,NO_TRANSITION);
//    DFA_set_accepting(p1,2,1);
//    DFA_set_transition(p1,3,'1',0);
//    DFA_set_transition(p1,4,'0',0);
//
//    DFA_print(p1);
//
//    printf("%d\n",DFA_execute(p1,"ab"));
//    printf("%d\n",DFA_execute(p1,"jzhong"));
//};

#endif