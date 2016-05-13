#include "userprog/syscall.h"
#include <stdio.h>
#include "devices/input.h"
#include  "threads/malloc.h"
#include "filesys/file.h"
#include <syscall-nr.h>
#include "filesys/filesys.h"
#include "filesys/off_t.h"
#include "threads/synch.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "../threads/thread.h"
#include "userprog/process.h"
#include "../threads/interrupt.h"
#include "../lib/syscall-nr.h"

static void syscall_handler (struct intr_frame *);

#define VADD_USER_BOTTOM ((void *) 0x08084000)

struct lock file_lock;

int args[3];

syscall_init (void){
  lock_init(&file_lock);

  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void parse_args(struct intr_frame * f, int * args, int n){

  int * arg_ptr;
  for(int i = 0 ; i < n ; i++){
    arg_ptr  = (int *) f->esp + i + 1;
    if(!is_user_vaddr((const void *) arg_ptr)){
      exit(-1);
      return -1;
    }
    args[i] = *arg_ptr;
  }
}

int get_kernel_ptr(const void * addr){
  if(!is_user_vaddr(addr)){
    thread_exit(-1);
  }
  void * ptr = pagedir_get_page(thread_current()->pagedir, addr);
  if(!ptr){
    exit(-1);
  }

  return (int) ptr;
}

int handle_exec(struct intr_frame *f){
    if(!args){
      thread_exit(-1);
      return -1;
    }


    parse_args(f, &args[0],1);
    if(!args[0]){
      thread_exit(-1);
      return -1;
    }
    f->eax = exec((const char *) args[0]);
    return 1;
}

int handle_exit(struct intr_frame *f){
  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],1);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  exit(&args[0]);
  return 1;
}

int handle_close(struct intr_frame *f){
  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],1);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  close(&args[0]);
  return 1;
}

int handle_write(struct intr_frame *f){

  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],3);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  args[1] = get_kernel_ptr((const void *) args[1]);
  f->eax = write(args[0], (void *) args[1], (unsigned) args[2]);

  return 1;
}

int handle_halt(struct intr_frame *f){
  halt();
  return 1;
}

int handle_wait(struct intr_frame *f){
  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],1);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  f->eax = wait(args[0]);

  return 1;
}

int handle_create(struct intr_frame *f){
  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],1);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  args[0] = get_kernel_ptr((const void *) args[0]);
  f->eax = open((const char *) args[0]);

  return 1;
}

int handle_remove(struct intr_frame *f){
  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],1);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  args[0] = get_kernel_ptr((const void *) args[0]);
  f->eax = remove((const char *) args[0]);
  return 1;
}

int handle_open(struct intr_frame *f){
  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],1);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  args[0] = get_kernel_ptr((const void *) args[0]);
  f->eax = open((const char *) args[0]);

  return 1;
}

int handle_read(struct intr_frame *f){
  if(!args){
    thread_exit(-1);
    return -1;
  }
  parse_args(f,&args[0],3);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  args[1] = get_kernel_ptr((const void *) args[1]);
  f->eax = read(args[0],(const void * )args[1],(unsigned) args[2]);

  return 1;
}

int handle_get_filesize(struct intr_frame *f){
  int success = 0;
  if(!args){
    thread_exit(-1);
    return -1;
  }

  f->eax = filesize(args[0]);
  return success;
}

int handle_seek(struct intr_frame *f){
  int success = 0;
  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],1);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  seek(args[0],(unsigned) args[1]);

  return success;
}

int handle_tell(struct intr_frame *f){
  int success = 0;
  if(!args){
    thread_exit(-1);
    return -1;
  }

  parse_args(f,&args[0],1);

  if(!args[0]){
    thread_exit(-1);
    return -1;
  }

  f->eax = tell(args[0]);

  return success;
}

int handleCall(struct intr_frame *f){

  if(!is_user_vaddr((const void *) f->esp)){
    exit(-1);
    return -1;
  }

  int success = 0;
  switch(*(int *) f->esp){
    case SYS_EXEC:{
      success = handle_exec(f);
      break;
    }
    case SYS_EXIT:{
      success = handle_exit(f);
      break;
    }
    case SYS_CLOSE:{
      success = handle_close(f);
        break;
    }case SYS_WRITE:{
      success = handle_write(f);
        break;
    }case SYS_HALT:{
      success = handle_halt(f);
        break;
    }case SYS_WAIT:{
        success = handle_wait(f);
        break;
    }case SYS_CREATE:{
        success = handle_create(f);
        break;
    }case SYS_REMOVE:{
      success = handle_remove(f);
        break;
    }case SYS_OPEN:{
      success = handle_open(f);
    }case SYS_READ:{
      success = handle_read(f);
        break;
    }case SYS_FILESIZE:{
      success = handle_get_filesize(f);
        break;
    }case SYS_SEEK:{
      success = handle_seek(f);
        break;
    }case SYS_TELL:{
      success = handle_tell(f);
        break;
    }
  }

  return success;
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  if(!is_user_vaddr((const void *) f->esp)|| (const void *) f->esp < VADD_USER_BOTTOM){
    thread_exit(-1);
  }

  int rval = * (int *) f->esp;
  if(rval < 0 || rval >= 20){
    thread_exit(-1);
  }

  if(!handleCall(f)) {
    thread_exit (-1);
  }

}


