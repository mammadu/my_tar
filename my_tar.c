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
            printf("can't overwrite archive\n");
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
            printf("can't overwrite archive\n");
            return -1;
        }
    }
    else
    {
        printf("Archive does not exist\n");
        return -2;
    }
}

//Linked list implementation. Creates the linked list and returns the head of the linked list. Returns NULL if none ofthe arguments are real files.
node* linked_list_initializer(int argc, char** argv)
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
        }
        else if (errno == 13)
        {
            my_putstr("my_tar: ");
            my_putstr(argv[i]);
            my_putstr(": Cannot open: Permission denied\n");
        }
        else
        {
            my_putstr("my_tar: ERRNO =");
            char* error = my_itoa_base(errno, 10);
            my_putstr(error);
            my_putstr(": Review man error for details\n");
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
        }
        else
        {
            my_putstr("my_tar: ");
            my_putstr(argv[i]);
            my_putstr(": Cannot open: Permission denied\n");
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

node* filter_arguments_by_modtime(node* head_x, node* head_c)
{
    node* head_u = malloc(sizeof(node));
    node* temp_head_x = head_x;
    node* temp_head_u = head_u;
    int debugging_exterior = 0;
    int debugging_interior = 0;
    int flag = 0;

    while(head_c != NULL )
    {
        //printf("e: %s = %d\n",head_c->string ,debugging_exterior);
        
        while(head_x != NULL)
        {
            //printf("i: %s = %d\n",head_x->string ,debugging_interior);
            
            if(my_strcmp(head_c->string, head_x->string) == 0)
            {
                int archive_mod_time = my_atoi_base(head_x->header->mtime, 8);
                int argument_mod_time = my_atoi_base(head_c->header->mtime, 8);
                flag += 1;

                if (argument_mod_time > archive_mod_time)//ATTN! might need a flag to do once per head_c node
                {
                    head_u = head_c;
                    //printf("\n I am the head_u->string =  %s\n", head_u->string);
                    head_u = head_u->next;

                    //head_u = head_u->next;
                    //use temp node to holde next address on linked list
                    
                    //printf("%s mod_time = %d \n%s mod_time = %d\n\n", head_c->string, argument_mod_time, head_x->string, archive_mod_time);
                }
                
                // dispose resolution
            }


            
            //printf("i: %s = %d\n", head_x->string ,debugging_interior);
            debugging_interior += 1;
            flag = 0;
            head_x = head_x->next;
        }

        if (flag == 0)
        {
            //append current node_c to head_u 
        }
        head_x = temp_head_x;
        
        debugging_interior = 0;
        debugging_exterior += 1;
        head_c = head_c->next;
    }

    //head_u->next = NULL;

    return head_u;


//     take first argument_list element
//     compare against entire archive_list elements
//         if both elements have the same name, compare time
//             if argument has newer time, make copy link with next = NULL and append to archive list
//             else move to next element in argument list
//         else make copy link with next = NULL and append to archive list
//     repeat for entire argument list

}



// {

    
//     node* argument_temp_head = head_c;
//     node* archive_temp_head = head_x;
//     while (argument_temp_head != NULL)
//     {
//         while (archive_temp_head != NULL)
//         {
//             if (strcmp(archive_temp_head->string, argument_temp_head->string) == 0) //if both nodes have the same name, compare the modified time
//             {
//                 int archive_mod_time = my_atoi_base(archive_temp_head->header->mtime, 8);
//                 int argument_mod_time = my_atoi_base(argument_temp_head->header->mtime, 8);
                
//                 if (argument_mod_time > archive_mod_time) // if the argument node has a modified time greater than the archive node, append to the archive linked list
//                 {
//                     node* temp_link = argument_temp_head;
//                     argument_temp_head = argument_temp_head->next;
//                     temp_link->next = NULL;
//                     append_link(temp_link, archive_temp_head);
//                 }
//                 else //otherwise move to the next argument node
//                 {
//                     argument_temp_head = argument_temp_head->next;
//                 }
//             }
//             archive_temp_head = archive_temp_head->next;
//             if (argument_temp_head == NULL)
//             {
//                 break;
//             }
//         }
//         if (argument_temp_head == NULL)
//         {
//             break;
//         }
//         node* temp_link = argument_temp_head;
//         argument_temp_head = argument_temp_head->next;
//         temp_link->next = NULL;
//         append_link(temp_link, archive_temp_head);
//     }
// }

void select_option(flags* my_flags, int argc, char** argv)
{   
    int flag_sum = my_flags->c + my_flags->x + my_flags->t + my_flags->u + my_flags->r + my_flags->f;

    if (my_flags->f < 1)
    {
        printf("You need -f to run my_tar my_dude\n");
        return;
    }
    else if (flag_sum == 2 && my_flags->c > 0)
    {
        node* head = linked_list_initializer(argc, argv);
        int fd = initilize_archive_write(argv[ARCHIVE_ARG]);
        fill_archive(head, fd);
        free_linked_list(head);
    }
    else if(flag_sum == 2 && my_flags->x > 0)
    {
        if(check_existence(argv[ARCHIVE_ARG]) == 0)
        {
            if (is_archive(argv[ARCHIVE_ARG]) == 0)
            {
                extract_archive(argv[ARCHIVE_ARG]);
            }
        }
        else 
        {
            my_putstr("my_tar: ");
            my_putstr(argv[ARCHIVE_ARG]);
            my_putstr(": Cannot open: No such file or directory\n");
            my_putstr("my_tar: Error is not recoverable: exiting now\n");
        }
    }
    else if(flag_sum == 2 && my_flags->t > 0)
    {
        if (is_archive(argv[ARCHIVE_ARG]) == 0 && argc == 3)
        {
            extract_archive_to_list(argv[ARCHIVE_ARG]);
        }
        if (is_archive(argv[ARCHIVE_ARG]) == 0 && argc > 3)
        {
            extract_archive_to_list_on_demand(argv[ARCHIVE_ARG], argv, argc);    
        }   
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
                node* head_c = linked_list_initializer(argc, argv);
                node* head_u = filter_arguments_by_modtime(head_x, head_c); 
                //filter by argument modtime 
                read_list(head_u);
                fd = initilize_archive_write(argv[ARCHIVE_ARG]);
                
                
                
                fill_archive(head_x, fd);
                
                
                free_linked_list(head_x);
                free_linked_list(head_c);
                free_linked_list(head_u);

                close(fd);
            }
        }
        else 
        {
            my_putstr("my_tar: ");
            my_putstr(argv[ARCHIVE_ARG]);
            my_putstr(": Cannot open: No such file or directory\n");
            my_putstr("my_tar: Error is not recoverable: exiting now\n");
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
                node* head_c = linked_list_initializer(argc, argv);
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
            my_putstr("my_tar: Error is not recoverable: exiting now\n");
        }
        
    }   
    else
    {
        printf("You are doing something wrong, check your flags");
    }
}

//actual main
int main(int argc, char** argv)
{
    flags flag;
    flag_initializer(&flag);
    flag_hunter(argc, argv, &flag);
    select_option(&flag, argc, argv);
    return 0;
}