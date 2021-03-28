#include "my_tar.h"

void write_header(node* node, int fd)
{
    write(fd, node->header->name, 500);
    write(fd, HEADER_FINALE, 12);
}

int end_of_file_nulls_size(int file_size)
{
    int number_of_nulls = BLOCK_SIZE - (file_size % BLOCK_SIZE);
    return number_of_nulls;
}

char* end_of_file_nulls(int file_size)
{
    int number_of_nulls = end_of_file_nulls_size(file_size);
    char* null_ending = malloc((number_of_nulls) * sizeof(char));
    null_filler(null_ending, number_of_nulls);
    return null_ending;
}

char* end_of_archive()
{
    char* end = malloc(ARCHIVE_FINALE * sizeof(char));
    null_filler(end, ARCHIVE_FINALE);
    return end;
}

void write_content(node* node, int fd)
{
    int size = my_atoi_base(node->header->size, 8);
    write(fd, node->file_contents, size);
    int number_of_nulls = end_of_file_nulls_size(size);
    char* end = end_of_file_nulls(size);
    write(fd, end, number_of_nulls);
    free(end);
}

//write header and file content into the fd opened by create_archive
int fill_archive(node* head, int fd)
{
    while( head != NULL)
    {
        write_header(head, fd); //debug tomorrow at work the position of next file
        if (head->header->typeflag != '1' && head->header->typeflag != '2' && head->header->typeflag != '5')
        {
            write_content(head, fd);
        }
        head = head->next;
    }
    char* end = end_of_archive();
    write(fd, end, ARCHIVE_FINALE);
    close(fd);
    free(end);
}



//Checks for file existence. 0 means the file exists, -1 means the file doesn't exist.
int check_existence(char* file_path)
{
    struct stat statbuf;
    int val = stat(file_path, &statbuf);
    return val;
}

//checks the user permission of a file and returns the octal value of the permission
int check_permission(char* file_path)
{
    struct stat statbuf;
    int status = stat(file_path, &statbuf);
    char* item_type_and_permissions = my_itoa_base(statbuf.st_mode, 8);
    int permission = item_type_and_permissions[my_strlen(item_type_and_permissions) - 3] - '0';
    free(item_type_and_permissions);
    return permission;
}
/*

#	Permission	rwx
7	read, write and execute	rwx
6	read and write	rw-
5	read and execute	r-x
4	read only	r--	100
3	write and execute	-wx
2	write only	-w-
1	execute only	--x
0	none	---

*/

//returns the file descriptor after checking for existance and permissions.
int initilize_archive_write(char* archive_name)
{
    int existence = check_existence(archive_name);
    if (existence == 0)
    {
        int permission = check_permission(archive_name);
        if (permission == 7 || permission == 6 || permission == 3 || permission == 2)
        {
            int fd = open(archive_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU); //debug how to open files. Perhaps use check existance function????
            return fd;
        }
        else
        {
            my_putstr("my_tar: ");
            my_putstr(archive_name);
            my_putstr(": Cannot open: Permission denied\n");
            return -1;
        }
    }
    else
    {
        int fd = open(archive_name, O_RDWR | O_CREAT, S_IRWXU);
        return fd;
    }
}

//returns the file descriptor after checking for existance and permissions.
int initilize_archive_read(char* archive_name)
{
    int existence = check_existence(archive_name);
    if (existence == 0)
    {
        int permission = check_permission(archive_name);
        if (permission == 7 || permission == 6 || permission == 5 || permission == 4)
        {
            int fd = open(archive_name, O_RDONLY); //debug how to open files. Perhaps use check existance function????
            return fd;
        }
        else
        {
            my_putstr("my_tar: ");
            my_putstr(archive_name);
            my_putstr(": Cannot open: Permission denied\n");
            return -1;
        }
    }
    else
    {
        my_putstr("my_tar: ");
        my_putstr(archive_name);
        my_putstr(": Cannot open: No such file or directory\n");
        return -2;
    }
}

