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


// Read block size.
// Used 512 bytes to copy tar's block size. :)
// Source: http://www.gnu.org/software/tar/manual/html_node/Blocking.html


int initilize_archive(char* archive_name);
int check_existence(char* file_path);
int check_permission(char* file_path);
void flag_initializer(flags* my_flags);
void flag_hunter(int argc, char* argv[], flags* my_flags);
void select_option(flags* my_flags, int argc, char** argv);
void linked_list_initializer(int nodes_qty, char** argv, node* head);

#endif