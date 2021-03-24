#ifndef linked_list_H
#define linked_list_H
#include <stdio.h>
#include <stdlib.h>
#include "posix_header_filler.h"
#include "my_c_functions.h"

#define HEAD (1)
#define NO_HEAD (0)


typedef struct node
{
    char* string;
    header* header;
    char* file_contents;
    struct node* next;
}node;

void linked_list_initializer(int position, char** argv, node* head, int argc, int is_head);

node* create_link_with_string(char** files, int position, int argc, int is_head);

node* create_link_with_max_string_length(int max_string_length);

node* prepend_link(node* new_link, node* head);

void append_link(node* new_link, node* head);

void insert_link(node* new_link, node* previous_link);

int read_list(node* head);

node* return_next_link(node* link);

void free_linked_list(node* head);

#endif