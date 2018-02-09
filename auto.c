#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IntSet.h"
#include "dfa.h"
#include "nfa.h"
#include "LinkedList.h"

#define INITIAL_STATE 0
#define NO_TRANSITION -1

bool Auto_stateExist(LinkedList *list, IntSet set) {
    LinkedListIterator *Litr = LinkedList_iterator(list);
    while (LinkedListIterator_has_next(Litr)) {
        IntSet temp = LinkedListIterator_next(Litr);
        if (IntSet_equals(temp, set)) {

            return true;

        }
    }

    free(Litr);
    return false;
}


DFA Auto_Translator(NFA nfa) {
    int nstates;
    LinkedList *queue, *container;
    IntSetIterator itr;

    queue = LinkedList_new();
    container = LinkedList_new();
    nstates = 1;
    LinkedList_add_at_end(container, IntSet_new());



    printf("Translating NFA to DFA....Please wait....\n");
    printf("\nSearching possible Transition Set... \n");


    for (int i = 0; i < 128; i++) {
        IntSet states = NFA_get_transitions(nfa, INITIAL_STATE, (char) i);
        //printf("%d\n",i);
        if (!IntSet_is_empty(states))
            LinkedList_add_at_end(queue, states);
    }

    while (!LinkedList_is_empty(queue)) {

        IntSet top = (IntSet) LinkedList_pop(queue);
        if (!Auto_stateExist(container, top)) {

            LinkedList_add_at_end(container, top);
            nstates++;


//            LinkedListIterator *itrr = LinkedList_iterator(Useful_Input);
//            while (LinkedListIterator_has_next(itrr)) {
//                int i = (int) (*(char *) LinkedListIterator_next(itrr));
            for (int i = 0; i < 128; i++) {

                IntSet dstSet = IntSet_new();
                itr = IntSet_iterator(top);
                while (IntSetIterator_has_next(itr)) {

                    int src = IntSetIterator_next(itr);
                    IntSet_union(dstSet, NFA_get_transitions(nfa, src, (char) i));

                }
                if (!IntSet_is_empty(dstSet) /*&& !IntSet_contains_all(top,dstSet)*/) {
                    if (!Auto_stateExist(container, dstSet)) {
                        LinkedList_add_at_end(queue, dstSet);
                    }

                }
                free(itr);
            }
            //free(itrr);/home/azhong/CSC173
        }

    }
    printf("Completed!\n");
    printf("\nHow many states....... %d\n", nstates);
    printf("Building up the DFA......\n");

    DFA dfa = DFA_new(nstates);
    DFA_Set_NFA_Translated(dfa, true);

    LinkedListIterator *Litr = LinkedList_iterator(container);


    IntSet Final_States_NFA = NFA_get_Final_States(nfa);
    for (int i = 0; i < nstates; i++) {

        IntSet tp = (IntSet) LinkedListIterator_next(Litr);
        itr = IntSet_iterator(tp);
        while (IntSetIterator_has_next(itr)) {
            int st = IntSetIterator_next(itr);
            if (IntSet_contains(Final_States_NFA, st))
                DFA_set_accepting(dfa, i, true);
        }

        DFA_set_State_Match(dfa, tp, i);

    }



    //IntSet *table_Match = DFA_get_State_Match_Table(dfa);
    //int **table_Transition = DFA_get_Transition_Function(dfa);
    int src;
    for (int i = 0; i < nstates; i++) {
        for (int j = 0; j < 128; j++) {

            IntSet dst_Set = IntSet_new();

            if (i == 0) {
                dst_Set = NFA_get_transitions(nfa, INITIAL_STATE, (char) j);
            } else {
                itr = IntSet_iterator(DFA_get_State_Set(dfa, i));
                while (IntSetIterator_has_next(itr)) {

                    src = IntSetIterator_next(itr);
                    IntSet dst = NFA_get_transitions(nfa, src, (char) j);
                    IntSet_union(dst_Set, dst);

                }
                free(itr);
            }


            //IntSet_print(dst_Set);

            if (!IntSet_is_empty(dst_Set)) {

                for (int k = 0; k < nstates; k++)
                    if (IntSet_equals(dst_Set, DFA_get_State_Set(dfa, k)))
                        DFA_set_transition(dfa, i, (char) j, k);
            } else {
                DFA_set_transition(dfa, i, (char) j, -1);
            }
        }


    }


    printf("Completed!\n");
    printf("Tranlation Completed!\n");

    return dfa;


}

