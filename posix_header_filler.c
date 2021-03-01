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
  char size[12];

  char mtime[12];

  char typeflag;
  char magic[6];
  char version[2];

  //In progress
  char chksum[8];               

  //UnDone                    

  char linkname[100];           
                
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

char* zero_filled_string(int len, int field_size) //field size is 1 less than the length of the character array to account for the NULL character
{
    int i = 0;
    char* zeroes = malloc(sizeof(char) * (field_size - len + 1));

    while(i < field_size - len)
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
    char* zero_string = zero_filled_string(statsize_buffer_len, 11);
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

/*Haven't figured out what to do about the following:
 Links to already archived files
 Reserved files
 */
void fill_typeflag(int mode, header* header)
{
    if (S_ISREG(mode))
    {
        header->typeflag = '0';
    }
    else if (S_ISLNK(mode))
    {
        header->typeflag = '2';
    }
    else if (S_ISCHR(mode))
    {
        header->typeflag = '3';
    }
    else if (S_ISBLK(mode))
    {
        header->typeflag = '4';
    }
    else if (S_ISDIR(mode))
    {
        header->typeflag = '5';
    }
    else if (S_ISFIFO(mode))
    {
        header->typeflag = '6';
    }
    else
    {
        header->typeflag = '7';
    }
}

void fill_magic(header* header)
{
    char* str = "ustar";
    int i = 0;
    while (str[i] != '\0')
    {
        header->magic[i] = str[i];
        i++;
    }
    header->magic[i] = ' ';
}

void fill_version(header* header)
{
    char* str = " ";
    int i = 0;
    while (str[i] != '\0')
    {
        header->version[i] = str[i];
        i++;
    }
    header->version[i] = '\0';
}

void fill_chksum(header* header)
{
    
    int sum = 0;
    for (int i = 0; i < my_strlen(header->name); i++)
    {
        sum += header->name[i];
    }
    for (int i = 0; i < my_strlen(header->mode); i++)
    {
        sum += header->mode[i];
    }
    for (int i = 0; i < my_strlen(header->uid); i++)
    {
        sum += header->uid[i];
    }
    for (int i = 0; i < my_strlen(header->gid); i++)
    {
        sum += header->gid[i];
    }
    for (int i = 0; i < my_strlen(header->size); i++)
    {
        sum += header->size[i];
    }
    for (int i = 0; i < my_strlen(header->mtime); i++)
    {
        sum += header->mtime[i];
    }
    sum += header->typeflag;
    for (int i = 0; i < my_strlen(header->linkname); i++)
    {
        sum += header->linkname[i];
    }
    for (int i = 0; i < my_strlen(header->magic); i++)
    {
        sum += header->magic[i];
    }
    for (int i = 0; i < my_strlen(header->version); i++)
    {
        sum += header->version[i];
    }
    // for (int i = 0; i < my_strlen(header->uname); i++)
    // {
    //     sum += header->uname[i];
    // }
    // for (int i = 0; i < my_strlen(header->gname); i++)
    // {
    //     sum += header->gname[i];
    // }
    // for (int i = 0; i < my_strlen(header->devmajor); i++)
    // {
    //     sum += header->devmajor[i];
    // }
    // for (int i = 0; i < my_strlen(header->devminor); i++)
    // {
    //     sum += header->devminor[i];
    // }
    // for (int i = 0; i < my_strlen(header->prefix); i++)
    // {
    //     sum += header->prefix[i];
    // }
    char* chksum = my_itoa_base(sum, 8);
    int len = my_strlen(chksum);
    char* zero_string = zero_filled_string(len, 7);
    char* zero_buffer_combination= combine_strings(zero_string, chksum);    
    for (int i = 0; i < my_strlen(zero_buffer_combination); i++)
    {
        header->chksum[i] = zero_buffer_combination[i];
    }
    free(chksum);
    free(zero_string);
    free(zero_buffer_combination);
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
    fill_typeflag(statbuf.st_mode, header);
    fill_magic(header);
    fill_version(header);   
    fill_chksum(header);
}


int main()
{
    header* my_header = malloc(sizeof(header));
    printf("enter name of file or directory\n");
    char* file_path = malloc(100);
    scanf("%s", file_path);
    fill_header(file_path, my_header);
    int fd = open("test.tar", O_RDWR, O_CREAT, S_IRWXU);
    printf("fd = %d\n", fd);
    int bytes_written = write(fd, my_header, sizeof(header));
    printf("number of bytes written is = %d\n", bytes_written);
    printf("errno = %d\n", errno);
    close(fd);

    free(file_path);
    free(my_header);
    return 0;   
}