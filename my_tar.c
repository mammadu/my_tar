#include "my_tar.h"

// int create_archive(char* archive_name)
// {
//     check_permission(archive_name);
// }

void check_permission(char* file_path)
{
    struct stat* statbuf;
    int status = stat(file_path, statbuf);
    printf("%d\n", statbuf->st_mode);
}

int main(int argc, char* argv[])
{
    check_permission(argv[1]);
    return 0;
}