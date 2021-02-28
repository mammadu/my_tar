#include "my_tar.h"
#include "my_c_functions.c"
/*
We are going to have all the function to fill the 
posix_header structure in this file!

typedef struct posix_header
{ 
  //Done                             
  char name[100];               
  char mode[8];   
  char uid[8];
  char gid[8];

  //In progress   
  char size[12];              

  //UnDone                    
                 
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
    header->name[i] = '\0';    
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

void fill_uid(int statuid,  header* header )
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

    free(itoa_buffer);
}

void fill_gid(int statguid, header* header)
{
    char* itoa_buffer_g = my_itoa_base(statguid, 8);
    int i = 0;
    int j = 3;
    
    header->gid[0] = '0';
    header->gid[1] = '0';
    header->gid[2] = '0';

    while(itoa_buffer_g[i])
    {
        header->gid[j] = itoa_buffer_g[i];
        i += 1;
        j += 1;
    }
    header->gid[j] = '\0';

    free(itoa_buffer_g);
}

//Count the length of the string
//strlen - 11 = num of 0's in a dummy string 
//char* combine_strings(char* first_string, char* second_string)

char* zero_filled_string(int len)
{
    int i = 0;
    char* zeroes = malloc(sizeof(char) * (11 - len + 1));

    while(i < 11 - len)
    {
        zeroes[i] = '0';
        i += 1;
    }
    zeroes[i] ='\0';
    return zeroes;
}

void fill_size(int statsize, header* header)
{
    char* statsize_buffer = my_itoa_base(statsize, 8);
    int statsize_buffer_len = my_strlen(statsize_buffer);
    char* zero_string = zero_filled_string(statsize_buffer_len);
    char* zero_buffer_combination= combine_strings(zero_string, statsize_buffer);
    int i = 0;

    while(zero_buffer_combination[i] != '\0')
    {
        header->size[i] = zero_buffer_combination[i];
        i += 1;
    }
    header->size[i] = '\0';
    // printf("%s", header->size);

    free(zero_buffer_combination);
    free(zero_string);
    free (statsize_buffer);
}

void fill_mtime(int mtime, header* header)
{
    char* octal_time = my_itoa_base(mtime, 8);
    int i = 0;
    while(octal_time[i] != '\0')
    {
        header->mtime[i] = octal_time[i];
        i++;
    }
    header->mtime[i] = '\0';
    free(octal_time);
}


void fill_header(char* file_path, header* header)
{
    struct stat statbuf;
    stat(file_path, &statbuf);

    //Fill structure in order of elements
    fill_name(file_path, header);
    fill_mode(statbuf.st_mode, header);
    fill_uid(statbuf.st_uid, header); // this and below function are Twin functions 
    fill_gid(statbuf.st_gid , header); //did not abstract due to possibly Unknown untested cases
    fill_size(statbuf.st_size , header);
    fill_mtime(statbuf.st_mtim.tv_sec, header);    
}


int main()
{
    header* my_header = malloc(sizeof(header));
    char* file_path = "file1";
    fill_header(file_path, my_header);
    int fd = open("fuck.tar", O_RDWR, O_CREAT);
    int bytes_written = write(fd, my_header, sizeof(header));
    printf("number of bytes written is = %d\n", bytes_written);
    printf("errno = %d\n", errno);
    close(fd);

    free(my_header);
    return 0;   
}