DFA Auto_Create_DFA_Part1_a() {
    DFA dfa = DFA_new(3);
    DFA_set_transition(dfa, 0, 'a', 1);
    DFA_set_transition(dfa, 1, 'b', 2);
    DFA_set_accepting(dfa, 2, true);
    return dfa;
}

DFA Auto_Create_DFA_Part1_b() {
    DFA dfa = DFA_new(3);
    DFA_set_transition(dfa, 0, 'a', 1);
    DFA_set_transition(dfa, 1, 'b', 2);
    DFA_set_transition_all(dfa, 2, 2);
    DFA_set_accepting(dfa, 2, true);
    return dfa;
}

DFA Auto_Create_DFA_Part1_c() {
    DFA dfa = DFA_new(3);
    DFA_set_transition(dfa, 0, '1', 1);
    DFA_set_transition(dfa, 0, '0', 0);
    DFA_set_transition(dfa, 1, '1', 2);
    DFA_set_transition(dfa, 1, '0', 1);
    DFA_set_transition(dfa, 2, '1', 1);
    DFA_set_transition(dfa, 2, '0', 2);
    DFA_set_accepting(dfa, 2, true);
    DFA_set_accepting(dfa, 0, true);
    return dfa;
}

DFA Auto_Create_DFA_Part1_d() {
    DFA dfa = DFA_new(3);
    DFA_set_transition(dfa, 0, '0', 1);
    DFA_set_transition(dfa, 0, '1', 2);
    DFA_set_transition(dfa, 1, '0', 0);
    DFA_set_transition(dfa, 1, '1', 2);
    DFA_set_transition(dfa, 2, '0', 1);
    DFA_set_transition(dfa, 2, '1', 0);
    DFA_set_accepting(dfa, 0, true);
    return dfa;
}

DFA Auto_Create_DFA_Part1_e() {
    DFA dfa = DFA_new(3);
    DFA_set_transition_all(dfa, 0, 0);
    DFA_set_transition(dfa, 0, '.', 1);
    DFA_set_transition(dfa, 1, 'c', 2);
    DFA_set_accepting(dfa, 2, true);
    return dfa;
}

DFA Auto_Create_DFA_Part1_In_One() {
    DFA dfa = DFA_new(6);
    DFA_set_transition_all(dfa, 0, 5);
    DFA_set_accepting(dfa, 0, 1);
    DFA_set_transition(dfa, 0, 'a', 1);
    DFA_set_transition(dfa, 0, '1', 3);
    DFA_set_transition(dfa, 0, '0', 4);
    DFA_set_transition(dfa, 1, 'b', 2);
    DFA_set_transition_all(dfa, 2, NO_TRANSITION);
    DFA_set_accepting(dfa, 2, 1);
    DFA_set_transition(dfa, 3, '1', 0);
    DFA_set_transition(dfa, 4, '0', 0);
    //DFA_set_transition_str(dfa, 0, "C language is tough", 6);
    return dfa;
}

NFA Auto_Create_NFA_Part2_a() {
    NFA nfa = NFA_new(4);
    NFA_add_transition_all(nfa, 0, 0);
    //NFA_add_transition_all(nfa, 3, 0);
    NFA_add_transition(nfa, 0, 'm', 1);
    NFA_add_transition(nfa, 1, 'm', 1);
    NFA_add_transition(nfa, 1, 'a', 2);
    NFA_add_transition(nfa, 2, 'n', 3);

    NFA_set_accepting(nfa, 3, true);
    return nfa;
}

