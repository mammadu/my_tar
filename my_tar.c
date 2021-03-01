#include "my_tar.h"

//This is option C
int create_archive(char* archive_name)
{
    int existence = check_existence(archive_name);
    if (existence == 0)
    {
        int permission = check_permission(archive_name);
        if (permission == 7 || permission == 6 || permission == 3 || permission == 2)
        {
            //make_archive(archive_name);
            printf("make archive\n");
        }
        else
        {
            printf("can't overwrite archive\n");
        }
    }
    else
    {
        //make_archive(archive_name);
        printf("make archive\n");
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

// int main(int argc, char** argv)
// {
//     int existence = check_existence(argv[1]);
//     printf("%s has existence %d\n", argv[1], existence);
//     // flags* my_flags;
//     // header* my_header;

//     // my_flags = malloc(sizeof(flags));
//     // my_header = malloc(sizeof(header));
    
//     // flag_initializer(my_flags); 
//     // flag_hunter(argc, argv, my_flags); 
    
//     // //logic router
//     // select_option(my_flags, argv);
//     // free(my_header);
//     // free(my_flags);
    
//     return 0;
// }

//actual main
int main(int argc, char** argv)
{
    flags flag;
    flag_initializer(flag);
    flag_hunter(argc, argv, flag);
    select_option(flag, argv);
        
    return 0;
}