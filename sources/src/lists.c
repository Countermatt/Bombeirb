#include <lists.h>
#include <stdlib.h>

struct List* list_add(struct List* list, void* element){
    struct List* newhead=(struct List*) malloc(sizeof(struct List));
    newhead->element=element;
    newhead->next=list;
    return(newhead);
}

void list_free(struct List* list,void (*free_fct)(void*)){
    if (free_fct==NULL){
        while (list!=NULL){
            struct List* tmp=list->next;
            free(list->element);
            free(list);
            list=tmp;
        }
    }
    else{
        while (list!=NULL){
            struct List* tmp=list->next;
            free_fct(list->element);
            free(list);
            list=tmp;
        }
    }
}

void list_remove_next(struct List* list){
    void* tmp = list->next;
    list->next=list->next->next;
    free(tmp);
}

void list_delete_next(struct List* list){
    free(list->next->element);
    void* tmp = list->next;
    list->next=list->next->next;
    free(tmp);
}