//Linked list implementation. Creates the linked list and returns the head of the linked list. Returns NULL if none ofthe arguments are real files.
node* linked_list_initializer(int argc, char** argv, int* error_status)
{
    int i = FIRST_FILE_ARG;

    node* head = create_link_with_string(argv[i]); //returns NULL if file does not exist
    while(head == NULL && i < argc) //keeps going through arguments until head != NULL
    {
        if (errno == 2)
        {
            my_putstr("my_tar: ");
            my_putstr(argv[i]);
            my_putstr(": Cannot stat: No such file or directory\n");
            *error_status = 2;
        }
        else if (errno == 13)
        {
            my_putstr("my_tar: ");
            my_putstr(argv[i]);
            my_putstr(": Cannot open: Permission denied\n");
            *error_status = 2;
        }
        else
        {
            my_putstr("my_tar: ERRNO =");
            char* error = my_itoa_base(errno, 10);
            my_putstr(error);
            my_putstr(": Review man error for details\n");
            *error_status = 2;
        }
        i++;
        head = create_link_with_string(argv[i]);
    }
    node* current = head;
    i++;

    while(i < argc) //adds rest of arguments into linked list
    {
        if (check_existence(argv[i]) == 0 && (check_permission(argv[i]) == 7 || check_permission(argv[i]) == 6 || check_permission(argv[i]) == 5 || check_permission(argv[i]) == 4)) //ie if the file exists and we have read permission
        {
            node* temp = create_link_with_string(argv[i]);
            current->next = temp;
            current = current->next;
        }
        else if (check_existence(argv[i]) == -1)
        {
            my_putstr("my_tar: ");
            my_putstr(argv[i]);
            my_putstr(": No such file or directory\n");
            *error_status = 2;
        }
        else
        {
            my_putstr("my_tar: ");
            my_putstr(argv[i]);
            my_putstr(": Cannot open: Permission denied\n");
            *error_status = 2;
        }
        i += 1;
    }

    return head;
}

void flag_initializer(flags* my_flags)
{
    my_flags->c = 0;
    my_flags->x = 0;
    my_flags->t = 0;
    my_flags->u = 0;
    my_flags->r = 0;
    my_flags->f = 0;
}

//loops thru the argv string until it finds the flags 
void flag_hunter(int argc, char* argv[], flags* my_flags)
{
    for(int i = 0; i < argc; i += 1)
    {
        if(argv[i][0] == '-')
        {
            for(int j = 1; j < my_strlen(argv[i]); j += 1)
            {
                switch(argv[i][j]) 
                {
                    case 'c' :
                        my_flags->c = 1;
                        break;
                    case 'r' :
                        my_flags->r = 1;
                        break;
                    case 't' :
                        my_flags->t = 1;
                        break;
                    case 'u' :
                        my_flags->u = 1;
                        break;
                    case 'x' :
                        my_flags->x = 1;
                        break;
                    case 'f':
                        my_flags->f = 1;
                        break;
                    default :
                        //unsopported options
                        my_flags-> unknown = argv[i][j];
                        return; 
                }
            }
        }
    }
}

f_arguments* filter_arguments_by_modtime(node* head_x, node* head_c, int argc)
{
    f_arguments* return_val = malloc(sizeof(f_arguments));
    return_val->f_argv = malloc(sizeof(char*) * argc);
    char* filler_arg = "blah";
    int i = 0;
    int in_archive = 0;
    while (i < FIRST_FILE_ARG)
    {
        return_val->f_argv[i] = my_strdup(filler_arg);
        i++;
    }
    return_val->f_argc = FIRST_FILE_ARG;
    while (head_c != NULL)
    {
        node* original_head_x = head_x;
        while (head_x != NULL)
        {
            if (my_strcmp(head_x->string, head_c->string) == 0)
            {
                in_archive = 1; //File is in archive
                int archive_mod_time = my_atoi_base(head_x->header->mtime, 8);
                int argument_mod_time = my_atoi_base(head_c->header->mtime, 8);
                if (argument_mod_time > archive_mod_time)
                {
                    return_val->f_argv[return_val->f_argc] = my_strdup(head_c->string);
                    return_val->f_argc++;
                }
            }
            head_x = head_x->next;
        }
        if (in_archive == 0)
        {
            return_val->f_argv[return_val->f_argc] = my_strdup(head_c->string);
            return_val->f_argc++;
        }
        in_archive = 0;
        head_c = head_c->next;
        head_x = original_head_x;
    }
    return return_val;
}

void free_filtered_args(f_arguments* f_arg)
{
    int i = 0;
    while (i < f_arg->f_argc)
    {
        free(f_arg->f_argv[i]);
        i++;
    }
    free(f_arg->f_argv);
    free(f_arg);
}

void option_c(int argc, char** argv, int* error_status)
{
    node* head = linked_list_initializer(argc, argv, error_status);
    int fd = initilize_archive_write(argv[ARCHIVE_ARG]);
    fill_archive(head, fd);
    free_linked_list(head);
}

