#ifndef INT_LIST_H
#define INT_LIST_H

#include <stdbool.h>

typedef struct two_cell {  
    struct two_cell *next;
    struct two_cell *previous;
    int value;
} two_cell;

typedef two_cell* list_position;

typedef struct {
    two_cell *top;
    two_cell *bottom;
} list;

list *list_empty(void);
list_position list_insert(list *l,int value, const list_position pos);
bool list_isEmpty(const list *l);
int list_inspect(const list *l, const list_position p);
list_position list_first(const list *l);
list_position list_end(const list *l);
list_position list_next(const list *l,const list_position p);
list_position list_previous(const list *l,const list_position p);
list_position list_remove(list *l,const list_position p);
void list_free(list *l);

#endif /* INT_LIST_H */

