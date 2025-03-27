; Create a variable called msg equal to "Hello, World!"
equ msg, "Hello, World!"

mov rax, 1      ; Write Syscall
mov rdi, 1      ; Write into stdout
mov rsi, msg    ; Stock msg in the register
mov rdx, 13     ; Len of the msg
syscall         ; Syscall