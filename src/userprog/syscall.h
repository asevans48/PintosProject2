#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <stdio.h>

#define NOT_LOADED 0
#define LOAD_SUCCESS 1
#define LOAD_FAIL 2

typedef int pid_t;

void syscall_init (void);
struct file_elem *  get_file(int fd);
void check_valid_ptr(const void *vaddr);
void halt (void);
void exit (int status);
pid_t exec (const char *file);
int wait (pid_t);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
int filesize (int fd);
int read (int fd, void *buffer, unsigned length);
int write (int fd, const void *buffer, unsigned length);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);

/*
int handle_exec(struct intr_frame *f);
int handle_exit(struct intr_frame *f);
int handle_close(struct intr_frame *f);
int handle_write(struct intr_frame *f);
int handle_halt(struct intr_frame *f);
int handle_wait(struct intr_frame *f);
int handle_create(struct intr_frame *f);
int handle_remove(struct intr_frame *f);
int handle_open(struct intr_frame *f);
int handle_read(struct intr_frame *f);
int handle_get_filesize(struct intr_frame *f);
int handle_seek(struct intr_frame *f);
int handle_tell(struct intr_frame *f);
int handleCall(struct intr_frame *f);
 */
struct file_elem *  get_file(int fd);
#endif /* userprog/syscall.h */
