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
        my_putstr("tar: ");
        my_putstr(file_path);
        my_putstr(": Cannot open: Permission denied\n");
        my_putstr("tar: Error is not recoverable: exiting now\n");
        return -1;
    }
    else
    {
        header* head = malloc(sizeof(header));
        read(fd, head, HEADER_SIZE);
        int checksum_byte_sum = string_byte_sum(head->chksum, CHKSUM_LEN); //sum of bytes in checksum field
        int original_checksum = my_atoi_base(head->chksum, 8);
        fill_chksum(head);
        int new_checksum = my_atoi_base(head->chksum, 8); 
        free(head);
        //we subtract checksum_byte_sum because fill_chksum works assuming the chksum field is only nulls
        if (original_checksum == (new_checksum - checksum_byte_sum))
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

// int check_existence(char* file_path)
// {
//     struct stat statbuf;
//     int val = stat(file_path, &statbuf);
//     return val;
// }

// int check_permission(char* file_path)
// {
//     struct stat statbuf;
//     int status = stat(file_path, &statbuf);
//     char* item_type_and_permissions = my_itoa_base(statbuf.st_mode, 8);
//     int permission = item_type_and_permissions[my_strlen(item_type_and_permissions) - 3] - '0';
//     free(item_type_and_permissions);
//     return permission;
// }

// int initilize_archive_read(char* archive_name)
// {
//     int existence = check_existence(archive_name);
//     if (existence == 0)
//     {
//         int permission = check_permission(archive_name);
//         if (permission == 7 || permission == 6 || permission == 5 || permission == 4)
//         {
//             int fd = open(archive_name, O_RDONLY); //debug how to open files. Perhaps use check existance function????
//             return fd;
//         }
//         else
//         {
//             printf("can't overwrite archive\n");
//             return -1;
//         }
//     }
//     else
//     {
//         printf("Archive does not exist\n");
//         return -2;
//     }
// }

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

void extract_archive_to_list(char* archive_name)
{
    int fd = initilize_archive_read(archive_name);
    node* head = fill_link(fd);
    int current_position = lseek(fd, 0, SEEK_CUR);
    
    //creates linked list of files from achive
    archive_to_linked_list(fd, current_position, head);

    read_list(head);
    free_linked_list(head);
    close(fd);
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
    int fd = initilize_archive_read(archive_name);
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
    file_creator_from_list(head);

    return head;
}

// int main(int argc, char** argv)
// {
//     int fd = initilize_archive_read(argv[2]);
//     node* head = fill_link(fd);
//     int current_position = lseek(fd, 0, SEEK_CUR);
    
//     //creates linked list of files from achive
//     while (data_seeker(fd, current_position) > 0)
//     {
//         // current_position = lseek(fd, -1, SEEK_CUR);
//         node* temp = fill_link(fd);
//         append_link(temp, head);
//         current_position = lseek(fd, 0, SEEK_CUR);
//     }

//     //creates files from linked list
//     file_creator_from_list(head);

//     free_linked_list(head);
//     close(fd);
    
//     return 0;
// }