NFA Auto_Create_NFA_Part2_b() {
    NFA nfa = NFA_new(20);
    NFA_add_transition_str(nfa, 0, "washington", 0);
    NFA_add_transition_str(nfa, 1, "washington", 1);
    NFA_add_transition_str(nfa, 3, "washington", 3);
    NFA_add_transition_str(nfa, 5, "washington", 5);
    NFA_add_transition_str(nfa, 7, "washington", 7);
    NFA_add_transition_str(nfa, 9, "washington", 9);
    NFA_add_transition_str(nfa, 10, "washington", 10);
    NFA_add_transition_str(nfa, 12, "washington", 12);
    NFA_add_transition_str(nfa, 14, "washington", 14);
    NFA_add_transition_str(nfa, 16, "washington", 16);
    NFA_add_transition_str(nfa, 18, "washington", 18);

    NFA_add_transition(nfa, 0, 'a', 1);
    NFA_add_transition(nfa, 0, 'g', 3);
    NFA_add_transition(nfa, 0, 'h', 5);
    NFA_add_transition(nfa, 0, 'i', 7);
    NFA_add_transition(nfa, 0, 'n', 9);
    NFA_add_transition(nfa, 0, 'o', 12);
    NFA_add_transition(nfa, 0, 's', 14);
    NFA_add_transition(nfa, 0, 't', 16);
    NFA_add_transition(nfa, 0, 'w', 18);
    NFA_add_transition(nfa, 1, 'a', 2);
    NFA_add_transition(nfa, 3, 'g', 4);
    NFA_add_transition(nfa, 5, 'h', 6);
    NFA_add_transition(nfa, 7, 'i', 8);
    NFA_add_transition(nfa, 9, 'n', 10);
    NFA_add_transition(nfa, 10, 'n', 11);
    NFA_add_transition(nfa, 12, 'o', 13);
    NFA_add_transition(nfa, 14, 's', 15);
    NFA_add_transition(nfa, 16, 't', 17);
    NFA_add_transition(nfa, 18, 'w', 19);


    NFA_set_accepting(nfa, 2, true);
    NFA_set_accepting(nfa, 4, true);
    NFA_set_accepting(nfa, 6, true);
    NFA_set_accepting(nfa, 8, true);
    NFA_set_accepting(nfa, 11, true);
    NFA_set_accepting(nfa, 13, true);
    NFA_set_accepting(nfa, 15, true);
    NFA_set_accepting(nfa, 17, true);
    NFA_set_accepting(nfa, 19, true);

    return nfa;
}

NFA Auto_Create_NFA_Part2_c() {
    NFA nfa = NFA_new(2);
    NFA_add_transition_all(nfa, 0, 0);
    NFA_add_transition(nfa, 0, '0', 1);
    NFA_set_accepting(nfa, 1, true);
    return nfa;

}

NFA Auto_Create_NFA_Part2_In_One() {
    NFA nfa = NFA_new(23);
    NFA_add_transition_all(nfa, 0, 0);
    NFA_add_transition_all(nfa, 1, 1);
    NFA_add_transition_all(nfa, 3, 3);
    NFA_add_transition_all(nfa, 5, 5);
    NFA_add_transition_all(nfa, 7, 7);
    NFA_add_transition_all(nfa, 9, 9);
    NFA_add_transition_all(nfa, 10, 10);
    NFA_add_transition_all(nfa, 12, 12);
    NFA_add_transition_all(nfa, 14, 14);
    NFA_add_transition_all(nfa, 16, 16);
    NFA_add_transition_all(nfa, 18, 18);

    NFA_add_transition_str(nfa, 0, "washington", 0);
    NFA_add_transition_str(nfa, 1, "washington", 1);
    NFA_add_transition_str(nfa, 3, "washington", 3);
    NFA_add_transition_str(nfa, 5, "washington", 5);
    NFA_add_transition_str(nfa, 7, "washington", 7);
    NFA_add_transition_str(nfa, 9, "washington", 9);
    NFA_add_transition_str(nfa, 10, "washington", 10);
    NFA_add_transition_str(nfa, 12, "washington", 12);
    NFA_add_transition_str(nfa, 14, "washington", 14);
    NFA_add_transition_str(nfa, 16, "washington", 16);
    NFA_add_transition_str(nfa, 18, "washington", 18);

    NFA_add_transition(nfa, 0, 'a', 1);
    NFA_add_transition(nfa, 0, 'g', 3);
    NFA_add_transition(nfa, 0, 'h', 5);
    NFA_add_transition(nfa, 0, 'i', 7);
    NFA_add_transition(nfa, 0, 'n', 9);
    NFA_add_transition(nfa, 0, 'o', 12);
    NFA_add_transition(nfa, 0, 's', 14);
    NFA_add_transition(nfa, 0, 't', 16);
    NFA_add_transition(nfa, 0, 'w', 18);
    NFA_add_transition(nfa, 0, 'm', 20);
    NFA_add_transition(nfa, 1, 'a', 2);
    NFA_add_transition(nfa, 3, 'g', 4);
    NFA_add_transition(nfa, 5, 'h', 6);
    NFA_add_transition(nfa, 7, 'i', 8);
    NFA_add_transition(nfa, 9, 'n', 10);
    NFA_add_transition(nfa, 10, 'n', 11);
    NFA_add_transition(nfa, 12, 'o', 13);
    NFA_add_transition(nfa, 14, 's', 15);
    NFA_add_transition(nfa, 15, 't', 17);
    NFA_add_transition(nfa, 18, 'w', 19);
    NFA_add_transition(nfa, 20, 'm', 20);
    NFA_add_transition(nfa, 20, 'a', 21);
    NFA_add_transition(nfa, 21, 'n', 22);

    NFA_set_accepting(nfa, 2, true);
    NFA_set_accepting(nfa, 4, true);
    NFA_set_accepting(nfa, 6, true);
    NFA_set_accepting(nfa, 8, true);
    NFA_set_accepting(nfa, 11, true);
    NFA_set_accepting(nfa, 13, true);
    NFA_set_accepting(nfa, 15, true);
    NFA_set_accepting(nfa, 17, true);
    NFA_set_accepting(nfa, 19, true);
    NFA_set_accepting(nfa, 22, true);

    return nfa;

}

