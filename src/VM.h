#pragma once
#include "Common.h"
#include "Value.h"

#define STACK_MAX 1024
#define DEBUG_TRACE_EXECUTION

namespace Clap
{
    enum class InterpretResult
    {
        OK,
        COMPILE_ERROR,
        RUNTIME_ERROR,
    };

    class VM
    {
    public:
        VM()
        {
            ResetStack();
        }
        InterpretResult Interpret(const std::string& source);
        InterpretResult Run();

    private:
        Chunk m_Chunk;
        uint8_t* m_PC;
        Value m_Stack[STACK_MAX];
        Value* m_StackTop;
        //TODO: Registers
    private:
        void ResetStack();
        void Push(Value value);
        Value Pop();
    };
}