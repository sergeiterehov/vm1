const asm = require("./asm1");
const fs = require("fs");
const cp = require("child_process");

asm
  .STORE_I(0, 0x1488)
  .RESTORE(0)
  .label("loop")
  .DUP()
  .CMP_LESS_THEN_I(0x1499)
  .NOT()
  .JMP_IF("loop_exit")
  .ADD_I(1)
  .JMP("loop")
  .label("loop_exit")
  .PUSH_I(0x99)
  .SUB()
  .STORE(0)
  .FINISH();

fs.writeFileSync("test.vm1", asm.getVm1File());
fs.writeFileSync("test.asm1", asm.getAsmFile());

console.log("VM: ", String(cp.execSync("./a.out test.vm1")));
console.log("RESULT MEMORY: ", fs.readFileSync("./out.bin"));
