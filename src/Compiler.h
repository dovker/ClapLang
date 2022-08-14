#pragma once
#include "VM.h"

namespace Clap
{
    bool Compile(const std::string& source, Chunk* chunk);

    class Parser
    {
        void Advance();
        void Expression();
    private:
        Token m_Current;
        Token m_Previous;
    }
}