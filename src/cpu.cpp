#include "cpu.h"

#include <fmt/core.h>

namespace mos6502 {

    void CPU::reset() {
        pc = 0;
        sp = 0;
        ac = 0;
        x = 0;
        y = 0;
        *reinterpret_cast<byte*>(&sr) = 0;
    }

    [[nodiscard]] byte CPU::fetch() {
        return memory.read(pc++);
    }

    [[nodiscard]] word CPU::fetchWord() {
        const word w = memory.readWord(pc);
        pc += 2;
        return w;
    }

    void CPU::push(const byte value) {
        memory.write(0x100 + sp--, value);
    }

    void CPU::pushWord(const word value) {
        push(value >> 8);
        push(value & 0xFF);
    }

    byte CPU::pop() {
        return memory.read(0x100 + ++sp);
    }

    word CPU::popWord() {
        return pop() | (pop() << 8);
    }

    CPU::instruction CPU::decode(const byte opcode) {
        return instructions[opcode];
    }

    cycles CPU::execute(const instruction operation) {
        return (this->*operation)();
    }

    void CPU::load(const Program& program) {
        reset();

        memory.write(program.entryPoint, program.code);

        pc = program.entryPoint;
        sp = 0xFF;
    }

    void CPU::run() {
        long cycles = 0;

        while (true) {
            const byte opcode = fetch();

            if (opcode == 0x00) break;

            const auto operation = decode(opcode);
            cycles += execute(operation);
        }

        fmt::println("Execution took {} cycles", cycles);
    }

    void CPU::run(const Program& program)  {
        load(program);

        run();
    }

} // mos6502