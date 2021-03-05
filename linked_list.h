#ifndef linked_list_H
#define linked_list_H
#include <stdio.h>
#include <stdlib.h>
#include "my_c_functions.h"
#include "my_tar.h"

typedef struct node
{
    header* header;
    char* string; 
    struct node* next;
}node;

node* create_link_with_string(char* string);

node* create_link_with_max_string_length(int max_string_length);

node* prepend_link(node* new_link, node* head);

void append_link(node* new_link, node* head);

void insert_link(node* new_link, node* previous_link);

int read_list(node* head);

node* return_next_link(node* link);

#endif