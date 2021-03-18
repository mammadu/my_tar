#include "archive_parser.h"

//open the archive, check for permission, etc...
//start parsin the damn thing.
//positions of the first header are clear and we have the offset documente on posix header.h
//parse every item starting from name
//  fill

int check_existence(char* file_path)
{
    struct stat statbuf;
    int val = stat(file_path, &statbuf);
    return val;
}

int check_permission(char* file_path)
{
    struct stat statbuf;
    int status = stat(file_path, &statbuf);
    char* item_type_and_permissions = my_itoa_base(statbuf.st_mode, 8);
    int permission = item_type_and_permissions[my_strlen(item_type_and_permissions) - 3] - '0';
    free(item_type_and_permissions);
    return permission;
}

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

//creates a link. Reads from FD. Fills the string name, file contents, and points to null for next link
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

//tar -xf archive.tar
int main(int argc, char** argv)
{
    int fd = initilize_archive_read(argv[2]);
    node* head = fill_link(fd);
    printf("head->string = %s\n", head->string);
    free(head);
    close(fd);
    return 0;
    // while()
}