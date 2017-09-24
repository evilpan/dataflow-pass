#include <stdio.h>
#include <assert.h>
#define ST_LOG(format, args...) fprintf(stderr, "ST_RUNTIME: " format"\n", ##args)

// See llvm/IR/instruction.def
#define MAX_NUM 65
static int block_inst[MAX_NUM] = {0};
static int total_inst[MAX_NUM] = {0};

void __visit(unsigned int opcode) {
    assert(opcode < MAX_NUM);
    ++block_inst[opcode];
    ++total_inst[opcode];
}

void __statistics() {
    ST_LOG("=== BEGIN STATISTICS FOR BLOCK ===");
    int i;
    for (i = 0; i < MAX_NUM; i++) {
        if (block_inst[i] != 0) {
            ST_LOG("opcode(%zu):%d", i, block_inst[i]);
            block_inst[i] = 0;
        }
    }
    ST_LOG("=== END STATISTICS FOR BLOCK ===");
}
