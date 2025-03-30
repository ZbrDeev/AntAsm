# Supported Opcodes
- mov
- add
- cmp
- or
- jmp
- sub
- imul
- push
- pop
- inc
- dec
- and
- xor
- jo
- jno
- jb
- jnb
- jz
- jnz
- jbe
- ja
- js
- jns
- jp
- jnp
- jl
- jnl
- jle
- jnle
- equ
- syscall

# Supported Registers
| 64-bit | 32-bit | 16-bit | 8 high bits of lower 16 bits | 8-bit | Description                              |
|--------|--------|--------|-------------------------------|-------|------------------------------------------|
| RAX      | RAX    | EAX    | AX     | AH                            | AL    | Accumulator                              |
| RBX      | RBX    | EBX    | BX     | BH                            | BL    | Base                                     |
| RCX      | RCX    | ECX    | CX     | CH                            | CL    | Counter                                  |
| RDX      | RDX    | EDX    | DX     | DH                            | DL    | Data (commonly extends the A register)   |
| RSI      | RSI    | ESI    | SI     | N/A                           | SIL   | Source index for string operations      |
| RDI      | RDI    | EDI    | DI     | N/A                           | DIL   | Destination index for string operations |
| RSP      | RSP    | ESP    | SP     | N/A                           | SPL   | Stack Pointer                           |
| RBP      | RBP    | EBP    | BP     | N/A                           | BPL   | Base Pointer (meant for stack frames)   |
| R8       | R8     | R8D    | R8W    | N/A                           | R8B   | General purpose                         |
| R9       | R9     | R9D    | R9W    | N/A                           | R9B   | General purpose                         |
| R10      | R10    | R10D   | R10W   | N/A                           | R10B  | General purpose                         |
| R11      | R11    | R11D   | R11W   | N/A                           | R11B  | General purpose                         |
| R12      | R12    | R12D   | R12W   | N/A                           | R12B  | General purpose                         |
| R13      | R13    | R13D   | R13W   | N/A                           | R13B  | General purpose                         |
| R14      | R14    | R14D   | R14W   | N/A                           | R14B  | General purpose                         |
| R15      | R15    | R15D   | R15W   | N/A                           | R15B  | General purpose                         |


# Supported Syscalls

## Linux
- SYS_WRITE 1
- SYS_OPEN 2
- SYS_CLOSE 3
- SYS_EXIT 60
- SYS_KILL 62
- SYS_CHDIR 80
- SYS_FCHDIR 81
- SYS_RENAME 82
- SYS_MKDIR 83
- SYS_RMDIR 84
- SYS_CREAT 85

## Windows
### Soon...
