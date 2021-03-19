#ifndef archive_parser_H
#define archive_parser_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "my_c_functions.h"
#include "linked_list.h"
#include "posix_header_filler.h"


int initilize_archive_read(char* archive_name);
int data_seeker(int fd, int current);
node* fill_link(int fd);

#endif