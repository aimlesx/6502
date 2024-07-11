#pragma once

#include <vector>

#include "types.h"

namespace mos6502 {

    class Memory {
        std::vector<byte> memory;

    public:
        explicit Memory(byte pages = 255);

        [[nodiscard]] byte read(address addr) const;
        [[nodiscard]] word readWord(address addr) const;

        void write(address addr, byte value);
        void writeWord(address addr, word value);
        void write(address addr, const std::vector<byte>& data);

        [[nodiscard]] byte getSize() const;
        [[nodiscard]] byte getPageCount() const;

        void print(byte page) const;
    };

} // mos6502
