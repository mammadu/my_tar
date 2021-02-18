#include "linked_list.h"

node* create_link_with_string(char* string)
{
    node* link = malloc(sizeof(node));
    link->string = my_strdup(string);
    link->next = NULL;    
    return link;
}

node* create_link_with_max_string_length(int max_string_length)
{
    node* link = malloc(sizeof(node));
    link->string = malloc(max_string_length * sizeof(char));
    link->next = NULL;    
    return link;
}

node* prepend_link(node* new_link, node* head)
{
    new_link->next = head;
    head = new_link;
    return head;
}


void append_link(node* new_link, node* head)
{   
    node* current = head;
    while(current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_link;
}

void insert_link(node* new_link, node* previous_link)
{
    new_link->next = previous_link->next;
    previous_link->next = new_link;
}

int read_list(node* head)
{  
    int index = 0;
    while (head != NULL)
    {
        index++;
        printf("%s", head->string);
        head = head->next;
    }
    return index;
}

node* return_next_link(node* link)
{
    node* next_link = link->next;
    return next_link;
}