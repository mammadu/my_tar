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

void archive_to_linked_list(int fd, int current_position, node* head);
void extract_archive_to_list(char* archive_name);
int file_creator_from_list(node* head);
int is_archive(char* file_path);
int check_permission(char* file_path);
int check_existence(char* file_path);
int initilize_archive_read(char* archive_name);
int data_seeker(int fd, int current);
node* fill_link(int fd);
void extract_archive(char* archive_name);

#endif