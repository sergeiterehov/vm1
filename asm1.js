const labels = {};
const replace_map = [];
const instructions = [];
const asm_instructions = [];

function uint(value, end = []) {
  let res = [];

  for (let i = end.length; i < 8; i += 1) {
    res.push(value & 0xff);
    value = value >> 8;
  }

  res.push(...end);

  return res;
}

module.exports = {
  FINISH() {
    asm_instructions.push(`FINISH`);
    instructions.push(...uint(0));

    return this;
  },
  PUSH_I(value) {
    asm_instructions.push(`PUSH_I ${value}`);
    instructions.push(...uint(value, [0x01]));

    return this;
  },
  ADD_I(value) {
    asm_instructions.push(`ADD_I ${value}`);
    instructions.push(...uint(value, [0x02]));

    return this;
  },
  SUB_I(value) {
    asm_instructions.push(`SUB_I ${value}`);
    instructions.push(...uint(value, [0x09]));

    return this;
  },
  ADD() {
    asm_instructions.push(`ADD`);
    instructions.push(...uint(0, [0x0A]));

    return this;
  },
  SUB() {
    asm_instructions.push(`SUB`);
    instructions.push(...uint(0, [0x0B]));

    return this;
  },
  NOT() {
    asm_instructions.push(`NOT`);
    instructions.push(...uint(0, [0x08]));

    return this;
  },
  CMP_EQUALS() {
    asm_instructions.push(`CMP_EQUALS`);
    instructions.push(...uint(0, [0x0C]));

    return this;
  },
  CMP_LESS() {
    asm_instructions.push(`CMP_LESS`);
    instructions.push(...uint(0, [0x0D]));

    return this;
  },
  CMP_GREATER() {
    asm_instructions.push(`CMP_GREATER`);
    instructions.push(...uint(0, [0x0E]));

    return this;
  },
  CMP_LESS_THEN_I(value) {
    asm_instructions.push(`CMP_LESS_THEN_I ${value}`);
    instructions.push(...uint(value, [0x03]));

    return this;
  },
  CMP_GREATER_THEN_I(value) {
    asm_instructions.push(`CMP_GREATER_THEN_I ${value}`);
    instructions.push(...uint(value, [0x06]));

    return this;
  },
  CMP_EQUAL_TO_I(value) {
    asm_instructions.push(`CMP_EQUAL_TO_I ${value}`);
    instructions.push(...uint(value, [0x07]));

    return this;
  },
  JMP_IF(label) {
    asm_instructions.push(`JMP_IF ${label}`);
    replace_map.push({ offset: instructions.length, label, length: 7 });

    instructions.push(...uint(0, [0x04]));

    return this;
  },
  JMP(label) {
    asm_instructions.push(`JMP ${label}`);
    replace_map.push({ offset: instructions.length, label, length: 7 });

    instructions.push(...uint(0, [0x0F]));

    return this;
  },
  DUP() {
    asm_instructions.push(`DUP`);
    instructions.push(...uint(0, [0x05]));

    return this;
  },
  STORE(value) {
    asm_instructions.push(`STORE ${value}`);
    instructions.push(...uint(value, [0x10]));

    return this;
  },
  RESTORE() {
    asm_instructions.push(`RESTORE`);
    instructions.push(...uint(0, [0x11]));

    return this;
  },
  label(name) {
    asm_instructions.push(`${name}:`);
    labels[name] = instructions.length;

    return this;
  },
  comment(text) {
    asm_instructions.push(`/* ${text.replace("*/", "* /")} */`);

    return this;
  },
  getVm1File() {
    for (const replace of replace_map) {
      const label_offset = labels[replace.label];

      if (label_offset === undefined) continue;

      const value = uint(label_offset);

      for (let i = 0; i < replace.length; i += 1) {
        instructions[replace.offset + i] = value[i];
      }
    }

    return Buffer.from(instructions);
  },
  getAsmFile() {
    return asm_instructions.join("\n");
  },
};