/* Reads a byte at user virtual address UADDR.
UADDR must be below PHYS_BASE.
Returns the byte value if successful, -1 if a segfault
occurred. pg 26 */
static int
get_user (const uint8_t *uaddr)
{
  if(!is_user_vaddr(uaddr)){
    return -1;
  }
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
  : "=&a" (result) : "m" (*uaddr));
  return result;
}
/* Writes BYTE to user address UDST.
UDST must be below PHYS_BASE.
Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  if (!is_user_vaddr(udst)){
    return false;
  }
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
  : "=&a" (error_code), "=m" (*udst) : "r" (byte));
  return error_code != -1;
}

bool create(const char * file,unsigned initial_size){
  lock_acquire(&file_lock);
  bool success =  filesys_create(file, initial_size);

  lock_release(&file_lock);
  return success;
}

bool remove(const char * file){
  lock_acquire(&file_lock);
  bool success = filesys_remove(file);
  lock_release(&file_lock);
  return success;
}

int open(const char * file){
  lock_acquire(&file_lock);
  struct file * f = filesys_open(file);
  if(!f){
    lock_release(&file_lock);
    return TID_ERROR;
  }

  struct file * ofile = filesys_open(file);
  if(ofile == NULL){
    return -1;
  }

  struct file_elem * felem = malloc(sizeof(struct file_elem));
  felem->file_name = file;
  felem->file=ofile;
  felem->fd = thread_current()->fd;
  thread_current()->fd++;
  list_push_back(&thread_current()->file_list,&felem->elem);
  lock_release(&file_lock);
  return felem->fd;
}

int filesize(int fd){
  lock_acquire(&file_lock);
  struct file_elem * f;
  f= get_file(fd);
  if(f == NULL){
    lock_release(&file_lock);
    return 0;
  }
  int l = file_length(f->file);
  lock_release(&file_lock);
  return l;
}

int read(int fd, void * buffer, unsigned size){
  off_t bytes = 0;
  if(fd ==1){
    //file input is file out so bad
    return -1;
  }

  lock_acquire(&file_lock);
  if(fd == 0) {
    //file system input so read from keyboard
    int fsize = 0;
    unsigned i;
    uint8_t *fbuf = (uint8_t *) buffer;
    for (i = 0; i < size; i++) {
      fsize += 1;
      fbuf[i] = input_getc();
    }
    lock_release(&file_lock);
    return fsize;

  }else{
    //regular file so read from file
    struct file_elem *f = get_file(fd);
    if(f == NULL){
      lock_release(&file_lock);
      return -1;
    }else{
      bytes = file_read(f->file,buffer,size);
    }
  }
  lock_release(&file_lock);
  return bytes;

}

int write(int fd,const void * buffer, unsigned size){
  int bytes = 0;
  lock_acquire(&file_lock);
  if(fd == 1){
    //write to standard out
    putbuf(buffer,size);
    lock_release(&file_lock);
    return size;
  }else{
    //write to file
    struct file_elem * f = get_file(fd);
    if(f == NULL){
      lock_release(&file_lock);
      return -1;
    }

    bytes = file_write(f->file,buffer,size);
  }
  lock_release(&file_lock);
  return bytes;
}

void seek(int fd, unsigned position){
  lock_acquire(&file_lock);
  struct file_elem * f = get_file(fd);
  if(f != NULL){
    file_seek(f->file,position);
  }
  lock_release(&file_lock);
}

unsigned tell(int fd){
  unsigned byte;
  lock_acquire(&file_lock);
  struct file_elem * f = get_file(fd);
  byte = file_tell(f->file);
  lock_release(&file_lock);
  return byte;
}

void close(int fd){
  lock_acquire(&file_lock);
  //get file
  struct file_elem * f = get_file(fd);
  struct thread * t = thread_current();

  //remove and close for current thread
  list_remove(&f->elem);
  file_close(f->file);
  free(&f->file_name);
  free(f);

  lock_release(&file_lock);
}

int wait(pid_t pid){
  return process_wait(pid);
}

void exit(int status){

  struct thread * t = thread_current();
  if(is_thread_alive(t->parent)){
    t->cp->status = status;
  }
  printf("%s: exit(%d)\n",thread_current()->program,thread_current()->status);
  thread_exit(status);
}

void halt(){
  shutdown_power_off();
}

void check_valid_ptr(const void *vaddr){
  if(vaddr <VADD_USER_BOTTOM){
    exit(-1);
  }
}
pid_t exec (const char *cmd_line)
{
  pid_t pid = process_execute(cmd_line);
  struct child * cp = get_child(pid);
  ASSERT(cp);

  while (cp->status > 0)
  {
    barrier();
  }

  if (cp->status == -1)
  {
    pid = -1;
    return pid;
  }
  return pid;
}

struct file_elem *  get_file(int fd){
  struct thread * t = thread_current();
  struct list_elem * e;
  for(e = list_begin(&t->file_list); e != list_end(&t->file_list);e = list_next(e)){
    struct file_elem * fp = list_entry(e,struct file_elem,elem);
    if(fd == fp->fd){
      return fp;
    }
  }
  return NULL;
}
