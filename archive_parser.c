//open the archive, check for permission, etc...
//start parsin the damn thing.
//positions of the first header are clear and we have the offset documente on posix header.h
//parse every item starting from name
//  fill

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


void fill_link(node* link, int fd, int position)
{
    //read string name, header, file contents,
}