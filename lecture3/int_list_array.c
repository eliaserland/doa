#include <stdlib.h>
#include <stdio.h>
#include "int_list_array.h"

list *list_empty(void){
    list *l=malloc(sizeof(list));
    l->min=0;
    l->max=9;
    l->values=calloc(l->max - l->min + 1,sizeof(int));
    l->last_pos=-1;
    return l;
}

bool list_isEmpty(const list *l){
    return (l->last_pos==-1);
}

int list_inspect(const list *l,const list_position p){
        return l->values[p-l->min];
}

list_position list_first(const list *l){
    return l->min;
}

list_position list_end(const list *l){
    return l->last_pos+1;
}

list_position list_next(const list *l,const list_position p){
    return p+1;
}

list_position list_previous(const list *l,const list_position p){
    return p-1;
}

list_position list_insert(list *l, int value, const list_position p){
        if(p>l->last_pos+1 || p<l->min) {
                fprintf(stderr,"List insert: Illegal position\n");
                exit(1);
        }

        l->values[p-l->min]=value;

        l->last_pos++;

        return p;
}

list_position list_remove(list *l,list_position p){
    for(int i=p; i<l->last_pos; i++){
            //array_setValue(l->values, array_inspectValue(l->values,i+1) , i);
    }

    l->last_pos--;
    return p;
}

void list_free(list *l){
    free(l->values);
    free(l);
}
