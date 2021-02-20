#include "my_tar.h"

//ft. gives memory to structs
void structs_initializer(flags* my_flags, header* my_header)
{
    my_flags = malloc(sizeof(flags));
    my_header = malloc(sizeof(header));
}

//loops troug
void flag_hunter(int argc, char* argv[])
{
    for(int i = 0; i < argc; i += 1)
    {
        if(argv[i][0] == '-')
        {
            for(int j = 1; j < my_strlen(argv[i]); j += 1)
            {
                
            }
        }
    }
}

int main(int argc, char* argv[])
{
    flags* my_flags;
    header* my_header;
    structs_initializer(my_flags, my_header);
    
    flag_hunter(argc, argv); 

    

    return 0;
}