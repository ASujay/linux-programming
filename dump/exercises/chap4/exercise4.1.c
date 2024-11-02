#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "../lib/error_functions.h"
#include "../lib/tlpi_hdr.h"


int 
main(int argc, char **argv){
    // first we check if the command was issued correctly
    if(argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);  

    int inputFd, outputFd, openFlags;
    mode_t filePerms;

    // we try to open file1 and file2 and handle the error if exists
    inputFd = open(argv[1], O_RDONLY);
    if(inputFd == -1)
        errExit("Opening file %s\n", argv[1]);

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    outputFd = open(argv[2], openFlags, filePerms);
    if(outputFd == -1)
        errExit("Opening file %s\n", argv[2]);

    // now we try to read the file
    int buf_size = 1024;
    char buf[buf_size]; 
    int numRead = 0;
    while((numRead = read(inputFd, buf, buf_size)) > 0){
       // we are trying to create the holes that is present in the original file
       // so when we encounter any 0 values when need to seek at the end of the 0 sequence to create holes
       // we do this until we reach EOF in the input file

       // the bytes could be like XX XX XX XX 00 00 00 XX 00 00 00 EOF
       int buf_ptr = 0;
       int last_write_ptr = 0;
       while(buf_ptr < numRead){
           for(; buf[buf_ptr] != 0 && buf_ptr < numRead; buf_ptr++){}
           int bytes_to_write = buf_ptr - last_write_ptr;
           if(write(outputFd, buf + last_write_ptr, bytes_to_write) != bytes_to_write)
               fatal("problem encountered in writing the buffer");
           last_write_ptr = buf_ptr;
           
           // we have stopped at the first encounter of 00
           // we need to increment the buf_ptr until we reach the end of the 00 sequence
           for(; buf[buf_ptr] == 0 && buf_ptr < numRead; buf_ptr++){}
           // now we will seek until the end of 00 sequence
           if(lseek(outputFd, buf_ptr - last_write_ptr, SEEK_CUR) == -1)
               errExit("lseek");
           last_write_ptr = buf_ptr;
       }
    } 

    if(numRead == -1)
        errExit("read");
    
    if(close(inputFd) == -1)
        errExit("Close input");

    if(close(outputFd) == -1)
        errExit("Close output");

    exit(EXIT_SUCCESS);
}
