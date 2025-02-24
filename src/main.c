#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include "../lib/error_functions.h"
#include "../lib/tlpi_hdr.h"

int 
main(int argc, char **argv){
    bool shouldAppend = false; 
    char *file;
    // first we check if the command was issued correctly
    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s new-file\n", argv[0]);  
    else if(argc == 3){
        if(strcmp(argv[1], "-a") != 0){
            usageErr("invalid command!\n%s new-file\n", argv[0]);
        }
        shouldAppend = true;
        file = argv[2];
    } else {
        file = argv[1];
    }
    int outputFd, openFlags;
    mode_t filePerms;
    if(shouldAppend){
        openFlags = O_APPEND | O_CREAT | O_WRONLY;
    } else {
        openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    }
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    outputFd = open(file, openFlags, filePerms);
    if(outputFd == -1)
        errExit("Opening file %s\n", file);

    // now we try to read the file
    int buf_size = 1024;
    char buf[buf_size]; 
    int numRead = 0;
    while((numRead = read(STDIN_FILENO, buf, buf_size)) > 0){
        //if the read is successful we now try to write to stdout and the file
        if(write(STDOUT_FILENO, buf, numRead) != numRead){
            fatal("couldn't write to the stdout!");
        }

        if(write(outputFd, buf, numRead) != numRead){
            fatal("couldn't write to the stdout!");
        }
    } 

    if(numRead == -1)
        errExit("read");
    
    if(close(outputFd) == -1)
        errExit("Close output");

    exit(EXIT_SUCCESS);
}
