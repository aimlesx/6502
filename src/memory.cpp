#include "memory.h"

#include <ranges>

#include <fmt/core.h>

namespace mos6502 {

    Memory::Memory(const byte pages) {
        memory = std::vector<byte>(pages * 256);
    }

    byte Memory::read(const address addr) const {
        return memory[addr];
    }

    [[nodiscard]] word Memory::readWord(const address addr) const {
        return memory[addr] | (memory[addr + 1] << 8);
    }

    void Memory::write(const address addr, const byte value) {
        memory[addr] = value;
    }

    void Memory::writeWord(const address addr, const word value) {
        memory[addr] = value & 0xFF;
        memory[addr + 1] = value >> 8;
    }

    void Memory::write(const address addr, const std::vector<byte>& data) {
        for (auto [i, b] : data | std::views::enumerate) {
            write(addr + i, b);
        }
    }

    [[nodiscard]] byte Memory::getSize() const {
        return memory.size();
    }

    [[nodiscard]] byte Memory::getPageCount() const {
        return memory.size() / 256;
    }

    void Memory::print(byte page) const  {
        const word start = page << 8;

        fmt::print("{:-^47}\n", fmt::format(" 0x{:02X} ", page));
        for (byte line = 0; line < 16; ++line) {
            for (byte i = 0; i < 16; ++i) {
                fmt::print("{:02X} ", read(start + line * 16 + i));
            }
            fmt::print("\n");
        }
    }
} // mos6502