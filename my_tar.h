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


int create_archive(char* archive_name);
int check_existence(char* file_path);
int check_permission(char* file_path);
void flag_hunter(int argc, char* argv[], flags* my_flags);
char* my_strcpy(char* dst, char* src);

#endif