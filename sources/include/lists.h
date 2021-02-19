#ifndef __LIST_H__
#define __LIST_H__
struct List
{
    void* element;
    struct List* next;

};
void list_free(struct List* list,void free_fct(void*));
void list_remove_next(struct List* list); // ! Does NOT free the element !
struct List* list_add(struct List* list, void* element);
void list_delete_next(struct List* list);  // ! Does free the element !
#endif //__LIST_H__