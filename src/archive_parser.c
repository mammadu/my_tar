#include "archive_parser.h"

//Checks for data different to Null '\0',
//returns exact position of first data encounter
//or end of file
int data_seeker(int fd, int current)
{
    char c;
    int bytes;
    int seek = lseek(fd, 0, current);

    while((bytes = read(fd, &c, sizeof(c))) > 0)
    {
        current +=1;
        if (c != '\0') 
        {
            seek = lseek(fd, -1, SEEK_CUR);
            current -=1;
            return current;
        }
    }
    //-1 -> It can't read archive
    //0 -> reaches endo of file
    return bytes;
}


//returns the sum of all the characters in a string
int string_byte_sum(char* str, int str_len)
{
    int i = 0;
    int sum = 0;
    while (i < str_len)
    {
        sum = sum + str[i];
        i++;
    }
    return sum;
}

int is_archive(char* file_path)
{
    int fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {

        if (errno == 2)
        {
            my_putstr("my_tar: ");
            my_putstr(file_path);
            my_putstr(": Cannot stat: No such file or directory\n");
        }
        else if (errno == 13)
        {
            my_putstr("my_tar: ");
            my_putstr(file_path);
            my_putstr(": Cannot open: Permission denied\n");
        }
        else
        {
            my_putstr("my_tar: ERRNO =");
            char* error = my_itoa_base(errno, 10);
            my_putstr(error);
            my_putstr(": Review man error for details\n");
            free(error);
        }
        close(fd);
        return -1;
    }
    else
    {
        header* head = malloc(sizeof(header));
        read(fd, head, HEADER_SIZE);
        int original_checksum = my_atoi_base(head->chksum, 8);
        fill_chksum(head);
        int new_checksum = my_atoi_base(head->chksum, 8);
        free(head);
        close(fd);
        //we subtract checksum_byte_sum because fill_chksum works assuming the chksum field is only nulls
        if (original_checksum == new_checksum)
        {
            return 0;
        }
        else
        {
            my_putstr("my_tar: This does not look like a tar archive\n");
            my_putstr("my_tar: Skipping to next header\n");
            my_putstr("my_tar: Exiting with failure status due to previous errors\n");
            return -2;
        }
    }
}

//Creates a link. Reads from FD. 
//Fills the string name, file contents, and points to null for next link
node* fill_link(int fd)
{
    //read string name, header, file contents,
    node* link = malloc(sizeof(node));
    link->header = malloc(sizeof(header));
    
    int header_bytes_read = read(fd, link->header, HEADER_SIZE);


    link->string = my_strdup(link->header->name);

    int size = my_atoi_base(link->header->size, 8);
    link->file_contents = malloc((size + 1) * sizeof(char));
    int seek = lseek(fd, 12 * sizeof(char), SEEK_CUR);

    int bytes_read = read(fd, link->file_contents, size);

    link->file_contents[size] = '\0';

    link->next = NULL;
}

int file_creator_from_list(node* head)
{  
    int index = 0;
    while (head != NULL)
    {
        index += 1;
        int mode = my_atoi_base(head->header->mode, 8);
        if (head->header->typeflag == '1' || head->header->typeflag == '2')
        {
            int sym = symlink(head->header->linkname, head->string);
        }
        
        else if(head->header->typeflag == '5')
        {
            int dir = mkdir(head->string, mode);
        }
        else
        {
            int fd = open(head->string, O_RDWR | O_CREAT | O_TRUNC, mode);
            write(fd, head->file_contents, my_atoi_base(head->header->size, 8));
            close(fd);
        }
        head = head->next;
    }
    return index;
}

//returns the file descriptor after checking for existance and permissions.
int initialize_archive_read(char* archive_name)
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

void extract_archive_to_list(char* archive_name)
{
    int fd = initialize_archive_read(archive_name);
    node* head = fill_link(fd);
    int current_position = lseek(fd, 0, SEEK_CUR);
    
    //creates linked list of files from achive
    archive_to_linked_list(fd, current_position, head);

    read_list(head);
    free_linked_list(head);
    close(fd);
}

int extract_archive_to_list_on_demand(char* archive_name, char** argv, int argc)
{
    int fd = initialize_archive_read(archive_name);
    node* head = fill_link(fd);
    int current_position = lseek(fd, 0, SEEK_CUR);
    int i = 3;
    int status = 0;
    int erro_status = 0;
    //creates linked list of files from achive
    archive_to_linked_list(fd, current_position, head);

    while(i < argc)
    {
        status = read_list_on_demand(head, argv[i]);
        if (status == 0)
            erro_status = 2;    
        i += 1;
    }
    free_linked_list(head);
    close(fd);
    
    return erro_status;
}

void archive_to_linked_list(int fd, int current_position, node* head)
{
     while (data_seeker(fd, current_position) > 0)
    {
        // current_position = lseek(fd, -1, SEEK_CUR);
        node* temp = fill_link(fd);
        append_link(temp, head);
        current_position = lseek(fd, 0, SEEK_CUR);
    }
}

void extract_archive(char* archive_name)
{
    int fd = initialize_archive_read(archive_name);
    node* head = fill_link(fd);
    int current_position = lseek(fd, 0, SEEK_CUR);
    
    //creates linked list of files from achive
    archive_to_linked_list(fd, current_position, head);

    //creates files from linked list
    file_creator_from_list(head);

    free_linked_list(head);
    close(fd);
}

node* extract_archive_to_node(char* archive_name, node* head, int fd)
{    
    head = fill_link(fd);
    int current_position = lseek(fd, 0, SEEK_CUR);
    
    //creates linked list of files from achive
    archive_to_linked_list(fd, current_position, head);

    //creates files from linked list
    // file_creator_from_list(head);

    return head;
}
