#include "cpu.h"

int main() {
    using namespace mos6502;

    const Program fill{
        {
            0xA2, 0x00, // LDX #$00
            0x86, 0x00, // STX $00

            // loop
            0xE0, 0x0A, // CPX #$0A
            0xF0, 0x07, // BEQ exit
            0x8A,       // TXA
            0x95, 0x10, // STA $10,X
            0xE8,       // INX
            0x4C, 0x04, 0x02, // JMP loop

            // exit
            0x00

        }, 0x0200};

    CPU cpu;
    cpu.run(fill);
    const auto& mem = cpu.getMemory();

    mem.print(0);
    mem.print(2);

    return 0;
}
