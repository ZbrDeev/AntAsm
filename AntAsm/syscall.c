#include "syscall.h"
#include "bst.h"
#include "run.h"
#include <stdio.h>

#if defined(__linux__)
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

int callSyscallLinux(struct RegisterEmu *register_emu) {
  int64_t reg_A = register_emu->reg_A;

  if (reg_A == SYS_WRITE) {
    struct Bst *value = getValueBst(register_emu->memory, register_emu->reg_SI);

    if (value == NULL || value->string_value == NULL) {
      return 500;
    }

    register_emu->reg_A =
        write(register_emu->reg_DI, value->string_value, register_emu->reg_D);
  } else if (reg_A == SYS_OPEN) {
    struct Bst *value = getValueBst(register_emu->memory, register_emu->reg_DI);

    if (value == NULL || value->string_value == NULL) {
      return 500;
    }

    register_emu->reg_A =
        open(value->string_value, register_emu->reg_SI, register_emu->reg_D);
  } else if (reg_A == SYS_CLOSE) {
    register_emu->reg_A = close(register_emu->reg_DI);
  } else if (reg_A == SYS_EXIT) {
    register_emu->exit = true;
  } else if (reg_A == SYS_KILL) {
    register_emu->reg_A = kill(register_emu->reg_DI, register_emu->reg_SI);
  } else if (reg_A == SYS_CHDIR) {
    struct Bst *value = getValueBst(register_emu->memory, register_emu->reg_DI);

    if (value == NULL || value->string_value == NULL) {
      return 500;
    }

    register_emu->reg_A = chdir(value->string_value);
  } else if (reg_A == SYS_FCHDIR) {
    register_emu->reg_A = fchdir(register_emu->reg_DI);
  } else if (reg_A == SYS_RENAME) {
    struct Bst *value_a =
        getValueBst(register_emu->memory, register_emu->reg_DI);
    struct Bst *value_b =
        getValueBst(register_emu->memory, register_emu->reg_SI);

    if ((value_a == NULL || value_a->string_value == NULL) ||
        (value_b == NULL || value_b->string_value == NULL)) {
      return 500;
    }

    register_emu->reg_A = rename(value_a->string_value, value_b->string_value);
  } else if (reg_A == SYS_MKDIR) {
    struct Bst *value = getValueBst(register_emu->memory, register_emu->reg_DI);

    if (value == NULL || value->string_value == NULL) {
      return 500;
    }

    register_emu->reg_A = mkdir(value->string_value, register_emu->reg_SI);
  } else if (reg_A == SYS_RMDIR) {
    struct Bst *value = getValueBst(register_emu->memory, register_emu->reg_DI);

    if (value == NULL || value->string_value == NULL) {
      return 500;
    }

    register_emu->reg_A = rmdir(value->string_value);
  } else if (reg_A == SYS_CREAT) {
    struct Bst *value = getValueBst(register_emu->memory, register_emu->reg_DI);

    if (value == NULL || value->string_value == NULL) {
      return 500;
    }

    register_emu->reg_A = creat(value->string_value, register_emu->reg_SI);
  } else {
    return 600;
  }

  return 0;
}
#endif