void option_x(char** argv, int* error_status)
{
    if(check_existence(argv[ARCHIVE_ARG]) == 0 && is_archive(argv[ARCHIVE_ARG]) == 0)
    {
        extract_archive(argv[ARCHIVE_ARG]);
    }
    else 
    {
        *error_status = 2;
        my_putstr("my_tar: ");
        my_putstr(argv[ARCHIVE_ARG]);
        my_putstr(": Cannot open: No such file or directory\n");
    }
}

void option_t(int argc, char** argv, int* error_status)
{
    if (is_archive(argv[ARCHIVE_ARG]) == 0 && argc == 3)
    {
        extract_archive_to_list(argv[ARCHIVE_ARG]);
    }
    else if (is_archive(argv[ARCHIVE_ARG]) == 0 && argc > 3)
    {
        extract_archive_to_list_on_demand(argv[ARCHIVE_ARG], argv, argc);
    }    
    else
    {
        *error_status = 2;
    }
}

int select_option(flags* my_flags, int argc, char** argv)
{   
    int flag_sum = my_flags->c + my_flags->x + my_flags->t + my_flags->u + my_flags->r + my_flags->f;

    int error_status = 0;

    if (my_flags->f < 1)
    {
        my_putstr("Cannot execute. Missing -f option?\n");
        error_status = 2;
    }

    else if (flag_sum == 2 && my_flags->c > 0)
    {
        option_c(argc, argv, &error_status);
    }

    else if(flag_sum == 2 && my_flags->x > 0)
    {
        option_x(argv, &error_status);
    }

    else if(flag_sum == 2 && my_flags->t > 0)
    {
        option_t(argc, argv, &error_status);
    }   
    
    else if(flag_sum == 2 && my_flags->u > 0)
    {
        if(check_existence(argv[ARCHIVE_ARG]) == 0)
        {
            if (is_archive(argv[ARCHIVE_ARG]) == 0)
            {
                //option_u
                int fd = initilize_archive_read(argv[ARCHIVE_ARG]);
                
                node* head_x = extract_archive_to_node(argv[ARCHIVE_ARG], head_x, fd);
                node* head_c = linked_list_initializer(argc, argv, &error_status);
                f_arguments* filtered_args = filter_arguments_by_modtime(head_x, head_c, argc);
                node* head_u = linked_list_initializer(filtered_args->f_argc, filtered_args->f_argv, &error_status);
                //filter by argument modtime 
                int links = read_list(head_u);
                append_link(head_u, head_x);
                fd = initilize_archive_write(argv[ARCHIVE_ARG]);
                
                fill_archive(head_x, fd);
                free_filtered_args(filtered_args);
                free_linked_list(head_x);
                free_linked_list(head_c);

                close(fd);
            }
        }
        else 
        {
            my_putstr("my_tar: ");
            my_putstr(argv[ARCHIVE_ARG]);
            my_putstr(": Cannot open: No such file or directory\n");
        }
    }
    else if(flag_sum == 2 && my_flags->r > 0)
    {
        if(check_existence(argv[ARCHIVE_ARG]) == 0)
        {
            if (is_archive(argv[ARCHIVE_ARG]) == 0)
            {
                //option_r
                int fd = initilize_archive_read(argv[ARCHIVE_ARG]);
                node* head_x = extract_archive_to_node(argv[ARCHIVE_ARG], head_x, fd);
                node* head_c = linked_list_initializer(argc, argv, &error_status);
                fd = initilize_archive_write(argv[ARCHIVE_ARG]);
                
                append_link( head_c, head_x);

                fill_archive(head_x, fd);
            
                free_linked_list(head_x);
                close(fd);
            }
        }
        else 
        {
            my_putstr("my_tar: ");
            my_putstr(argv[ARCHIVE_ARG]);
            my_putstr(": Cannot open: No such file or directory\n");            
        }
        
    }   
    else
    {
        my_putstr("my_tar: You must specify one of the '-ctrux'\n");
    }
    return error_status;
}

//actual main
int main(int argc, char** argv)
{
    flags flag;
    flag_initializer(&flag);
    flag_hunter(argc, argv, &flag);
    int success = select_option(&flag, argc, argv);
    if (success != 0)
    {
        my_putstr("my_tar: Exiting with failure status due to previous errors\n");
    }
    return success;
}