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

C equivalent

```c
uint64_t main() {
  uint64_t a = 0x1488;

  while (a < 0x1499) {
    a += 1;
  }

  a -= 0x99;

  return a;
}
```

## Output

Result stack from machine will be written into `out.bin` (Little Endian).

```
RESULT STACK:  <Buffer 00 14 00 00 00 00 00 00>
```