void Auto_Compare(NFA nfa, DFA dfa){
    char User_Input[100];
    printf("Please type in a string: ");
    scanf("%s", User_Input);

    printf("Runing the original nfa: \n");
    bool r1 = NFA_execute(nfa,User_Input);
    printf("Accepting state: %d\n",r1);

    printf("\n\n\nRunning the translated dfa: \n");
    bool r2 = DFA_execute(dfa,User_Input);
    printf("Accepting state: %d\n",r2);

    printf("\nThus, the translated DFA is equivalent to the original NFA\n");

}

void Auto_REPL_DFA() {
    DFA dfa;
    bool exit = false;
    printf("\n****************CSC173 Project1: Finite Automaton**************. \nAuthor: Jianyuan Zhong\nAll the code are implement by myself.");
    printf("\n\nTest Case: DFA\nPlease Choose an instance of DFA to test: \nPart1_a\nPart1_b\nPart1_c\nPart1_d\nPart1_e\n");

    char User_Input[100];
    while (!exit) {
        printf("\nChoose a DFA: ");
        scanf("%s", User_Input);
        if (strcmp("Part1_a", User_Input) == 0) {
            dfa = Auto_Create_DFA_Part1_a();
            while (strcmp("done", User_Input)) {
                printf("Recognize Exactly the string ab\n");
                printf("(enter done to stop testing this instance)\n");
                printf("Please type in a string: ");
                scanf("%s", User_Input);
                printf("Accepting State: %d\n", DFA_execute(dfa, User_Input));
            }
        }
        if (strcmp("Part1_b", User_Input) == 0) {
            dfa = Auto_Create_DFA_Part1_b();
            while (strcmp("done", User_Input)) {
                printf("Recognize strings that begins with ab\n");
                printf("(enter done to stop testing this instance)\n");
                printf("Please type in a string: ");
                scanf("%s", User_Input);
                printf("Accepting State: %d\n", DFA_execute(dfa, User_Input));
            }
        }
        if (strcmp("Part1_c", User_Input) == 0) {
            dfa = Auto_Create_DFA_Part1_c();
            while (strcmp("done", User_Input)) {
                printf("Recognize binary input with even number of 1's\n");
                printf("(enter done to stop testing this instance)\n");
                printf("Please type in a string: ");
                scanf("%s", User_Input);
                printf("Accepting State: %d\n", DFA_execute(dfa, User_Input));
            }
        }
        if (strcmp("Part1_d", User_Input) == 0) {
            dfa = Auto_Create_DFA_Part1_d();
            while (strcmp("done", User_Input)) {
                printf("Recognize binary input with even number of both 0's and 1's\n");
                printf("(enter done to stop testing this instance)\n");
                printf("Please type in a string: ");
                scanf("%s", User_Input);
                printf("Accepting State: %d\n", DFA_execute(dfa, User_Input));
            }
        }
        if (strcmp("Part1_e", User_Input) == 0) {
            dfa = Auto_Create_DFA_Part1_e();
            while (strcmp("done", User_Input)) {
                printf("Recognize strings that end with .c\n");
                printf("(enter done to stop testing this instance)\n");
                printf("Please type in a string: ");
                scanf("%s", User_Input);
                printf("Accepting State: %d\n", DFA_execute(dfa, User_Input));
            }
        }
        free(dfa);
        printf("Do you want to continue testing?(yes/no)");
        scanf("%s", User_Input);
        if (strcmp("no", User_Input) == 0)
            exit = true;
        else
            exit = false;
    }
};

