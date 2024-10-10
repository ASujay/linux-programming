#include <stdarg.h>
#include "../lib/error_functions.h"
#include "../lib/tlpi_hdr.h"
#include "../lib/ename.h"
/* Defines ename and MAX_ENAME */
#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif
static void
terminate(Boolean useExit3)
{
    char *s;
    /* Dump core if EF_DUMPCORE environment variable is defined and
    is a nonempty string; otherwise call exit(3) or _exit(2),
    depending on the value of 'useExit3'. */
    
    s = getenv("EF_DUMPCORE");
    
    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

static void 
outputError(
    Boolean useErr, 
    int err, 
    Boolean flushStdout,
    const char *format,
    va_list ap
){
    #define BUF_SIZE 500

    // buf will have the final error message
    // userMsg will be the custom message to the user
    // errText will be from the system, obtained by using errno
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    // print the custom message to the user
    vsnprintf(userMsg, BUF_SIZE, format, ap);
    
    if(useErr){
        snprintf(
            errText, 
            BUF_SIZE, 
            "[%s %s]",
            (err > 0 && err <= MAX_ENAME ) ? ename[err] : "?UNKOWN?",
            strerror(err) 
        );
    } else {
        snprintf(errText, BUF_SIZE, ":");
    }

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    // there could be an error here so the errno can change
    if(flushStdout)
        fflush(stdout);         // flush any pending stdout
    fputs(buf, stderr);
    fflush(stderr); 
}

void
errMsg(const char *format, ...){
    va_list argList;
    int savedErrno = errno;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    errno = savedErrno;
}

void
errExit(const char *format, ...){
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

void
err_exit(const char *format, ...){
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, FALSE, format, argList);
    va_end(argList);

    terminate(FALSE);
}

void
errExitEN(int errnum, const char *format, ...){
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errnum, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

void
fatal(const char *format, ...){
    va_list argList;
    
    va_start(argList, format);
    outputError(FALSE, 0, TRUE, format, argList);
    va_end(argList);
    
    terminate(TRUE);
}

void
usageErr(const char *format, ...){
    va_list argList;

    /* Flush any pending stdout */
    fflush(stdout);
    
    fprintf(stderr, "Usage: ");
    
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);
    
    fflush(stderr);
    
    exit(EXIT_FAILURE);
}

void
cmdLineErr(const char *format, ...){
    va_list argList;
    
    fflush(stdout);
    /* Flush any pending stdout */
    
    fprintf(stderr, "Command-line usage error: ");
    
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);
    
    fflush(stderr);
    
    exit(EXIT_FAILURE);
    /* In case stderr is not line-buffered */
}