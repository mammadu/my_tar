# my_tar

On the terminal, enter make to create the my_tar executable.
My_tar operates very similarly to the tar program. Replace 'tar' with 'my_tar' to have same functionality.
Currently only options c, t, r, u, and x are working.

## Examples

Make the my_tar executable: ```make```

Add files 'a', 'b', and 'c' to archive: ```./my_tar -cf archive.tar a b c```

List contents of archive: ```./my_tar -tf archive.tar```

Append files 'd' and 'e' to archive ```./my_tar: -rf archive.tar d e```

Append newest modified files 'd' and 'e' to archive ```./my_tar: -uf archive.tar d e```

Extract files from archive: ```./my_tar -xf archive.tar```

## Authorized functions

- chmod
- close|(2)
- free(3)
- fstat(2)
- getpwuid
- getgrgid
- getxattr
- listxattr
- lseek(2)
- major
- malloc(3)
- minor
- mkdir(2)
- open(2)
- opendir
- read(2)
- readdir
- readlink
- stat(2)
- lstat(2)
- symlink
- unlink(2)
- time
- ctime
- utime
- write(2)

## Learning Resources

<https://www.gnu.org/software/tar/manual/html_node/tar_toc.html#SEC_Contents> 

<https://www.gnu.org/software/tar/manual/html_node/Blocking.html>

<https://www.mkssoftware.com/docs/man4/tar.4.asp>

## TODO

- [ ] Find out how to extract specific files from archive
- [ ] Create fill_prefix() function
- [ ] Find way to append to an archive consisting of just nulls... maybe investigate is_archive?
