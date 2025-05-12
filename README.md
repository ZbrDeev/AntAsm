# AntAsm - An X86_64 Assembler Interpreter Written in C

AntAsm is a lightweight and efficient x86_64 assembler interpreter written in C. It allows users to interpret assembly code in a simulated environment, making it ideal for educational purposes or experimentation with low-level programming concepts.

## Features

- [x] **Interpretation of Assembly Code**: AntAsm can interpret simple assembly code and simulate its execution.
- [ ] **Memory Simulation**: It provides a simulated memory environment where users can view and modify registers and memory.
- [x] **Support for Basic Instructions**: AntAsm supports basic assembly instructions like mov, add, sub, jmp, and more.
- [x] **REPL Support**: AntAsm provides an interactive REPL for testing and executing assembly code in real-time.

## Installation

To get started with AntAsm, follow the steps below:

### Requirements

- C Compiler (GCC or Clang)
- CMake

### Steps

1. **Clone the Repository**

```bash
git clone https://github.com/ZbrDeev/AntAsm.git
cd AntAsm
```

2. **Compile the Code**

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..
ninja -jN
```

## Usage

Supported registers, opcodes and syscalls are in [SUPPORTED.md](https://github.com/ZbrDeev/AntAsm/blob/main/SUPPORTED.md).

Once you have compiled, you can run AntAsm with an assembly file.

```bash
./AntAsm test.asm
```

### Example Assembly Code:

```asm
; test.asm
mov eax, 3
mov ebx, eax
inc ebx
```

## Contributing

If you would like to contribute to AntAsm, feel free to fork the repository, make changes, and create pull requests. All contributions are welcome!

### Steps to Contribute:

1.  Fork the repository.
2.  Make your changes and commit them (`git add . && git commit -m 'Add new feature' && git push origin main`).
3.  Create a new pull request.

## License

AntAsm is open-source software released under the MIT License.
