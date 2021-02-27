#include "my_tar.h"
/*
We are going to have all the function to fill the 
posix_header structure in this file!

typedef struct posix_header
{                              
  char name[100];               
  char mode[8];   
  //Done

  //In progress   
  char uid[8];                  
  
  //UnDone
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

char* my_itoa_base(int value, int base)
{
	int len;
	long nbr;
	char* pointer;
	char* base_string = "0123456789ABCDEF";

	if (value == 0)
		return ("0");
	len = 0;
	nbr = value;
	while (nbr)
	{
		nbr /= base;
		len += 1;
	}
	nbr = value;
	if (nbr < 0)
	{
		if (base == 10)
			len += 1;
		nbr *= -1;
	}
	if (!(pointer = (char *)malloc(sizeof(char) * len + 1)))
		return (NULL);
	pointer[len] = '\0';
	while (nbr)
	{
		pointer[--len] = base_string[nbr % base];
		nbr /= base;
	}
	if (value < 0 && base == 10)
		pointer[0] = '-';
    
    return (pointer);
}

void fill_name(char* file_path, header* header)
{
    int i = 0;
    while(file_path[i])
    {
        header->name[i] = file_path[i];
        i += 1;
    }    
}

void fill_mode(int statmode, header* header)
{
    char* itoa_buffer = my_itoa_base(statmode, 8);
    int i = 1;
    int j = 2;
    header->mode[0] = '0';
    header->mode[1] = '0';
    while(itoa_buffer[i])
    {
        header->mode[j] = itoa_buffer[i];
        i += 1;
        j += 1;
    }
    header->mode[j] = '\0';
   
    free(itoa_buffer); 
}

void fill_uid(int statuid, header* header )
{
    char* itoa_buffer = my_itoa_base(statuid, 8);
    int i = 0;
    int j = 3;
    header->uid[0] = '0';
    header->uid[1] = '0';
    header->uid[2] = '0';
    while(itoa_buffer[i])
    {
        header->uid[j] = itoa_buffer[i];
        i += 1;
        j += 1;
    }
    header->uid[j] = '\0';
    printf("%s", header->name);
    free(itoa_buffer);
}

void fill_header(char* file_path, header* header)
{
    struct stat statbuf;
    stat(file_path, &statbuf);

    //Fill structure in order of elements
    fill_name(file_path, header);
    fill_mode(statbuf.st_mode, header);
    fill_uid(statbuf.st_uid, header);
   
}


int main()
{
    header* my_header = malloc(sizeof(header));
    char* file_path = "b";
    fill_header(file_path, my_header);
    
    free(my_header);
    return 0;   
}