#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef enum {
  VM_READY,
  VM_WIP,
  VM_FINISHED,
} vm_state;

struct {
  u_int64_t ip;

  u_int64_t *stack_top;
  u_int64_t *memory;
  u_int64_t *stack_pointer;

  vm_state state;
} vm;

typedef enum uint64_t {
  OP_FINISH = 0x00,
  OP_PUSH_I = 0x01,
  OP_ADD_I = 0x02,
  OP_CMP_LESS_THEN_I = 0x03,
  OP_JMP_IF = 0x04,
  OP_DUP = 0x05,
  OP_CMP_GREATER_THEN_I = 0x06,
  OP_CMP_EQUAL_TO_I = 0x07,
  OP_NOT = 0x08,
  OP_SUB_I = 0x09,
  OP_ADD = 0x0A,
  OP_SUB = 0x0B,
  OP_CMP_EQUALS = 0x0C,
  OP_CMP_LESS = 0x0D,
  OP_CMP_GREATER = 0x0E,
  OP_JMP = 0x0F,
  OP_STORE = 0x10,
  OP_RESTORE = 0x11,
} opcode;

void vm_push(uint64_t value) {
  (*vm.stack_pointer) = value;
  vm.stack_pointer += sizeof(uint64_t);
}

uint64_t vm_pop() {
  vm.stack_pointer -= sizeof(uint64_t);

  return *vm.stack_pointer;
}

int main(int argc, char *argv[]) {
  // Read file

  u_int64_t *exe_mem;

  if (argc < 2) {
    printf("Argument required!\n");
    exit(1);
  }

  FILE *exe_file = fopen(argv[1], "r");

  if (exe_file == NULL) {
    printf("File not fond!\n");
    exit(1);
  }

  fseek(exe_file, 0, SEEK_END);

	int exe_len = ftell(exe_file);

	fseek(exe_file, 0, SEEK_SET);

  exe_mem = malloc(exe_len);
  fread(exe_mem, exe_len, 1, exe_file);

  fclose(exe_file);

  // Init VM

  int memory_size = 64 * 1024;

  vm.memory = malloc(memory_size);
  vm.stack_top = calloc(1024, sizeof(u_int64_t));
  vm.stack_pointer = vm.stack_top;
  vm.state = VM_READY;
  vm.ip = 0;

  // Start

  uint64_t op = 0;
  uint8_t op8 = 0;
  uint64_t i_value = 0;

  while (vm.state != VM_FINISHED) {
    op = exe_mem[vm.ip];
    op8 = op >> (8*7);
    i_value = op & (UINT64_MAX >> 8);

    switch (op8) {
      case OP_FINISH: {
        vm.state = VM_FINISHED;
        break;
      }
      case OP_PUSH_I: {
        vm_push(i_value);

        break;
      }
      case OP_ADD_I: {
        uint64_t value = vm_pop();
        vm_push(value + i_value);

        break;
      }
      case OP_SUB_I: {
        uint64_t value = vm_pop();
        vm_push(value - i_value);

        break;
      }
      case OP_NOT: {
        uint64_t value = vm_pop();
        vm_push(value == 0 ? 1 : 0);

        break;
      }
      case OP_ADD: {
        uint64_t b_value = vm_pop();
        uint64_t a_value = vm_pop();
        vm_push(a_value + b_value);

        break;
      }
      case OP_SUB: {
        uint64_t b_value = vm_pop();
        uint64_t a_value = vm_pop();
        vm_push(a_value - b_value);

        break;
      }
      case OP_CMP_EQUALS: {
        uint64_t b_value = vm_pop();
        uint64_t a_value = vm_pop();
        vm_push(a_value == b_value ? 1 : 0);

        break;
      }
      case OP_CMP_GREATER: {
        uint64_t b_value = vm_pop();
        uint64_t a_value = vm_pop();
        vm_push(a_value > b_value ? 1 : 0);

        break;
      }
      case OP_CMP_LESS: {
        uint64_t b_value = vm_pop();
        uint64_t a_value = vm_pop();
        vm_push(a_value < b_value ? 1 : 0);

        break;
      }
      case OP_CMP_LESS_THEN_I: {
        uint64_t value = vm_pop();
        vm_push(value < i_value ? 1 : 0);

        break;
      }
      case OP_CMP_GREATER_THEN_I: {
        uint64_t value = vm_pop();
        vm_push(value > i_value ? 1 : 0);

        break;
      }
      case OP_CMP_EQUAL_TO_I: {
        uint64_t value = vm_pop();
        vm_push(value == i_value ? 1 : 0);

        break;
      }
      case OP_JMP_IF: {
        uint64_t if_value = vm_pop();

        if (if_value > 0) {
          vm.ip = i_value >> 3;
          continue;
        }

        break;
      }
      case OP_JMP: {
        vm.ip = i_value >> 3;
        continue;

        break;
      }
      case OP_DUP: {
        uint64_t value = vm_pop();
        vm_push(value);
        vm_push(value);

        break;
      }
      case OP_STORE: {
        uint64_t value = vm_pop();
        vm.memory[i_value] = value;

        break;
      }
      case OP_RESTORE: {
        vm_push(vm.memory[i_value]);

        break;
      }
      default: {
        printf("Unknown op code %" PRIu64 "\n", op);
        vm.state = VM_FINISHED;

        break;
      }
    }

    vm.ip += 1;
  }

  // Write results

  printf("Finished! Writing results ... ");

  FILE *result_file = fopen("out.bin", "w+");

  fwrite(vm.memory, memory_size, 1, result_file);

  fclose(result_file);

  printf("OK\n");

  return 0;
}
