#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char **argv){
    int fd = open(argv[1], O_WRONLY);
    if(fd != -1){
        printf("[PID %ld File \"%s\" already exists\n", (long)getpid(), argv[1]);
        close(fd);
    } else {
        if(errno != ENOENT){
            errExit("open");
        } else {
            printf("[PID %ld] File \"%s\" doesn't exist yet\n", (long)getpid(), argv[1]);
            if(argc > 2){
                sleep(5);
                printf("[PID %ld] Done sleeping\n", (long)getpid());
            }
            fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if(fd == -1)
                errExit("open");
            printf("[PID %ld Created file \"%s\" exclusively\n", (long)getpid(), argv[1]);
        }
    }
    return 0;
}
