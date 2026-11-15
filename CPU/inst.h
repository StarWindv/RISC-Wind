#ifndef INST_H
#define INST_H 

#include <stdint.h>
#include "./opcode.h"


#define GET_ALU(w)    (((w) >> 60) & 0xF)
#define GET_OP(w)     (((w) >> 56) & 0xF)
#define GET_S1(w)     (((w) >> 52) & 0xF)
#define GET_S2(w)     (((w) >> 48) & 0xF)
#define GET_RD(w)     (((w) >> 44) & 0xF)
#define GET_IMMLEN(w) (((w) >> 40) & 0xF)
#define GET_IMM(w)    ((w) & 0xFFFFFFFFFFULL)   // 40-bit


#define SET_ALU(w, v)    (((w) & ~(0xFULL << 60)) | (((uint64_t)(v) & 0xF) << 60))
#define SET_OP(w, v)     (((w) & ~(0xFULL << 56)) | (((uint64_t)(v) & 0xF) << 56))
#define SET_S1(w, v)     (((w) & ~(0xFULL << 52)) | (((uint64_t)(v) & 0xF) << 52))
#define SET_S2(w, v)     (((w) & ~(0xFULL << 48)) | (((uint64_t)(v) & 0xF) << 48))
#define SET_RD(w, v)     (((w) & ~(0xFULL << 44)) | (((uint64_t)(v) & 0xF) << 44))
#define SET_IMMLEN(w, v) (((w) & ~(0xFULL << 40)) | (((uint64_t)(v) & 0xF) << 40))
#define SET_IMM(w, v)    (((w) & ~(0xFFFFFFFFFFULL)) | ((uint64_t)(v) & 0xFFFFFFFFFFULL))

#define MK_INST(alu_, op_, s1_, s2_, rd_, immlen_, imm_)          \
    ((((uint64_t)(alu_)    & 0xF)          << 60) |               \
     (((uint64_t)(op_)     & 0xF)          << 56) |               \
     (((uint64_t)(s1_)     & 0xF)          << 52) |               \
     (((uint64_t)(s2_)     & 0xF)          << 48) |               \
     (((uint64_t)(rd_)     & 0xF)          << 44) |               \
     (((uint64_t)(immlen_) & 0xF)          << 40) |               \
     (((uint64_t)(imm_)    & 0xFFFFFFFFFFULL)))


struct Inst;
typedef struct Inst Inst_t;

struct Inst {  // 4 * 6 + 40 = 64-bit
    /**
     * 使用的 ALU 版本 (Scalar / ALU_8 / ... 等)
     * 用于扩展未来也许会存在的 SIMD
     */
    uint64_t alu    : 4 ;
    uint64_t  op    : 4 ;
    uint64_t  s1    : 4 ;
    uint64_t  s2    : 4 ;
    uint64_t  rd    : 4 ;

    // 2^15 = 32768
    // 最多可以容纳 32768-bit 长的立即数
    // "STRAI 3, 16 address"
    // 表示向 address 内存入一个长度在 2^3-bit 内
    // 值为 16 的立即数
    uint64_t immlen : 4 ;
    uint64_t   imm  : 40;  // MAX = 1_0995_1162_7775
};
_Static_assert(sizeof(struct Inst) == 8, "Inst must be 8 bytes");

/*
 * 让我想一想, 如果按照这个INST走的话
 * 那么我们一行 "COPY BPR TPR" 就应该写成这样:
 * 0b0000 0b0110 0x000C 0b0000 0x000B 0b0000   0b0000
 * [ ALU] [ op ] [ s1 ] [ s2 ] [ rd ] [immlen] [imm]
 * 看上去似乎是正常的
 * 让我们读一遍它
 * ALU Version: 0, 说明这是一个基础操作
 * OP : 0b0110, 在基础操作里查表得到这是 COPY
 * S1 : 0x000C, 这是 BPR
 * S2 : 0b0000, 因为 COPY 不需要它, 所以跳过它的值
 * RD : 0x000B, 这是 TPR
 * ImmLength: 0b0000, 这条指令不需要立即数
 * Imm: 0b0000, 立即数也是 0, 但是本来也用不到
 * 所以这条指令是合理的、可以表达出来的
 * */

#endif

