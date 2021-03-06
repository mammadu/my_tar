#ifndef posix_header_filler_H
#define posix_header_filler_H

#include "my_c_functions.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


# define BLOCK_SIZE	(512)
# define HEADER_SIZE (500)

# define NAME_LEN (100)
# define MODE_LEN_LEN (8)
# define UID_LEN_LEN (8)
# define GID_LEN (8)
# define SIZE_LEN (12)
# define MTIME_LEN (12)
# define CHKSUM_LEN (8)
# define LINKNAME_LEN (100)
# define MAGIC_LEN (6)
# define VERSION_LEN (2)
# define UNAME_LEN (32)
# define GNAME_LEN (32)
# define DEVMAJOR_LEN (8)
# define DEVMINOR_LEN (8)
# define PREFIX_LEN (155)

//Metadata header for collection of file data 

typedef struct posix_header
{                              /* byte offset */
  char name[100];               /*   0 */ //name of file
  char mode[8];                 /* 100 */ //file mode
  char uid[8];                  /* 108 */
  char gid[8];                  /* 116 */
  char size[12];                /* 124 */
  char mtime[12];               /* 136 */
  char chksum[8];               /* 148 */
  char typeflag;                /* 156 */
  char linkname[100];           /* 157 */
  char magic[6];                /* 257 */
  char version[2];              /* 263 */
  char uname[32];               /* 265 */
  char gname[32];               /* 297 */
  char devmajor[8];             /* 329 */
  char devminor[8];             /* 337 */
  char prefix[155];             /* 345 */
                                /* 500 */
} header;

void fill_header(char* file_path, header* header);

#endif