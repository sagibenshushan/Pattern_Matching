#include "pattern_matching.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


int pm_init(pm_t* p){
    //create the root
    p->zerostate = (pm_state_t*)malloc(sizeof (pm_state_t));    //creation
    if(p == NULL){
        perror("MALLOC FAIL");
        return -1;
    }
    //initialize
    p->zerostate->id = 0;
    p->zerostate->depth = 0;
    p->zerostate->_transitions = (dbllist_t*)malloc(sizeof(dbllist_t)); //creation
    if(p == NULL){
        perror("MALLOC FAIL");
        return -1;
    }
    dbllist_init(p->zerostate->_transitions);
    p->zerostate->output = (dbllist_t*)malloc(sizeof(dbllist_t)); //creation
    if(p == NULL){
        perror("MALLOC FAIL");
        return -1;
    }
    dbllist_init(p->zerostate->output);
    p->zerostate->fail = NULL;
    p->newstate = 1;

    return 0;
}


int pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state){
    pm_labeled_edge_t* labelE =(pm_labeled_edge_t*)malloc(sizeof (pm_labeled_edge_t));     //create new edge
    if(labelE == NULL)
        return -1;

    labelE->label = symbol;     //put the char on the edge
    labelE->state = to_state;   //init the state to to_state
    dbllist_append(from_state->_transitions, labelE);   //add to the neighbors' list
    printf("%d -> %c -> %d \n", from_state->id,symbol, to_state->id);   //as required
    return 0;

}


pm_state_t* pm_goto_get(pm_state_t *state,unsigned char symbol){
    if(state == NULL)
        return NULL;
    dbllist_node_t* node = dbllist_head(state->_transitions);  // create pointer to the head of the transition list of the input state
    while(node!= NULL){
        pm_labeled_edge_t* labelE = dbllist_data(node);             //put the data on the edge, pointer
        if(labelE->label == symbol){                                 //check which state have edge in common
            return labelE->state;                                    //return the state we are looking for
        }
        node = dbllist_next(node);                            //updating node, ++
    }
    return NULL;
}


int pm_addstring(pm_t *p,unsigned char* c, _size_t n) {
    pm_state_t *stat = p->zerostate;     //stat pointer to the root
    int i = 0;
    while (pm_goto_get(stat, c[i]) != NULL) {   //there is an edge for that symbol in the FSM
        stat = pm_goto_get(stat, c[i]);
        i++;
    }
    for (; i < n; i++) {
        pm_state_t *s = (pm_state_t *) malloc(sizeof(pm_state_t)); //creation
        if (s == NULL) {
            return -1;
        }
        s->id = p->newstate;
        p->newstate++;    //addition
        s->depth = stat->depth + 1;
        s->fail = NULL;
        s->_transitions = (dbllist_t *) malloc(sizeof(dbllist_t));   //creation
        dbllist_init(s->_transitions); //init
        if (s == NULL) {
            perror("MALLOC FAIL");
            return -1;
        }
        s->output = (dbllist_t *) malloc(sizeof(dbllist_t));
        if (s == NULL) {
            perror("MALLOC FAIL");
            return -1;
        }
        dbllist_init(s->output);  //init
        printf("Allocating state %d\n", s->id);
        pm_goto_set(stat, c[i], s);   //create edge between stat to s
        stat = s;  //updating
    }
    dbllist_append(stat->output, c);   //accepting state
    return 0;
}


