#include "my_tar.h"
/*
We are going to have all the function to fill the 
posix_header structure in this file!

typedef struct posix_header
{                              
  char name[100];               
  char mode[8];                 
  char uid[8];                  
  char gid[8];                  
  char size[12];               
  char mtime[12];               
  char chksum[8];               
  char typeflag;                
  char linkname[100];           
  char magic[6];                
  char version[2];              
  char uname[32];               
  char gname[32];               
  char devmajor[8];             
  char devminor[8];             
  char prefix[155];            

} header;

Type Flag	File Type
0 or null	Regular file
1	Link to another file already archived
2	Symbolic link
3	Character special device
4	Block special device
5	Directory
6	FIFO special file
7	Reserved
A-Z	Available for custom usage

*/

void fill_name(char* file_path, header* header)
{
    int i = 0;
    while(file_path[i])
    {
        header->name[i] = file_path[i];
        i += 1;
    }    
}

// void fill_mode(stat statbuf, header* header)
// {

// }



void fill_header(char* file_path, header* header)
{
    struct stat statbuf;
    int val = stat(file_path, &statbuf);

    fill_name(file_path, header);
    printf("%o",statbuf.st_mode);
}

int main()
{
    header* my_header = malloc(sizeof(header));
    char* file_path = "abc/";
    fill_header(file_path, my_header);

    return 0;   
}