void Auto_REPL_NFA() {
    NFA nfa;
    bool exit = false;
    printf("\n****************CSC173 Project1: Finite Automaton**************. \nAuthor: Jianyuan Zhong\nAll the code are implement by myself.");
    printf("\n\nTest Case: NFA\nPlease Choose an instance of NFA to test: \nPart2_a\nPart2_b\nPart2_c\n");

    char User_Input[100];
    while (!exit) {
        printf("\nChoose a NFA: ");
        scanf("%s", User_Input);
        if (strcmp("Part2_a", User_Input) == 0) {
            nfa = Auto_Create_NFA_Part2_a();
            while (strcmp("done", User_Input)) {
                printf("Recognize string ending in man\n");
                printf("(enter done to stop testing this instance)\n");
                printf("Please type in a string: ");
                scanf("%s", User_Input);
                printf("Accepting State: %d\n", NFA_execute(nfa, User_Input));
            }
        }
        if (strcmp("Part2_b", User_Input) == 0) {
            nfa = Auto_Create_NFA_Part2_b();
            while (strcmp("done", User_Input)) {
                printf("Recognize strings that are not partial anagram of washington\n");
                printf("(enter done to stop testing this instance)\n");
                printf("Please type in a string: ");
                scanf("%s", User_Input);
                printf("Accepting State: %d\n", NFA_execute(nfa, User_Input));
            }
        }
        if (strcmp("Part2_c", User_Input) == 0) {
            nfa = Auto_Create_NFA_Part2_c();
            while (strcmp("done", User_Input)) {
                printf("Recognize strings that end with 0\n");
                printf("Please type in a string: ");
                scanf("%s", User_Input);
                printf("Accepting State: %d\n", NFA_execute(nfa, User_Input));
            }
        }

        free(nfa);
        printf("Do you want to continue testing?(yes/no)");
        scanf("%s", User_Input);
        if (strcmp("no", User_Input) == 0)
            exit = true;
        else
            exit = false;
    }
}

void Auto_REPL_NFA_to_DFA() {
    NFA nfa;
    DFA dfa;
    bool exit = false;
    printf("\n****************CSC173 Project1: Finite Automaton**************. \nAuthor: Jianyuan Zhong\nAll the code are implement by myself.");
    printf("\n\nTest Case: NFA to DFA\nPlease Choose an instance of NFA to test: \nPart2_a\nPart2_b\nPart2_c\n");

    char User_Input[100];
    while (!exit) {
        printf("\nChoose a NFA: ");
        scanf("%s", User_Input);
        if (strcmp("Part2_a", User_Input) == 0) {
            nfa = Auto_Create_NFA_Part2_a();

            printf("Recognize string ending in man\n");
            dfa = Auto_Translator(nfa);

        }
        if (strcmp("Part2_b", User_Input) == 0) {
            nfa = Auto_Create_NFA_Part2_b();

            printf("Recognize strings that are not partial anagram of washington\n");
            dfa = Auto_Translator(nfa);

        }
        if (strcmp("Part2_c", User_Input) == 0) {
            nfa = Auto_Create_NFA_Part2_c();

            printf("Recognize strings that end with 0\n");

            dfa = Auto_Translator(nfa);

        }

        printf("\nTo test the translation, There are two choice:\n\nCompare: Type in an input, it will run step by step to show the equalvilent in language.\nPrint: print our the transition table of two FAs\n" );
        printf("\nYour Choice: ");
        scanf("%s", User_Input);
        if(strcmp("Compare", User_Input) == 0){
            Auto_Compare(nfa,dfa);
        }
        if(strcmp("Print",User_Input)==0){
            NFA_print(nfa);
            printf("\n\n\n\n");
            DFA_print(dfa);
        }



        free(nfa);
        free(dfa);

        printf("Do you want to continue testing?(yes/no)");
        scanf("%s", User_Input);
        if (strcmp("no", User_Input) == 0)
            exit = true;
        else
            exit = false;
    }
}


int main(int argc, char **argv) {

//    NFA nfa = Auto_Create_NFA_Part2_b();
//    DFA dfa = Auto_Translator(nfa);
//    //DFA_print(dfa);
//    dfa=DFA_new(6);
//    free(dfa);
    //dfa=0;

    //DFA dfa = Auto_Create_DFA_Part1();

    //printf("Accepting State: %d\n", DFA_execute(dfa, ""));


//    printf("Accepting State: %d\n", DFA_execute(dfa, "shitty fucking CSC173"));
//    printf("Accepting State: %d\n", DFA_execute(dfa, "notman"));

//    LinkedList* ui = Auto_Train_Input(nfa);
//    LinkedListIterator *itr=LinkedList_iterator(ui);
//    while(LinkedListIterator_has_next(itr)){
//        printf("%c\n",*(char*)LinkedListIterator_next(itr));
//    }

    Auto_REPL_DFA();
    Auto_REPL_NFA();
    Auto_REPL_NFA_to_DFA();

    printf("\n\n\n\nThank you for your patient! Have a nice day!\n\n\n");

}

