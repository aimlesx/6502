#pragma once

#include <vector>

#include "types.h"

namespace mos6502 {

    struct Program {
        std::vector<byte> code;
        word entryPoint;

        explicit Program(std::vector<byte> code, const word entryPoint)
            : code(std::move(code)), entryPoint(entryPoint) {}
    };

} // mos6502