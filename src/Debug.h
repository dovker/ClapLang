#pragma once

#include "Common.h"

namespace Clap
{
    void DisassembleChunk(const Chunk& chunk, std::string name);
    int DisassembleInstruction(const Chunk& chunk, int offset);
}