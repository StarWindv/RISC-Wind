#ifndef OPCODE_H
#define OPCODE_H


enum ALUVersion {  // 4-bit
    /**
     * 普通操作, 按理说没有这个也可以
     * 但是加上后明显会让判断归属更加简单
     * 起码不会为了一个歧义的Code而去查表
     */
    BASE     = 0b0000,
    SCALAR   = 0b0001,  // ALU_S
    SIMD_8   = 0b0010,  // ALU_8
    SIMD_16  = 0b0011,  // ALU_16
    SIMD_32  = 0b0100,  // ALU_32
    SIMD_64  = 0b0101,  // ALU_64
    SIMD_128 = 0b0110,  // ALU_128
    SIMD_256 = 0b0111,  // ALU_256
    SIMD_512 = 0b1000,  // ALU_512
};


enum BaseOp {  // 4-bit
    HALT  = 0b0000,
    LOAD  = 0b0001,
    ALOAD = 0b0010,
    STR   = 0b0011,
    STRAI = 0b0100,
    STRA  = 0b0101,
    COPY  = 0b0110,
    JMP   = 0b0111,
    JZ    = 0b1000,
    JNZ   = 0b1001
};


// MUST 4-bit,
// 你不能保证以后没有扩展,
// 不能保证 SIMD 也是 3-bit
enum ALU_S {
     SADD_S  = 0b0000,
     SSUB_S  = 0b0001,
    NANDT_S  = 0b0010,
      LGR_S  = 0b0011,
     LSFT_S  = 0b0100,
    LRSFT_S  = 0b0101
};


#endif

