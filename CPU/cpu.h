#ifndef CPU_H
#define CPU_H


#include <stdint.h>
#include <stdbool.h>

#include "./opcode.h"
#include "./register.h"
#include "./inst.h"
#include "./mmu.h"


struct CPU;
typedef struct CPU CPU_t;

struct CPU {
    uint64_t reg_s[16];
    MMU_t    mmu;
    bool     is_halt;
};


void reset_cpu(CPU_t *cpu);
void step_cpu (CPU_t *cpu);
void run_cpu  (CPU_t *cpu);


#endif // !CPU_H

