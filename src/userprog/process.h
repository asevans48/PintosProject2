#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

struct argument{
    char * argument;
    uint32_t size;
    int offset;
}arg;

tid_t process_execute (const char *file_name);
int process_wait (tid_t child_tid);
void process_exit (void);
void process_activate (void);
void append_argument(char * arg, struct argument * args,int i);



#endif /* userprog/process.h */
