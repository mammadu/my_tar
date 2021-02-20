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

//loops troug the argv string until it finds the flags 
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

int main(int argc, char* argv[])
{
    check_permission(argv[1]);
    flags* my_flags;
    header* my_header;

    my_flags = malloc(sizeof(flags));
    my_header = malloc(sizeof(header));

    flag_hunter(argc, argv, my_flags); 
    printf("%d", my_flags->r);
    

    return 0;
}