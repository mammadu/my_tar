#include "my_tar.h"

//This is option C

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
        //We need to check if argv[3] exist :/
        node* head = create_link_with_string(argv, FIRST_FILE, argc, HEAD);
        //linked_list_initializer(argc, argv, head, argc);
        int fd = initilize_archive_write(argv[2]);
        fill_archive(head, fd);
        free_linked_list(head);
    }
    else if(flag_sum == 2 && my_flags->x > 0)
    {
        if(check_existence(argv[2]) == 0)
        {
            extract_archive(argv[2]);
        }
        else 
        {
            my_putstr("my_tar: ");
            my_putstr(argv[2]);
            my_putstr(": Cannot open: No such file or directory\n");
            my_putstr("my_tar: Error is not recoverable: exiting now\n");
        }
    }
    else if(flag_sum == 2 && my_flags->t > 0)
    {
        if(check_existence(argv[2]) == 0)
        {
            extract_archive_to_list(argv[2]);
        }
        else 
        {
            my_putstr("my_tar: ");
            my_putstr(argv[2]);
            my_putstr(": Cannot open: No such file or directory\n");
            my_putstr("my_tar: Error is not recoverable: exiting now\n");
        }
    }   
    else if(flag_sum == 2 && my_flags->u > 0)
    {
        printf("Run option u please\n");//option_u
    }
    else if(flag_sum == 2 && my_flags->r > 0)
    {
        if(check_existence(argv[2]) == 0)
        {
            //Option_r
            int fd = initilize_archive_read(argv[2]);
            node* head_x = extract_archive_to_node(argv[2], head_x, fd);
            
            node* head_c = create_link_with_string(argv, FIRST_FILE, argc, HEAD);
            
            fd = initilize_archive_write(argv[2]);
            
            append_link( head_c, head_x);

            fill_archive(head_x, fd);
        
            free_linked_list(head_x);
            close(fd);
        }
        else 
        {
            my_putstr("my_tar: ");
            my_putstr(argv[2]);
            my_putstr(": Cannot open: No such file or directory\n");
            my_putstr("my_tar: Error is not recoverable: exiting now\n");
        }
        
    }   
    else
    {
        printf("You are doing something wrong, check your flags");
    }
}

//Linked list implementation

// void linked_list_initializer(int position, char** argv, node* head, int argc)
// {
//     int i = position;

//     while(i < argc)
//     {
//         if (check_existence(argv[i]) == 0)
//         {
//             node* temp = create_link_with_string(argv, i, argc);
//             head->next = temp;
//             head = head->next;
//         }
//         else
//         {
//             my_putstr("my_tar: ");
//             my_putstr(argv[i]);
//             my_putstr(": No such file or directory\n");
//         }
        
//         i += 1;
//     }
// }

//actual main
int main(int argc, char** argv)
{
    flags flag;
    flag_initializer(&flag);
    flag_hunter(argc, argv, &flag);
    select_option(&flag, argc, argv);
    return 0;
}