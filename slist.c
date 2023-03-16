#include "slist.h"
#include "stdlib.h"

void dbllist_init(dbllist_t *list){
    dbllist_head(list) = NULL;
    dbllist_tail(list) = NULL;
    dbllist_size(list) = 0;
}

void dbllist_destroy(dbllist_t* list,dbllist_destroy_t des){
    dbllist_node_t* temp;
    temp = dbllist_head(list) ; //pointer to the head of the list
    while(dbllist_head(list) != NULL){
        temp = dbllist_head(list) ;
       dbllist_remove(list, temp,des);
    }
}

int dbllist_append(dbllist_t *list,void* data) {
    dbllist_node_t *new_node = (dbllist_node_t *) malloc(sizeof(dbllist_node_t));  //create new node
    dbllist_next(new_node) = NULL; // INIT
    dbllist_prev(new_node) = NULL;
    dbllist_data(new_node) = data; //put in the data
    if (dbllist_head(list) == NULL) {
        dbllist_prev(new_node) = NULL;
        dbllist_head(list) = new_node;
        dbllist_tail(list) = new_node;
        list->size++;
        return 0;
    }
    dbllist_next(dbllist_tail(list)) = new_node;
    dbllist_prev(new_node) = dbllist_tail(list);  // Make prev_node as previous of new_node0
    dbllist_next(new_node) = NULL;
    dbllist_tail(list) = new_node;   //the tail of the list pointer to the new node
    list->size++;
    return 0;
}

int dbllist_prepend(dbllist_t* list,void* data) {
    dbllist_node_t* new_node = ( dbllist_node_t*)malloc(sizeof(dbllist_node_t));  //create new node
    if (new_node == NULL || list == NULL) {
        return -1;    //failed
    }
    dbllist_data(new_node) = data; //put in the data
    dbllist_prev(new_node) = NULL;
    dbllist_next(new_node)=NULL;
    list->size++;
    if (dbllist_head(list) == NULL) {      //put the tail and the head on the first node
        dbllist_tail(list) = new_node;
        dbllist_head(list) = new_node;
        return 0;
    }
    dbllist_next(new_node) = dbllist_head(list);
    dbllist_prev(dbllist_head(list)) = new_node;
    dbllist_head(list) = new_node;
    return 0;
}

int dbllist_remove(dbllist_t* list , dbllist_node_t* del,dbllist_destroy_t des) {
    if (dbllist_head(list) == NULL || del == NULL) {
        return -1;
    }
    if(dbllist_prev(del) == NULL && dbllist_next(del) == NULL){   //there is single node
        dbllist_head(list) = NULL;
        dbllist_tail(list) = NULL;
        if (des == DBLLIST_FREE_DATA){
            free(dbllist_data(del));
        }
        free(del);
        (list->size)--;
        return 0;
    }
    if (dbllist_head(list) == del) {   //If node to be deleted is head node

        list->head = dbllist_next(del);
        dbllist_prev(dbllist_head(list)) = NULL;
        if (des == DBLLIST_FREE_DATA){
            free(dbllist_data(del));
        }
        free(del);
        (list->size)--;
        return 0;
    }

    if(dbllist_tail(list) == del){
        list->tail = dbllist_prev(del);
        dbllist_next(dbllist_tail(list)) = NULL;
        if (des == DBLLIST_FREE_DATA){
            free(dbllist_data(del));
        }
        free(del);
        list->size--;
        return 0;
    }
    //node in the middle

    dbllist_next(dbllist_prev(del)) = dbllist_next(del);
    dbllist_prev(dbllist_next(del)) = dbllist_prev(del);
    if (des == DBLLIST_FREE_DATA){
        free(dbllist_data(del));
    }
        free(del);
    list->size--;
    return 0;
}


