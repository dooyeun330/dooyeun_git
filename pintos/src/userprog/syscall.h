#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

typedef int pid_t;

void syscall_init (void);
void halt(void);
void exit(int );
pid_t exec(const char *);
int wait(pid_t);
int read(int, void *, unsigned);
int write(int, const void*, unsigned);
#endif /* userprog/syscall.h */
