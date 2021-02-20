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

void flag_initializer(flags* my_flags)
{
    my_flags->c = 0; 
    my_flags->x = 0; 
    my_flags->t = 0; 
    my_flags->u = 0;
    my_flags->r = 0; 
    my_flags->f = 0;    
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


void select_option(flags* my_flags, char* argv[])
{   
    int flag_sum = my_flags->c + my_flags->x + my_flags->t + my_flags->u + my_flags->r + my_flags->f;

    if (my_flags->f < 1)
    {
        printf("You need -f to run my_tar my_dude\n");
        return;
    }
    else if (flag_sum == 2 && my_flags->c > 0)
    {
        printf("Run option c please\n");//option_c
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

int main(int argc, char* argv[])
{
    // check_permission(argv[1]);
    flags* my_flags;
    header* my_header;

    my_flags = malloc(sizeof(flags));
    my_header = malloc(sizeof(header));
    
    flag_initializer(my_flags); 
    flag_hunter(argc, argv, my_flags); 
    
    //logic router
    select_option(my_flags, argv);
    free(my_header);
    free(my_flags);
    

    return 0;
}