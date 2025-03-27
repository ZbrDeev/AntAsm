equ file, "example.txt"
equ text, "Hello from AntAsm!"

mov rax, 85     ; Creat Syscall
mov rdi, file   ; Stock file in the register
mov rsi, 420    ; File Mode
syscall         ; Syscall

mov rdi, rax    ; Mov file descriptor in rdi
mov rax, 1      ; Write Syscall
mov rsi, text   ; Stock text in the register
mov rdx, 18     ; Len of text
syscall         ; Syscall