int pm_makeFSM(pm_t *pm) {
    dbllist_t *Q = (dbllist_t *) malloc(sizeof(dbllist_t));
    if (Q == NULL) {
        return -1;
    }
    dbllist_init(Q);

    dbllist_node_t* node = dbllist_head(pm->zerostate->_transitions);  //the head of list of the root transitions
    for(; node != NULL; node = dbllist_next(node)) {    //step over to the next in transitions list
        pm_state_t *state = ((pm_labeled_edge_t *) dbllist_data(node))->state;
        if (dbllist_append(Q, state) == -1) {    //success maked append
            return -1;
        }
        state->fail = pm->zerostate;        //the setting fail of all nodes in depth 1
        printf("Setting f(%d) = %d\n", state->id, state->fail->id);  //asked
    }
        int flag = 0;
        while (dbllist_size(Q) > 0){
            dbllist_node_t *n = dbllist_head(Q); // make pointer to the head of the list of states
            pm_state_t *st = dbllist_data(n);   //st = data of the head of Q
            dbllist_remove(Q, n, DBLLIST_LEAVE_DATA); //remove from Q list
            dbllist_node_t *no1 = dbllist_head(st->_transitions);

            while (no1 != NULL) {    //there is edge
                pm_labeled_edge_t *temp1 = dbllist_data(no1);  // the edge that connected to state st
                dbllist_append(Q, temp1->state);      //add the state the came out from st
                pm_state_t *sta = st->fail;   //creation state fail

                while (sta != NULL){
                    dbllist_node_t *no2 = dbllist_head(sta->_transitions);
                    while (no2 != NULL) {    //there is edge
                        pm_labeled_edge_t *temp2 = dbllist_data(no2);  // the edge that connected to state st
                        if (temp1->label == temp2->label) {
                            flag = 1; //found
                            temp1->state->fail = temp2->state;
                            break;
                        }
                        no2 = dbllist_next(no2);
                    }
                    if(flag == 1){
                        break;
                    }
                    sta = sta->fail; //one step back in order to find the right state
                }
                if(flag == 0){       //no matching found
                    temp1->state->fail = pm->zerostate;
                }
                //fill the output list
                dbllist_t *list1 = temp1->state->fail->output;        //took the list of the outputs fails
                if(list1 != NULL){
                    dbllist_node_t* no4 = dbllist_head(list1);
                    while(no4 != NULL){
                        dbllist_append(temp1->state->output,dbllist_data(no4));    //add the data of no4 to output list of temp1
                        no4 = dbllist_next(no4);
                    }
                }
                printf("Setting f(%d) = %d\n", temp1->state->id, temp1->state->fail->id);  //asked
                flag = 0;
                no1 = dbllist_next(no1);
            }
        }
    dbllist_destroy(Q, DBLLIST_FREE_DATA);
    return 0;
}


dbllist_t* pm_fsm_search(pm_state_t * state,unsigned char * c,_size_t size){
    if(state == NULL) {
        return NULL;
    }
    dbllist_t *matchList = (dbllist_t *) malloc(sizeof(dbllist_t));   //lists of match
    dbllist_init(matchList);
    int j;
    for(j = 0; j < size; j++){
        while(pm_goto_get(state,c[j]) == NULL){
            if(state->id == 0){
                break;
            }
            state = state->fail;
        }
        if(pm_goto_get(state, c[j]) != NULL){
            state = pm_goto_get(state, c[j]);
        }

        if(state->output != NULL && state->output->size > 0 ){
            dbllist_node_t* node = dbllist_head(state->output);          //pointer to thr head of output list
            while(node != NULL){
                pm_match_t* match =(pm_match_t*) malloc(sizeof (pm_match_t));
                if(match == NULL){
                    return NULL;
                }
                match->pattern = dbllist_data(node);
                match->start_pos = j - strlen(match->pattern) + 1;
                match->end_pos = j;
                match->fstate = state;


                dbllist_append(matchList,match);       //push match to the match list
                node = dbllist_next(node);           //next node in output list

            }
        }
    }
    return matchList;
}


void pm_destroy(pm_t * pm) {
    dbllist_t *Q = (dbllist_t *) malloc(sizeof(dbllist_t));
    dbllist_init(Q);
    dbllist_node_t *node = dbllist_head(pm->zerostate->_transitions);  //the head of list of the root transitions
    dbllist_append(Q, pm->zerostate);           //add the root to the list
    while (dbllist_size(Q) > 0) {
        dbllist_node_t *n = dbllist_head(Q); // make pointer to the head of the list of states
        pm_state_t *st = dbllist_data(n);   //st = data of the head of Q
        dbllist_remove(Q ,n , DBLLIST_LEAVE_DATA);     //dequeue
        dbllist_node_t *no1 = dbllist_head(st->_transitions);
        while (no1 != NULL) {    //there is edge
            pm_state_t *s = ((pm_labeled_edge_t *) dbllist_data(no1))->state;
            dbllist_append(Q,s);    //add the sons- enqueue sons
            no1 = dbllist_next(no1);
        }
        dbllist_destroy(st->output, DBLLIST_LEAVE_DATA);   //frees
        dbllist_destroy(st->_transitions, DBLLIST_FREE_DATA);
        free(st);
    }
    dbllist_destroy(Q,DBLLIST_FREE_DATA);
}











