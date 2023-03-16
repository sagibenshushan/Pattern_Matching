
String Patern Matchng Algorithm
Authored by sagi ben shushan
209351147

==Description for ex1==
String pattern matching algorithms are very useful for several purposes, like simple search for a pattern in a text or looking for attacks with predefined signatures.
We will implement a dictionary-matching algorithm that locates elements of a finite set of strings (the "dictionary") within an input text. It matches all patterns "at once", 
so the complexity of the algorithm is linear in the length of the patterns plus the length of the searched text plus the number of output matches.
The algorithm matches multiple patterns simultaneously, by first constructing a Deterministic Finite Automaton (DFA) representing the patterns set, and then, 
with this DFA on its disposal, processing the text in a single pas.
The DFA is traversed starting from root. When the traversal goes through an accepting state, it indicates that some patterns are a suffix of the input; one of these 
patterns always corresponds to the label of the accepting state.
The DFA is presented as a tree which each node of this tree have some attributes like transitions list and output list.
All nodes use Doube linked list data base, for instance the output of the final state will be save in list.

==functions of ex1==

int pm_init(pm_t *) : 

Initializes the fsm parameters . 
 Returns 0 on success, -1 on failure. 


int pm_addstring(pm_t *,unsigned char *, size_t_ n) :
Adds a new string to the fsm, given that the string is of length n. 
Returns 0 on success, -1 on failure.


int pm_makeFSM(pm_t *) :
Finalizes construction by setting up the failrue transitions, as well as the goto transitions of the zerostate.
Returns 0 on success, -1 on failure.


int pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state) :
Set a transition arrow from this from_state, via a symbol, to a to_state. will be used in the pm_addstring and pm_makeFSM functions.
Returns 0 on success, -1 on failure.


pm_state_t* pm_goto_get(pm_state_t *state,unsigned char symbol) :
Will be used in pm_addstring, pm_makeFSM, pm_fsm_search, pm_destroy functions.
Returns the transition state.  If no such state exists, returns NULL.

dbllist_t* pm_fsm_search(pm_state_t *,unsigned char *,size_t_) :
Search for matches in a string of size n in the FSM.  If there are no matches return empty list.


void pm_destroy(pm_t *) :
Destroys the fsm, deallocating memory


==Program Files==
slist.c , pattern_matching.c 

==How to compile ex1?==
compile: gcc main.c slist.c pattern_matching.c -o main
run: ./main

===Input:==
user input, the input is patterns and text.

==Output:==
Each time you create new state in the FSM print: 
"Allocating state i\n"
Each time you create an edge for the goto function from state i to state j in the FSM 
print:
"I -> a -> j\n", where 'a' is the character that generates this edge. 
Each time you create an edge for the failure function from state i with depth > 1 to
any state j in the FSM print: 
"Setting f(i) = j\n"




