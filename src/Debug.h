#pragma once

#include "Common.h"

namespace Clap
{
    void DisassembleChunk(Chunk& chunk, std::string name);
    int DisassembleInstruction(Chunk& chunk, int offset);
}