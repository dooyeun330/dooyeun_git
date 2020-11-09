#include "userprog/syscall.h"
#include "userprog/process.h"
#include "userprog/exception.h"
#include <stdio.h>
#include <stdlib.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "devices/input.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int sys_num = *(int *)(f->esp);
  switch ( sys_num ) {
	  case SYS_HALT:
		  halt();
		  break;
  	  case SYS_EXIT:
		  if ( is_user_vaddr(f->esp + 4) != true ) exit(-1);
		  exit( *(unsigned int *)(f->esp+4) );
		  break;
	  case SYS_EXEC:
		  if ( is_user_vaddr(f->esp + 4) != true ) exit(-1);
		  f->eax = exec((const char *)*(unsigned int *)(f->esp +4));
		  break;
	  case SYS_WAIT:
                  if ( is_user_vaddr(f->esp + 4) != true ) exit(-1);
		  f->eax = wait( (pid_t)*(unsigned int *)(f->esp+4) );
                  break;
	  case SYS_CREATE:
		  break;
	  case SYS_REMOVE:
		  break;
	  case SYS_OPEN:
		  break;
	  case SYS_FILESIZE:
		  break;
	  case SYS_READ:
		  if ( is_user_vaddr(f->esp+20) && is_user_vaddr(f->esp+24) && is_user_vaddr(f->esp+28) != true) exit(-1);
                  f->eax = read((int)*(int *)(f->esp+20), (void *)*(int *)(f->esp+24), (unsigned)*(int *)(f->esp+28));
		  break;
	  case SYS_WRITE:
		  if ( is_user_vaddr(f->esp+20) && is_user_vaddr(f->esp+24) && is_user_vaddr(f->esp+28) != true) exit(-1);
		  f->eax = write((int)*(int *)(f->esp+20), (void *)*(int *)(f->esp+24), (unsigned)*(int *)(f->esp+28));
		  break;
	  case SYS_SEEK:
		  break;
	  case SYS_TELL:
		  break;
	  case SYS_CLOSE:
	  	  break;
	  default : 
		  break;
  }
  //thread_exit ();
}

void halt(void){
	shutdown_power_off();
}

void exit(int status){
	printf("%s: exit(%d)\n", thread_name(), status);
	thread_current()->e_status = status;
	thread_exit();
}
pid_t exec(const char *cmd_line){
	return process_execute(cmd_line);
}
int wait (pid_t pid){
	return process_wait(pid);
}
int read(int fd, void *buffer, unsigned size){
	int i;
	if ( fd == 0 ) {
		for ( i = 0 ; i < (int)size ; ++i ) *(unsigned char *)(buffer+i) = input_getc();
		return size;
	}
	else return -1;
}
int write(int fd, const void *buffer, unsigned size){
	if ( fd == 1 ) {
		putbuf(buffer, size);
		return size;
	}
	else return -1;
}
