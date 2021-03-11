#include "my_tar.h"

//This is option C

void write_header(node* node, int fd)
{
    write(fd, node->header->name, 500);
    write(fd, HEADER_FINALE, 11);
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
        if (head->header->typeflag != '1' && head->header->typeflag != '2')
        {
            write_content(head, fd);
            printf("head->header->typeflag = %c\n", head->header->typeflag);
        }
        head = head->next;
    }
    char* end = end_of_archive();
    write(fd, end, ARCHIVE_FINALE);
    close(fd);
    free(end);
}

//returns the file descriptor after checking for existance and permissions.
int initilize_archive(char* archive_name)
{
    int existence = check_existence(archive_name);
    if (existence == 0)
    {
        int permission = check_permission(archive_name);
        if (permission == 7 || permission == 6 || permission == 3 || permission == 2)
        {
            int fd = open(archive_name, O_RDWR | O_CREAT, S_IRWXU);//debug how to remove old archive to write a fresh archive
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

void select_option(flags* my_flags, char* archive_name ,node* head)
{   
    int flag_sum = my_flags->c + my_flags->x + my_flags->t + my_flags->u + my_flags->r + my_flags->f;

    if (my_flags->f < 1)
    {
        printf("You need -f to run my_tar my_dude\n");
        return;
    }
    else if (flag_sum == 2 && my_flags->c > 0)
    {
        int fd = initilize_archive(archive_name);
        fill_archive(head, fd);
    }
    else if(flag_sum == 2 && my_flags->x > 0)
    {
        printf("Run option x please\n");//option_x
    }
    else if(flag_sum == 2 && my_flags->t > 0)
    {
        printf("Run option t please\n");//option_t
    }   
    else if(flag_sum == 2 && my_flags->u > 0)
    {
        printf("Run option u please\n");//option_u
    }
    else if(flag_sum == 2 && my_flags->r > 0)
    {
        printf("Run option r please\n");//option_r
    }   
    else if(flag_sum == 2 && my_flags->f > 0)
    {
        printf("Run option f please\n");//option_f
    }
    else
    {
        printf("You are doing something extremely wrong curb your expectations");
    }
}

//Linked list implementation

void linked_list_initializer(int nodes_qty, char** argv, node* head)
{
    int i = 4;

    while(i < nodes_qty)
    {
        node* temp = create_link_with_string(argv[i]);
        head->next = temp;
        head = head->next;
        i += 1;
    }
}



//actual main
int main(int argc, char** argv)
{
    flags flag;
    node* head = create_link_with_string(argv[3]);
    linked_list_initializer(argc, argv, head);
    flag_initializer(&flag);
    flag_hunter(argc, argv, &flag);
    select_option(&flag, argv[2], head);
    //void select_option(flags* my_flags, char* archive_name ,node* head)
    free_linked_list(head);
    return 0;
}