#include "./cpu.h"


void reset_cpu(CPU_t *cpu) {
    for (int i=0; i<16; i++) {
        cpu->reg_s[i] = 0;
    }
    cpu->is_halt = false;
}


void step_cpu(CPU_t *cpu) {
    
}

