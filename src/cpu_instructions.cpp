#include <stdexcept>

#include "cpu.h"

namespace mos6502 {

    bool isSamePage(const address a, const address b) {
        return a & 0xFF00 == b & 0xFF00;
    }

    bool isNegative(const byte value) {
        return value & 0b10000000;
    }

#pragma region Transfer Instructions

    void CPU::lda(const byte value) {
        ac = value;
        sr.z = ac == 0;
        sr.n = isNegative(ac);
    }

    cycles CPU::lda_imm() {
        lda(fetch());
        return 2;
    }

    cycles CPU::lda_zp() {
        lda(memory.read(fetch()));
        return 3;
    }

    cycles CPU::lda_zp_x() {
        lda(memory.read(fetch() + x));
        return 4;
    }

    cycles CPU::lda_abs() {
        lda(memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::lda_abs_x() {
        const auto addr = fetchWord();
        lda(memory.read(addr + x));
        return isSamePage(addr, addr + x) ? 4 : 5;
    }

    cycles CPU::lda_abs_y() {
        const auto addr = fetchWord();
        lda(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 4 : 5;
    }

    cycles CPU::lda_ind_x() {
        lda(memory.read(memory.readWord(fetch() + x)));
        return 6;
    }

    cycles CPU::lda_ind_y() {
        const auto addr = memory.readWord(fetch());
        lda(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 5 : 6;
    }

    void CPU::ldx(const byte value) {
        x = value;
        sr.z = x == 0;
        sr.n = isNegative(x);
    }

    cycles CPU::ldx_imm() {
        ldx(fetch());
        return 2;
    }

    cycles CPU::ldx_zp() {
        ldx(memory.read(fetch()));
        return 3;
    }

    cycles CPU::ldx_zp_y() {
        ldx(memory.read(fetch() + y));
        return 4;
    }

    cycles CPU::ldx_abs() {
        ldx(memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::ldx_abs_y() {
        const auto addr = fetchWord();
        ldx(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 4 : 5;
    }

    void CPU::ldy(const byte value) {
        y = value;
        sr.z = y == 0;
        sr.n = isNegative(y);
    }

    // Add implementations for ldy_ methods
    cycles CPU::ldy_imm() {
        ldy(fetch());
        return 2;
    }

    cycles CPU::ldy_zp() {
        ldy(memory.read(fetch()));
        return 3;
    }

    cycles CPU::ldy_zp_x() {
        ldy(memory.read(fetch() + x));
        return 4;
    }

    cycles CPU::ldy_abs() {
        ldy(memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::ldy_abs_x() {
        const auto addr = fetchWord();
        ldy(memory.read(addr + x));
        return isSamePage(addr, addr + x) ? 4 : 5;
    }

    cycles CPU::sta_zp() {
        memory.write(fetch(), ac);
        return 3;
    }

    cycles CPU::sta_zp_x() {
        memory.write(fetch() + x, ac);
        return 4;
    }

    cycles CPU::sta_abs() {
        memory.write(fetchWord(), ac);
        return 4;
    }

    cycles CPU::sta_abs_x() {
        memory.write(fetchWord() + x, ac);
        return 5;
    }

    cycles CPU::sta_abs_y() {
        memory.write(fetchWord() + y, ac);
        return 5;
    }

    cycles CPU::sta_ind_x() {
        memory.write(memory.readWord(fetch() + x), ac);
        return 6;
    }

    cycles CPU::sta_ind_y() {
        memory.write(memory.readWord(fetch()) + y, ac);
        return 6;
    }

    cycles CPU::stx_zp() {
        memory.write(fetch(), x);
        return 3;
    }

    cycles CPU::stx_zp_y() {
        memory.write(fetch() + y, x);
        return 4;
    }

    cycles CPU::stx_abs() {
        memory.write(fetchWord(), x);
        return 4;
    }

    cycles CPU::sty_zp() {
        memory.write(fetch(), y);
        return 3;
    }

    cycles CPU::sty_zp_x() {
        memory.write(fetch() + x, y);
        return 4;
    }

    cycles CPU::sty_abs() {
        memory.write(fetchWord(), y);
        return 4;
    }

    cycles CPU::tax() {
        x = ac;
        sr.z = x == 0;
        sr.n = isNegative(x);
        return 2;
    }

    cycles CPU::tay() {
        y = ac;
        sr.z = y == 0;
        sr.n = isNegative(y);
        return 2;
    }

    cycles CPU::tsx() {
        x = sp;
        sr.z = x == 0;
        sr.n = isNegative(x);
        return 2;
    }

    cycles CPU::txa() {
        ac = x;
        sr.z = ac == 0;
        sr.n = isNegative(ac);
        return 2;
    }

    cycles CPU::txs() {
        sp = x;
        return 2;
    }

    cycles CPU::tya() {
        ac = y;
        sr.z = ac == 0;
        sr.n = isNegative(ac);
        return 2;
    }

#pragma endregion
#pragma region Stack Instructions

    cycles CPU::pha() {
        push(ac);
        return 3;
    }

    cycles CPU::php() {
        push(*reinterpret_cast<byte*>(&sr) | 0b00110000);
        return 3;
    }

    cycles CPU::pla() {
        ac = pop();
        sr.z = ac == 0;
        sr.n = isNegative(ac);
        return 4;
    }

    cycles CPU::plp() {
        *reinterpret_cast<byte*>(&sr) = pop() & 0b11001111;
        return 4;
    }

#pragma endregion
#pragma region Decrements & Increments

    cycles CPU::dec_zp() {
        const auto addr = fetch();
        const auto value = memory.read(addr) - 1;
        memory.write(addr, value);
        sr.z = value == 0;
        sr.n = isNegative(value);
        return 5;
    }

    cycles CPU::dec_zp_x() {
        const auto addr = fetch() + x;
        const auto value = memory.read(addr) - 1;
        memory.write(addr, value);
        sr.z = value == 0;
        sr.n = isNegative(value);
        return 6;
    }

    cycles CPU::dec_abs() {
        const auto addr = fetchWord();
        const auto value = memory.read(addr) - 1;
        memory.write(addr, value);
        sr.z = value == 0;
        sr.n = isNegative(value);
        return 6;
    }

    cycles CPU::dec_abs_x() {
        const auto addr = fetchWord() + x;
        const auto value = memory.read(addr) - 1;
        memory.write(addr, value);
        sr.z = value == 0;
        sr.n = isNegative(value);
        return 7;
    }

    cycles CPU::dex() {
        x--;
        sr.z = x == 0;
        sr.n = isNegative(x);
        return 2;
    }

    cycles CPU::dey() {
        y--;
        sr.z = y == 0;
        sr.n = isNegative(y);
        return 2;
    }

    cycles CPU::inc_zp() {
        const auto addr = fetch();
        const auto value = memory.read(addr) + 1;
        memory.write(addr, value);
        sr.z = value == 0;
        sr.n = isNegative(value);
        return 5;
    }

    cycles CPU::inc_zp_x() {
        const auto addr = fetch() + x;
        const auto value = memory.read(addr) + 1;
        memory.write(addr, value);
        sr.z = value == 0;
        sr.n = isNegative(value);
        return 6;
    }

    cycles CPU::inc_abs() {
        const auto addr = fetchWord();
        const auto value = memory.read(addr) + 1;
        memory.write(addr, value);
        sr.z = value == 0;
        sr.n = isNegative(value);
        return 6;
    }

    cycles CPU::inc_abs_x() {
        const auto addr = fetchWord() + x;
        const auto value = memory.read(addr) + 1;
        memory.write(addr, value);
        sr.z = value == 0;
        sr.n = isNegative(value);
        return 7;
    }

    cycles CPU::inx() {
        x++;
        sr.z = x == 0;
        sr.n = isNegative(x);
        return 2;
    }

    cycles CPU::iny() {
        y++;
        sr.z = y == 0;
        sr.n = isNegative(y);
        return 2;
    }

#pragma endregion
#pragma region Arithmetic Operations

    void CPU::adc(const byte value) {
        const auto result = ac + value + sr.c;
        sr.c = result > 0xFF;
        sr.z = (result & 0xFF) == 0;
        sr.v = (~(ac ^ value) & (ac ^ result) & 0x80) != 0;
        sr.n = isNegative(result);
        ac = result;
    }

    cycles CPU::adc_imm() {
        adc(fetch());
        return 2;
    }

    cycles CPU::adc_zp() {
        adc(memory.read(fetch()));
        return 3;
    }

    cycles CPU::adc_zp_x() {
        adc(memory.read(fetch() + x));
        return 4;
    }

    cycles CPU::adc_abs() {
        adc(memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::adc_abs_x() {
        const auto addr = fetchWord();
        adc(memory.read(addr + x));
        return isSamePage(addr, addr + x) ? 4 : 5;
    }

    cycles CPU::adc_abs_y() {
        const auto addr = fetchWord();
        adc(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 4 : 5;
    }

    cycles CPU::adc_ind_x() {
        adc(memory.read(memory.readWord(fetch() + x)));
        return 6;
    }

    cycles CPU::adc_ind_y() {
        const auto addr = memory.readWord(fetch());
        adc(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 5 : 6;
    }

    void CPU::sbc(const byte value) {
        const auto result = ac - value - !sr.c;
        sr.c = result < 0;
        sr.z = (result & 0xFF) == 0;
        sr.v = ((ac ^ result) & (ac ^ value) & 0x80) != 0;
        sr.n = isNegative(result);
        ac = result;
    }

    cycles CPU::sbc_imm() {
        sbc(fetch());
        return 2;
    }

    cycles CPU::sbc_zp() {
        sbc(memory.read(fetch()));
        return 3;
    }

    cycles CPU::sbc_zp_x() {
        sbc(memory.read(fetch() + x));
        return 4;
    }

    cycles CPU::sbc_abs() {
        sbc(memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::sbc_abs_x() {
        const auto addr = fetchWord();
        sbc(memory.read(addr + x));
        return isSamePage(addr, addr + x) ? 4 : 5;
    }

    cycles CPU::sbc_abs_y() {
        const auto addr = fetchWord();
        sbc(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 4 : 5;
    }

    cycles CPU::sbc_ind_x() {
        sbc(memory.read(memory.readWord(fetch() + x)));
        return 6;
    }

    cycles CPU::sbc_ind_y() {
        const auto addr = memory.readWord(fetch());
        sbc(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 5 : 6;
    }

#pragma endregion
#pragma region Logical Operations

    void CPU::and_(const byte value) {
        ac &= value;
        sr.z = ac == 0;
        sr.n = isNegative(ac);
    }

    cycles CPU::and_imm() {
        and_(fetch());
        return 2;
    }

    cycles CPU::and_zp() {
        and_(memory.read(fetch()));
        return 3;
    }

    cycles CPU::and_zp_x() {
        and_(memory.read(fetch() + x));
        return 4;
    }

    cycles CPU::and_abs() {
        and_(memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::and_abs_x() {
        const auto addr = fetchWord();
        and_(memory.read(addr + x));
        return isSamePage(addr, addr + x) ? 4 : 5;
    }

    cycles CPU::and_abs_y() {
        const auto addr = fetchWord();
        and_(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 4 : 5;
    }

    cycles CPU::and_ind_x() {
        and_(memory.read(memory.readWord(fetch() + x)));
        return 6;
    }

    cycles CPU::and_ind_y() {
        const auto addr = memory.readWord(fetch());
        and_(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 5 : 6;
    }

    void CPU::eor_(const byte value) {
        ac ^= value;
        sr.z = ac == 0;
        sr.n = isNegative(ac);
    }

    cycles CPU::eor_imm() {
        eor_(fetch());
        return 2;
    }

    cycles CPU::eor_zp() {
        eor_(memory.read(fetch()));
        return 3;
    }

    cycles CPU::eor_zp_x() {
        eor_(memory.read(fetch() + x));
        return 4;
    }

    cycles CPU::eor_abs() {
        eor_(memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::eor_abs_x() {
        const auto addr = fetchWord();
        eor_(memory.read(addr + x));
        return isSamePage(addr, addr + x) ? 4 : 5;
    }

    cycles CPU::eor_abs_y() {
        const auto addr = fetchWord();
        eor_(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 4 : 5;
    }

    cycles CPU::eor_ind_x() {
        eor_(memory.read(memory.readWord(fetch() + x)));
        return 6;
    }

    cycles CPU::eor_ind_y() {
        const auto addr = memory.readWord(fetch());
        eor_(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 5 : 6;
    }

    void CPU::ora_(const byte value) {
        ac |= value;
        sr.z = ac == 0;
        sr.n = isNegative(ac);
    }

    cycles CPU::ora_imm() {
        ora_(fetch());
        return 2;
    }

    cycles CPU::ora_zp() {
        ora_(memory.read(fetch()));
        return 3;
    }

    cycles CPU::ora_zp_x() {
        ora_(memory.read(fetch() + x));
        return 4;
    }

    cycles CPU::ora_abs() {
        ora_(memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::ora_abs_x() {
        const auto addr = fetchWord();
        ora_(memory.read(addr + x));
        return isSamePage(addr, addr + x) ? 4 : 5;
    }

    cycles CPU::ora_abs_y() {
        const auto addr = fetchWord();
        ora_(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 4 : 5;
    }

    cycles CPU::ora_ind_x() {
        ora_(memory.read(memory.readWord(fetch() + x)));
        return 6;
    }

    cycles CPU::ora_ind_y() {
        const auto addr = memory.readWord(fetch());
        ora_(memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 5 : 6;
    }

#pragma endregion
#pragma region Shift & Rotate Instructions

    byte CPU::asl_(const byte value) {
        sr.c = value & 0b10000000;
        const auto result = value << 1;
        sr.z = result == 0;
        sr.n = isNegative(result);
        return result;
    }

    cycles CPU::asl_acc() {
        ac = asl_(ac);
        return 2;
    }

    cycles CPU::asl_zp() {
        const auto addr = fetch();
        const auto value = memory.read(addr);
        memory.write(addr, asl_(value));
        return 5;
    }

    cycles CPU::asl_zp_x() {
        const auto addr = fetch() + x;
        const auto value = memory.read(addr);
        memory.write(addr, asl_(value));
        return 6;
    }

    cycles CPU::asl_abs() {
        const auto addr = fetchWord();
        const auto value = memory.read(addr);
        memory.write(addr, asl_(value));
        return 6;
    }

    cycles CPU::asl_abs_x() {
        const auto addr = fetchWord() + x;
        const auto value = memory.read(addr);
        memory.write(addr, asl_(value));
        return 7;
    }

    byte CPU::lsr_(const byte value) {
        sr.c = value & 0b00000001;
        const auto result = value >> 1;
        sr.z = result == 0;
        sr.n = isNegative(result);
        return result;
    }

    cycles CPU::lsr_acc() {
        ac = lsr_(ac);
        return 2;
    }

    cycles CPU::lsr_zp() {
        const auto addr = fetch();
        const auto value = memory.read(addr);
        memory.write(addr, lsr_(value));
        return 5;
    }

    cycles CPU::lsr_zp_x() {
        const auto addr = fetch() + x;
        const auto value = memory.read(addr);
        memory.write(addr, lsr_(value));
        return 6;
    }

    cycles CPU::lsr_abs() {
        const auto addr = fetchWord();
        const auto value = memory.read(addr);
        memory.write(addr, lsr_(value));
        return 6;
    }

    cycles CPU::lsr_abs_x() {
        const auto addr = fetchWord() + x;
        const auto value = memory.read(addr);
        memory.write(addr, lsr_(value));
        return 7;
    }

    byte CPU::rol_(const byte value) {
        const auto result = (value << 1) | sr.c;
        sr.c = value & 0b10000000;
        sr.z = result == 0;
        sr.n = isNegative(result);
        return result;
    }

    cycles CPU::rol_acc() {
        ac = rol_(ac);
        return 2;
    }

    cycles CPU::rol_zp() {
        const auto addr = fetch();
        const auto value = memory.read(addr);
        memory.write(addr, rol_(value));
        return 5;
    }

    cycles CPU::rol_zp_x() {
        const auto addr = fetch() + x;
        const auto value = memory.read(addr);
        memory.write(addr, rol_(value));
        return 6;
    }

    cycles CPU::rol_abs() {
        const auto addr = fetchWord();
        const auto value = memory.read(addr);
        memory.write(addr, rol_(value));
        return 6;
    }

    cycles CPU::rol_abs_x() {
        const auto addr = fetchWord() + x;
        const auto value = memory.read(addr);
        memory.write(addr, rol_(value));
        return 7;
    }

    byte CPU::ror_(const byte value) {
        const auto result = (value >> 1) | (sr.c << 7);
        sr.c = value & 0b00000001;
        sr.z = result == 0;
        sr.n = isNegative(result);
        return result;
    }

    cycles CPU::ror_acc() {
        ac = ror_(ac);
        return 2;
    }

    cycles CPU::ror_zp() {
        const auto addr = fetch();
        const auto value = memory.read(addr);
        memory.write(addr, ror_(value));
        return 5;
    }

    cycles CPU::ror_zp_x() {
        const auto addr = fetch() + x;
        const auto value = memory.read(addr);
        memory.write(addr, ror_(value));
        return 6;
    }

    cycles CPU::ror_abs() {
        const auto addr = fetchWord();
        const auto value = memory.read(addr);
        memory.write(addr, ror_(value));
        return 6;
    }

    cycles CPU::ror_abs_x() {
        const auto addr = fetchWord() + x;
        const auto value = memory.read(addr);
        memory.write(addr, ror_(value));
        return 7;
    }

#pragma endregion
#pragma region Flag Instructions

    cycles CPU::clc() {
        sr.c = false;
        return 2;
    }

    cycles CPU::cld() {
        sr.d = false;
        return 2;
    }

    cycles CPU::cli() {
        sr.i = false;
        return 2;
    }

    cycles CPU::clv() {
        sr.v = false;
        return 2;
    }

    cycles CPU::sec() {
        sr.c = true;
        return 2;
    }

    cycles CPU::sed() {
        sr.d = true;
        return 2;
    }

    cycles CPU::sei() {
        sr.i = true;
        return 2;
    }

#pragma endregion
#pragma region Comparisons

    void CPU::cmp_(const byte reg, const byte value) {
        sr.c = reg >= value;
        sr.z = reg == value;
        sr.n = isNegative(reg - value);
    }

    cycles CPU::cmp_imm() {
        cmp_(ac, fetch());
        return 2;
    }

    cycles CPU::cmp_zp() {
        cmp_(ac, memory.read(fetch()));
        return 3;
    }

    cycles CPU::cmp_zp_x() {
        cmp_(ac, memory.read(fetch() + x));
        return 4;
    }

    cycles CPU::cmp_abs() {
        cmp_(ac, memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::cmp_abs_x() {
        const auto addr = fetchWord();
        cmp_(ac, memory.read(addr + x));
        return isSamePage(addr, addr + x) ? 4 : 5;
    }

    cycles CPU::cmp_abs_y() {
        const auto addr = fetchWord();
        cmp_(ac, memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 4 : 5;
    }

    cycles CPU::cmp_ind_x() {
        cmp_(ac, memory.read(memory.readWord(fetch() + x)));
        return 6;
    }

    cycles CPU::cmp_ind_y() {
        const auto addr = memory.readWord(fetch());
        cmp_(ac, memory.read(addr + y));
        return isSamePage(addr, addr + y) ? 5 : 6;
    }

    cycles CPU::cpx_imm() {
        cmp_(x, fetch());
        return 2;
    }

    cycles CPU::cpx_zp() {
        cmp_(x, memory.read(fetch()));
        return 3;
    }

    cycles CPU::cpx_abs() {
        cmp_(x, memory.read(fetchWord()));
        return 4;
    }

    cycles CPU::cpy_imm() {
        cmp_(y, fetch());
        return 2;
    }

    cycles CPU::cpy_zp() {
        cmp_(y, memory.read(fetch()));
        return 3;
    }

    cycles CPU::cpy_abs() {
        cmp_(y, memory.read(fetchWord()));
        return 4;
    }

#pragma endregion
#pragma region Conditional Branch Instructions

    cycles CPU::branch_(const bool condition) {
        const auto offset = static_cast<signed char>(fetch());
        const byte startPage = pc >> 8;

        if (condition) {
            pc += offset;
        }

        const bool pageChanged = startPage != pc >> 8;

        return 2 + (condition ? (pageChanged ? 2 : 1) : 0);
    }

    cycles CPU::bcc() {
        return branch_(!sr.c);
    }

    cycles CPU::bcs() {
        return branch_(sr.c);
    }

    cycles CPU::beq() {
        return branch_(sr.z);
    }

    cycles CPU::bmi() {
        return branch_(sr.n);
    }

    cycles CPU::bne() {
        return branch_(!sr.z);
    }

    cycles CPU::bpl() {
        return branch_(!sr.n);
    }

    cycles CPU::bvc() {
        return branch_(!sr.v);
    }

    cycles CPU::bvs() {
        return branch_(sr.v);
    }

#pragma endregion
#pragma region Jumps & Subroutines

    cycles CPU::jmp_abs() {
        pc = fetchWord();
        return 3;
    }

    cycles CPU::jmp_ind() {
        pc = memory.readWord(fetchWord());
        return 5;
    }

    cycles CPU::jsr() {
        const address routine = fetchWord();
        pushWord(pc);
        pc = routine;
        return 6;
    }

    cycles CPU::rts() {
        pc = popWord();
        return 6;
    }

#pragma endregion
#pragma region Interrupts

    cycles CPU::brk() {
        pushWord(pc + 2);
        push(*reinterpret_cast<byte*>(&sr) | 0b00110000);
        sr.i = true;
        pc = memory.readWord(0xFFFE);
        return 7;
    }

    cycles CPU::rti() {
        *reinterpret_cast<byte*>(&sr) = pop() & 0b11001111;
        pc = popWord();
        return 6;
    }

#pragma endregion
#pragma region Other Instructions

    cycles CPU::bit_zp() {
        const auto value = memory.read(fetch());
        sr.z = (ac & value) == 0;
        sr.v = value & 0b01000000;
        sr.n = value & 0b10000000;
        return 3;
    }

    cycles CPU::bit_abs() {
        const auto value = memory.read(fetchWord());
        sr.z = (ac & value) == 0;
        sr.v = value & 0b01000000;
        sr.n = value & 0b10000000;
        return 4;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    cycles CPU::nop() { // NOLINT(*-convert-member-functions-to-static)
        return 2;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    cycles CPU::illegal() { // NOLINT(*-convert-member-functions-to-static)
        throw std::runtime_error("Illegal instruction");
        return 0;
    }

#pragma endregion

// define static field instructions from header file
const CPU::instruction CPU::instructions[256] = {
    &brk,       &ora_ind_x, &illegal, &illegal, &illegal,   &ora_zp,    &asl_zp,    &illegal, &php, &ora_imm,   &asl_acc,   &illegal, &illegal,     &ora_abs,   &asl_abs,   &illegal,
    &bpl,       &ora_ind_y, &illegal, &illegal, &illegal,   &ora_zp_x,  &asl_zp_x,  &illegal, &clc, &ora_abs_y, &illegal,   &illegal, &illegal,     &ora_abs_x, &asl_abs_x, &illegal,
    &jsr,       &and_ind_x, &illegal, &illegal, &bit_zp,    &and_zp,    &rol_zp,    &illegal, &plp, &and_imm,   &rol_acc,   &illegal, &bit_abs,     &and_abs,   &rol_abs,   &illegal,
    &bmi,       &and_ind_y, &illegal, &illegal, &illegal,   &and_zp_x,  &rol_zp_x,  &illegal, &sec, &and_abs_y, &illegal,   &illegal, &illegal,     &and_abs_x, &rol_abs_x, &illegal,
    &rti,       &eor_ind_x, &illegal, &illegal, &illegal,   &eor_zp,    &lsr_zp,    &illegal, &pha, &eor_imm,   &lsr_acc,   &illegal, &jmp_abs,     &eor_abs,   &lsr_abs,   &illegal,
    &bvc,       &eor_ind_y, &illegal, &illegal, &illegal,   &eor_zp_x,  &lsr_zp_x,  &illegal, &cli, &eor_abs_y, &illegal,   &illegal, &illegal,     &eor_abs_x, &lsr_abs_x, &illegal,
    &rts,       &adc_ind_x, &illegal, &illegal, &illegal,   &adc_zp,    &ror_zp,    &illegal, &pla, &adc_imm,   &ror_acc,   &illegal, &jmp_ind,     &adc_abs,   &ror_abs,   &illegal,
    &bvs,       &adc_ind_y, &illegal, &illegal, &illegal,   &adc_zp_x,  &ror_zp_x,  &illegal, &sei, &adc_abs_y, &illegal,   &illegal, &illegal,     &adc_abs_x, &ror_abs_x, &illegal,
    &illegal,   &sta_ind_x, &illegal, &illegal, &sty_zp,    &sta_zp,    &stx_zp,    &illegal, &dey, &illegal,   &txa,       &illegal, &sty_abs,     &sta_abs,   &stx_abs,   &illegal,
    &bcc,       &sta_ind_y, &illegal, &illegal, &sty_zp_x,  &sta_zp_x,  &stx_zp_y,  &illegal, &tya, &sta_abs_y, &txs,       &illegal, &illegal,     &sta_abs_x, &illegal,   &illegal,
    &ldy_imm,   &lda_ind_x, &ldx_imm, &illegal, &ldy_zp,    &lda_zp,    &ldx_zp,    &illegal, &tay, &lda_imm,   &tax,       &illegal, &ldy_abs,     &lda_abs,   &ldx_abs,   &illegal,
    &bcs,       &lda_ind_y, &illegal, &illegal, &ldy_zp_x,  &lda_zp_x,  &ldx_zp_y,  &illegal, &clv, &lda_abs_y, &tsx,       &illegal, &ldy_abs_x,   &lda_abs_x, &ldx_abs_y, &illegal,
    &cpy_imm,   &cmp_ind_x, &illegal, &illegal, &cpy_zp,    &cmp_zp,    &dec_zp,    &illegal, &iny, &cmp_imm,   &dex,       &illegal, &cpy_abs,     &cmp_abs,   &dec_abs,   &illegal,
    &bne,       &cmp_ind_y, &illegal, &illegal, &illegal,   &cmp_zp_x,  &dec_zp_x,  &illegal, &cld, &cmp_abs_y, &illegal,   &illegal, &illegal,     &cmp_abs_x, &dec_abs_x, &illegal,
    &cpx_imm,   &sbc_ind_x, &illegal, &illegal, &cpx_zp,    &sbc_zp,    &inc_zp,    &illegal, &inx, &sbc_imm,   &nop,       &illegal, &cpx_abs,     &sbc_abs,   &inc_abs,   &illegal,
    &beq,       &sbc_ind_y, &illegal, &illegal, &illegal,   &sbc_zp_x,  &inc_zp_x,  &illegal, &sed, &sbc_abs_y, &illegal,   &illegal, &illegal,     &sbc_abs_x, &inc_abs_x, &illegal
};

} // mos6502