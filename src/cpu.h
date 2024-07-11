#pragma once

#include "types.h"
#include "memory.h"
#include "program.h"

namespace mos6502 {

class CPU {
    word pc{};
    byte sp{};
    byte ac{};
    byte x{};
    byte y{};
    struct {
        bool c : 1{};
        bool z : 1{};
        bool i : 1{};
        bool d : 1{};
        bool b : 1{};
        bool : 1;
        bool v : 1{};
        bool n : 1{};
    } sr{};

    Memory memory;

    void reset();

    [[nodiscard]] byte fetch();
    [[nodiscard]] word fetchWord();

#pragma region Transfer Instructions

    void lda(byte value);
    cycles lda_imm();
    cycles lda_zp();
    cycles lda_zp_x();
    cycles lda_abs();
    cycles lda_abs_x();
    cycles lda_abs_y();
    cycles lda_ind_x();
    cycles lda_ind_y();

    void ldx(byte value);
    cycles ldx_imm();
    cycles ldx_zp();
    cycles ldx_zp_y();
    cycles ldx_abs();
    cycles ldx_abs_y();

    void ldy(byte value);
    cycles ldy_imm();
    cycles ldy_zp();
    cycles ldy_zp_x();
    cycles ldy_abs();
    cycles ldy_abs_x();

    cycles sta_zp();
    cycles sta_zp_x();
    cycles sta_abs();
    cycles sta_abs_x();
    cycles sta_abs_y();
    cycles sta_ind_x();
    cycles sta_ind_y();

    cycles stx_zp();
    cycles stx_zp_y();
    cycles stx_abs();

    cycles sty_zp();
    cycles sty_zp_x();
    cycles sty_abs();

    cycles tax();
    cycles tay();
    cycles tsx();
    cycles txa();
    cycles txs();
    cycles tya();

#pragma endregion
#pragma region Stack Instructions

    void push(byte value);
    void pushWord(word value);
    byte pop();
    word popWord();

    cycles pha();
    cycles php();
    cycles pla();
    cycles plp();

#pragma endregion
#pragma region Decrements & Increments

    cycles dec_zp();
    cycles dec_zp_x();
    cycles dec_abs();
    cycles dec_abs_x();

    cycles dex();
    cycles dey();

    cycles inc_zp();
    cycles inc_zp_x();
    cycles inc_abs();
    cycles inc_abs_x();

    cycles inx();
    cycles iny();

#pragma endregion
#pragma region Arithmetic Instructions

    void adc(byte value);
    cycles adc_imm();
    cycles adc_zp();
    cycles adc_zp_x();
    cycles adc_abs();
    cycles adc_abs_x();
    cycles adc_abs_y();
    cycles adc_ind_x();
    cycles adc_ind_y();

    void sbc(byte value);
    cycles sbc_imm();
    cycles sbc_zp();
    cycles sbc_zp_x();
    cycles sbc_abs();
    cycles sbc_abs_x();
    cycles sbc_abs_y();
    cycles sbc_ind_x();
    cycles sbc_ind_y();

#pragma endregion
#pragma region Logical Operations

    void and_(byte value);
    cycles and_imm();
    cycles and_zp();
    cycles and_zp_x();
    cycles and_abs();
    cycles and_abs_x();
    cycles and_abs_y();
    cycles and_ind_x();
    cycles and_ind_y();

    void eor_(byte value);
    cycles eor_imm();
    cycles eor_zp();
    cycles eor_zp_x();
    cycles eor_abs();
    cycles eor_abs_x();
    cycles eor_abs_y();
    cycles eor_ind_x();
    cycles eor_ind_y();

    void ora_(byte value);
    cycles ora_imm();
    cycles ora_zp();
    cycles ora_zp_x();
    cycles ora_abs();
    cycles ora_abs_x();
    cycles ora_abs_y();
    cycles ora_ind_x();
    cycles ora_ind_y();

#pragma endregion
#pragma region Shift & Rotate Instructions

    byte asl_(byte value);
    cycles asl_acc();
    cycles asl_zp();
    cycles asl_zp_x();
    cycles asl_abs();
    cycles asl_abs_x();

    byte lsr_(byte value);
    cycles lsr_acc();
    cycles lsr_zp();
    cycles lsr_zp_x();
    cycles lsr_abs();
    cycles lsr_abs_x();

    byte rol_(byte value);
    cycles rol_acc();
    cycles rol_zp();
    cycles rol_zp_x();
    cycles rol_abs();
    cycles rol_abs_x();

    byte ror_(byte value);
    cycles ror_acc();
    cycles ror_zp();
    cycles ror_zp_x();
    cycles ror_abs();
    cycles ror_abs_x();

#pragma endregion
#pragma region Flag Instructions

    cycles clc();
    cycles cld();
    cycles cli();
    cycles clv();
    cycles sec();
    cycles sed();
    cycles sei();

#pragma endregion
#pragma region Comparisons

    void cmp_(byte reg, byte value);

    cycles cmp_imm();
    cycles cmp_zp();
    cycles cmp_zp_x();
    cycles cmp_abs();
    cycles cmp_abs_x();
    cycles cmp_abs_y();
    cycles cmp_ind_x();
    cycles cmp_ind_y();

    cycles cpx_imm();
    cycles cpx_zp();
    cycles cpx_abs();

    cycles cpy_imm();
    cycles cpy_zp();
    cycles cpy_abs();

#pragma endregion
#pragma region Conditional Branch Instructions

    cycles branch_(bool condition);

    cycles bcc();
    cycles bcs();
    cycles beq();
    cycles bmi();
    cycles bne();
    cycles bpl();
    cycles bvc();
    cycles bvs();

#pragma endregion
#pragma region Jumps & Subroutines

    cycles jmp_abs();
    cycles jmp_ind();

    cycles jsr();

    cycles rts();

#pragma endregion
#pragma region Interrupts

    cycles brk();
    cycles rti();

#pragma endregion
#pragma region Other Instructions

    cycles bit_zp();
    cycles bit_abs();
    cycles nop();

    cycles illegal();

#pragma endregion

    // define jumptable as a 2d array
    using instruction = cycles (CPU::*)();
    static const instruction instructions[];

    static instruction decode(byte opcode);
    cycles execute(instruction operation);

public:
    explicit CPU(const byte memoryPages = 255): memory(memoryPages) {}

    Memory& getMemory() { return memory; }

    void load(const Program& program);
    void run();
    void run(const Program& program);
};

} // mos6502