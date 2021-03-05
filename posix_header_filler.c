#include "my_tar.h"
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
  
  char magic[6];
  char version[2];
  char uname[32];               
  char gname[32];
  char devmajor[8];             
  char devminor[8];   
    

  //In progress
  char chksum[8];  
  char typeflag;             
                
  //Undone         
  char prefix[155];
  char linkname[100];            

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
    int i = 3;
    int j = 4;
    header->mode[0] = '0';
    header->mode[1] = '0';
    header->mode[2] = '0';
    header->mode[3] = '0';
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

void fill_chksum(header* header)
{
    int sum = 0;
    char* header_index = header->name;
    int i = 0;
    while (i <= 500)
    {
        sum += header_index[i];
        i++;
    }
    sum += 32 * 8;
    char* chksum = my_itoa_base(sum, 8);
    int len = my_strlen(chksum);
    char* zero_string = zero_filled_string(len, 6);
    char* zero_buffer_combination = combine_strings(zero_string, chksum);
    i = 0;
    while (i < my_strlen(zero_buffer_combination))
    {
        header->chksum[i] = zero_buffer_combination[i];
        i++;
    }
    header->chksum[i] = '\0';
    header->chksum[i + 1] = ' ';
    free(chksum);
    free(zero_string);
    free(zero_buffer_combination);
}

/*Haven't figured out what to do about the following:
 Links to already archived files
 Reserved files
 */
void fill_typeflag(struct stat statbuf, header* header)
{
    if (S_ISREG(statbuf.st_mode))
    {
        header->typeflag = '0';
    }
    else if (S_ISLNK(statbuf.st_mode))
    {
        // if(link points to item in archive)
        // {
        //     header->typeflag = '1';
        // }
        // else
        // {
        //     header->typeflag = '2';
        // }
        header->typeflag = '2';
    }
    else if (S_ISCHR(statbuf.st_mode))
    {
        header->typeflag = '3';
    }
    else if (S_ISBLK(statbuf.st_mode))
    {
        header->typeflag = '4';
    }
    else if (S_ISDIR(statbuf.st_mode))
    {
        header->typeflag = '5';
    }
    else if (S_ISFIFO(statbuf.st_mode))
    {
        header->typeflag = '6';
    }
    else
    {
        header->typeflag = '7';
    }
}

void fill_linkname(char* file_path, header* header)
{
    if (header->typeflag == '1' || header->typeflag == '2')
    {
        readlink(file_path, header->linkname, 99);
        header->linkname[99] = '\0';
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

void fill_devmajor(int device_id, header* header)
{
    int major_id = major(device_id);
    
    if (major_id != 0)
    {
        char* major_id_str = my_itoa_base(major_id, 8);
        int len = my_strlen(major_id_str);
        char* zero_string = zero_filled_string(len, 7);
        char* zero_buffer_combination = combine_strings(zero_string, major_id_str);
        int i = 0;
        while(zero_buffer_combination[i] != '\0')
        {
            header->devmajor[i] = zero_buffer_combination[i];
            i++;
        }
        header->devmajor[i] = '\0';
        // free(major_id_str);
        free(zero_string);
        free(zero_buffer_combination);
    }
    else
    {
       header->devmajor[0] = '\0';     
    }
}

void fill_devminor(int device_id, header* header)
{
    int minor_id = minor(device_id);
    
    if (minor_id != 0)
    {
        char* minor_id_str = my_itoa_base(minor_id, 8);
        int len = my_strlen(minor_id_str);
        char* zero_string = zero_filled_string(len, 7);
        char* zero_buffer_combination = combine_strings(zero_string, minor_id_str);
        int i = 0;
        while(zero_buffer_combination[i] != '\0')
        {
            header->devminor[i] = zero_buffer_combination[i];
            i++;
        }
        header->devminor[i] = '\0';
        //free(minor_id_str);
        free(zero_string);
        free(zero_buffer_combination);        
    } 
    else
    {
       header->devminor[0] = '\0';     
    }
}

void fill_uname(int statbuf, header* header)
{
    struct passwd *pwd;
    pwd = getpwuid(statbuf);
    int i = 0;
    if(pwd != NULL)
    {
        while(pwd->pw_name[i] != '\0')
        {
            header->uname[i] = pwd->pw_name[i];
             i+= 1;
        }
        header->uname[i] = '\0';
    }
}

void fill_gname(int statbuf, header* header)
{
    struct group *grp;
    grp = getgrgid(statbuf);
    int i = 0;

    if(grp != NULL)
    {
        while(grp->gr_name[i] != '\0')
        {
            header->gname[i] = grp->gr_name[i];
            i+= 1;
        }
        header->gname[i] = '\0';
    }
}

void null_filler(header* header)
{
    char* header_index = header->name;
    int i = 0;
    while (i <= 500)
    {
        header_index[i] = '\0';
        i++;
    }
}

void fill_header(char* file_path, header* header)
{
    struct stat statbuf;
    lstat(file_path, &statbuf);

    null_filler(header);
    //Fill structure in order of elements
    fill_name(file_path, header);
    fill_mode(statbuf.st_mode, header);
    fill_uid(statbuf.st_uid, header); // this and below function are Twin functions 
    fill_gid(statbuf.st_gid , header); //did not abstract due to possibly Unknown untested cases
    fill_size(statbuf.st_size , header);
    fill_mtime(statbuf.st_mtim.tv_sec, header);    
    fill_typeflag(statbuf, header);
    fill_linkname(file_path, header);
    fill_magic(header);
    fill_version(header);       
    fill_uname(statbuf.st_uid, header);               
    fill_gname(statbuf.st_uid, header);
    fill_devmajor(statbuf.st_rdev, header);
    fill_devminor(statbuf.st_rdev, header);

    fill_chksum(header); //It needs to be the last filler function to be called 
}

int main()
{
    header* my_header = malloc(sizeof(header));
    printf("enter name of file or directory\n");
    char* file_path = malloc(100);
    scanf("%s", file_path);
    fill_header(file_path, my_header);
    int fd = open("test.tar", O_RDWR | O_CREAT, S_IRWXU);
    printf("fd = %d\n", fd);
    int bytes_written = write(fd, my_header, sizeof(header));
    printf("number of bytes written is = %d\n", bytes_written);
    printf("errno = %d\n", errno);
    close(fd);

    free(file_path);
    free(my_header);
    return 0;   
}