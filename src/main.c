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
       
    } 

    if(numRead == -1)
        errExit("read");
    
    if(close(inputFd) == -1)
        errExit("Close input");

    if(close(outputFd) == -1)
        errExit("Close output");

    exit(EXIT_SUCCESS);
}
