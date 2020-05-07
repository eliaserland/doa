#include <stdlib.h>
#include <stdio.h>
#include "int_list_2cell.h"

list *list_empty(void){
    list *l=malloc(sizeof(list)) ;
    l->top=calloc(1,sizeof(two_cell));
    l->bottom=calloc(1,sizeof(two_cell));
    l->top->next=l->bottom;
    l->bottom->previous=l->top;
    return l;
}

bool list_isEmpty(const list *l){
    return (l->top->next==l->bottom);
}

int list_inspect(const list *l,const list_position pos){
    return pos->value;
}

list_position list_first(const list *l){
    return l->top->next;
}

list_position list_end(const list *l){
    return l->bottom;
}

list_position list_next(const list *l,const list_position p){
    return p->next;
}

list_position list_previous(const list *l,const list_position p){
    return p->previous;
}

list_position list_insert(list *l,int value, const list_position p){
    list_position newlink=malloc(sizeof(two_cell));
    newlink->value=value;
    newlink->next=p;
    newlink->previous=p->previous;
    p->previous=newlink;
    newlink->previous->next=newlink;
    return newlink;  
}

list_position list_remove(list *l,const list_position p){
    list_position retur=p->next;
    p->previous->next=p->next;
    p->next->previous=p->previous;
    free(p);
    return retur;
}

void list_free(list *l){
    list_position temppos=list_first(l);

    while(!list_isEmpty(l)){
        temppos=list_remove(l,temppos);
    }
    free(l->top);
    free(l->bottom);
    free(l);
    return;
}

