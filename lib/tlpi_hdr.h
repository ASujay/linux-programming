#ifndef TLPI_HDR_H
#define TLPI_HDR_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "get_num.h"            // Declares our functions for handling numeric arguments
#include "error_functions.h"    // Declares our error-handling functions
                                
typedef enum {FALSE, TRUE} Boolean;

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
