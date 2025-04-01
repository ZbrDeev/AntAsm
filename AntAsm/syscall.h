#ifndef SYSCALL_H
#define SYSCALL_H

#include "run.h"

#define SYS_WRITE 1
#define SYS_OPEN 2
#define SYS_CLOSE 3
#define SYS_EXIT 60
#define SYS_KILL 62
#define SYS_CHDIR 80
#define SYS_FCHDIR 81
#define SYS_RENAME 82
#define SYS_MKDIR 83
#define SYS_RMDIR 84
#define SYS_CREAT 85

#if defined(__linux__) || defined(__APPLE__)
int callSyscallLinux(struct RegisterEmu *register_emu);
#endif

#endif // SYSCALL_H
