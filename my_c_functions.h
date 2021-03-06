#ifndef my_c_functions_H
#define my_c_functions_H
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

char* my_strdup(char* param_1);

char* combine_strings(char* first_string, char* second_string);

int my_strlen(char* string);

int my_recursive_pow(int param_1, int param_2);

int my_atoi(char* param_1);

void my_putstr(char* param_1);

char* reverse_string(char* param_1);

char hex_to_char (int num, char base);

char *num_to_str(int num, char base);

char* my_itoa_base(int value, int base);

int my_strcmp(char* param_1, char* param_2);

void null_filler(void* pointer, int length);

#endif