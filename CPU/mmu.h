#ifndef MMU_H
#define MMU_H


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>

#include "./constant.h"


struct MMU;
typedef struct MMU MMU_t;

struct MMU {
    uint8_t  buffer[MAX_MEM_SIZE];
    uint64_t size;
};


static inline void check(MMU_t *m, uint64_t addr, size_t n) {
    if (addr > m->size || n > m->size - addr) {
        /* 防止加法溢出导致回绕 */
        fprintf(stderr, "[Error] MMU OOB: addr=%#"PRIx64" n=%zu size=%zu\n",
                addr, n, m->size);
        abort();
    }
}


static inline uint8_t read_08(MMU_t *m, uint64_t addr) {
    check(m, addr, 1);
    return m->buffer[addr];
}


static inline uint16_t read_16(MMU_t *m, uint64_t addr) {
    check(m, addr, 2);
    return (uint16_t)m->buffer[addr] | ((uint16_t)m->buffer[addr+1] << 8);
}


static inline uint32_t read_32(MMU_t *m, uint64_t addr) {
    check(m, addr, 4);
    return (uint32_t)m->buffer[addr]
        | ((uint32_t)m->buffer[addr+1] << 8)
        | ((uint32_t)m->buffer[addr+2] << 16)
        | ((uint32_t)m->buffer[addr+3] << 24);
}


static inline uint64_t read_64(MMU_t *m, uint64_t addr) {
    check(m, addr, 8);
    uint64_t v = 0;
    for (int i = 0; i < 8; i++) {
        v |= (uint64_t)m->buffer[addr + i] << (8 * i);
    }
    return v;
}


static inline void write_08(MMU_t *m, uint64_t addr, uint8_t v) {
    check(m, addr, 1);
    m->buffer[addr] = v;
}


static inline void write_16(MMU_t *m, uint64_t addr, uint16_t v) {
    check(m, addr, 2);
    m->buffer[addr+0] = (uint8_t)(v      );
    m->buffer[addr+1] = (uint8_t)(v >>  8);
}


static inline void write_32(MMU_t *m, uint64_t addr, uint32_t v) {
    check(m, addr, 4);
    m->buffer[addr+0] = (uint8_t)(v      );
    m->buffer[addr+1] = (uint8_t)(v >>  8);
    m->buffer[addr+2] = (uint8_t)(v >> 16);
    m->buffer[addr+3] = (uint8_t)(v >> 24);
}


static inline void write_64(MMU_t *m, uint64_t addr, uint64_t v) {
    check(m, addr, 8);
    for (int i=0; i<8; i++) {
        m->buffer[addr+i] = (uint8_t)(v >> (8*i));
    }
}


#endif // !MMU_H

