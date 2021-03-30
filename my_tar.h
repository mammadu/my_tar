#ifndef MY_TAR_H
#define MY_TAR_H

// Header files dependencies.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include "linked_list.h"
#include "my_c_functions.h"
#include "posix_header_filler.h"
#include "archive_parser.h"
#define HEADER_FINALE "\0\0\0\0\0\0\0\0\0\0\0"
#define ARCHIVE_FINALE (1024)
#define ARCHIVE_ARG (2) //for any option, the archive argument is index 2
#define FIRST_FILE_ARG (3) //for any option, the first file argument is index 3
#define SUCCESS (0)
#define FAILURE (2)

//Struct to keep track of the passed flags

typedef struct flag_structs
{
    int c;
    int x;
    int t;
    int u;
    int r;
    int f;
    char unknown;
} flags;

typedef struct filtered_arguments
{
    char** f_argv;
    int f_argc;
} f_arguments;


// Read block size.
// Used 512 bytes to copy tar's block size. :)
// Source: http://www.gnu.org/software/tar/manual/html_node/Blocking.html


void write_header(node* node, int fd);
int end_of_file_nulls_size(int file_size);
char* end_of_file_nulls(int file_size);
char* end_of_archive();
void write_content(node* node, int fd);
int fill_archive(node* head, int fd);
int check_existence(char* file_path);
int check_permission(char* file_path);
int initialize_archive_write(char* archive_name);
node* linked_list_initializer(int argc, char** argv, int* error_status);
void flag_initializer(flags* my_flags);
void flag_hunter(int argc, char* argv[], flags* my_flags);
f_arguments* filter_arguments_by_modtime(node* head_x, node* head_c, int argc);
void free_filtered_args(f_arguments* f_arg);
void option_c(int argc, char** argv, int* error_status);
void option_x(char** argv, int* error_status);
void option_t(int argc, char** argv, int* error_status);
void option_u(int argc, char** argv, int* error_status);
void option_r(int argc, char** argv, int* error_status);
int select_option(flags* my_flags, int argc, char** argv);

#endif