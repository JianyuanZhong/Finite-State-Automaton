# Finite-State-Automaton
How to run:

Go to the directory of the project in Terminal, and enter commands:

$...: make auto

$...: ./auto



Accomplishment:

Part 1: Created a DFA data Structure which will work for all the DFA in generall. And created 5 instances as require by the problem.

(a) Exactly the string ab
(b) Any string that starts with the characters ab
(c) Binary input with an even number of 1’s
(d) Binary input with an even number of both 0’s and 1’s
(e) strings that end with .c

Part 2: Created a NFA data Structure which will work for all the DFA in generall. And created 3 instancs as required by the problem.

(a) Strings ending in man
(b) Strings with more than one a, g, h, i, o, s, t, or w, or more than two n’s.
This automaton is described in FOCS as part of a project to find words that
can be made from the letters of the word “Washington” (so-called “partial
anagrams”). In this automaton, acceptance means that the input string is not
a partial anagram of washington, because it has too many of some letter.
(c) String ending with 0

Part 3: Successfully implement a function to transfer any NFA to DFA! Even the "washington" problem can be done in less than 20 second.

1, The program name is auto

2, Standard i/o to allow user to create instances of dfa and nfa. Beside, User can easily test the whole project with standard i/o;

3, REPL functions for testing the project


How to build the project:

Part 1: as the starting point of the project, part one is relatively easy. I made it by just following the header file "dfa.h" provided in the code bundle. 

Part 2: Part 2 is a little trickier, but just with a little thought, I changed the transition function to IntSet** to denode the transition to possible states by one input and do the union for executing. Afer doing so, Part 2 was solved.

Part 3: This is definitly the most difficult part of the whole project. Also, it took me a massive amount of time to think about the data struction in c and to go through the misable debuging process. Nevertheless, my effort has eventual paid off and even made it to translate the "washington" NFA to a equivalent DFA with 4096 with less than 20 seconds! For building the function, I thought of the nature of an NFA. It is actually a gragh whose nodes are sets of transitions. In order to walk through the whole grah and gets all the possible node, I used the BFS method, by building a queue and find all the possible sets of transitions stating at {0}.


How to test the project:

Inorder to make it easier to grade this project, I made three REPL fuctions in response to three parts. 

For part1: There are five choices Part1_a, Part1_b, Part1_c, Part1_d, Part1_e. Grader can follow the instructions to to create instances above and test them by input on the terminal

For part2: There are three choices Part2_a, Part2_b, Part2_c. Grader can follow the instructions to to create instances above and test them by input on the terminal.

For part3: There are three choices Part2_a, Part2_b, Part2_c. After Graders make a choice, they will have two options: "Compare": to execute two FAs and print a step by step output to demonstrate two FA's equivalence. "Print" to print out the transition table of two FAs.







