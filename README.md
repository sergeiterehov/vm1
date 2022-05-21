## VM1 Stack Virtual Machine

- 64-bit arch.
- Little Endian

## Compile & Compile & Run

```sh
gcc main.c && node test.js
```

## NodeJS Compiling

`test1.js` contains the source code for machine. `asm1.js` is the compiler module.

## Instructions example

```
PUSH_I 5256
loop:
DUP
CMP_LESS_THEN_I 5273
NOT
JMP_IF loop_exit
ADD_I 1
JMP loop
loop_exit:
PUSH_I 153
SUB
FINISH
```

## Output

Result stack from machine will be written into `out.bin`.