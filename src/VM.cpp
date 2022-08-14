#include "VM.h"

namespace Clap
{
    InterpretResult VM::Interpret(const std::string& source)
    {
        Chunk chunk;
        InitCunk(&chunk);

        if(!Compile(source, chunk))
        {
            return InterpretResult::COMPILE_ERROR;
        }

        m_Chunk = chunk;
        m_PC = m_Chunk.Begin();

        InterpretResult result = Run();

        return result;
    }
    InterpretResult VM::Run()
    {
        #define READ_BYTE() *(m_PC++)
        #define READ_CONSTANT() (m_Chunk->GetConstant(READ_BYTE()))
        #define READ_CONSTANT_LONG() (m_Chunk->GetConstant(READ_BYTE() | READ_BYTE() << 8 | READ_BYTE() << 16)) 
        #define BINARY_OP(op) \
            do { \
            double b = pop(); \
            double a = pop(); \
            push(a op b); \
            } while (false)
        #ifdef DEBUG_TRACE_EXECUTION
            std::cout<<"          ";
            for (Value* slot = m_Stack; slot < m_StackTop; slot++) {
                std::cout<<"[ "<<*slot<<" ]";
            }
            std::cout<<'\n';
            disassembleInstruction(m_Chunk,
                                (int)(m_PC - m_Chunk.Begin()));
        #endif

        for(;;)
        {
            uint8_t instruction;
            switch(instruction = READ_BYTE())
            {
                case OpCode::RETURN:
                    return InterpretResult::OK;
                break;
                case OpCode::ADD: BINARY_OP(+); break;
                case OpCode::SUB: BINARY_OP(-); break;
                case OpCode::DIV: BINARY_OP(/); break;
                case OpCode::MUL: BINARY_OP(*); break;
                case OpCode::MOD: BINARY_OP(%); break;
                case OpCode::BSR: BINARY_OP(>>); break;
                case OpCode::BSL: BINARY_OP(<<); break;
                case OpCode::XOR: BINARY_OP(^); break;
                case OpCode::BAND: BINARY_OP(&); break;
                case OpCode::BOR: BINARY_OP(|); break;
                case OpCode::NEGATE:
                    Push(-Pop());
                break;
                case OpCode::CONSTANT:
                    Push(READ_CONSTANT());
                break;
                case OpCode::CONSTANT_LONG:
                    Push(READ_CONSTANT_LONG());
                break;
            }
        }
        #undef READ_BYTE
        #undef READ_CONSTANT
        #undef READ_CONSTANT_LONG
        #undef BINARY_OP
    }
    
    void VM::ResetStack()
    {
        m_StackTop = m_Stack;
    }
    void VM::Push(Value value)
    {
        *m_StackTop = value;
        m_StackTop++;
    }
    Value VM::Pop()
    {
        m_StackTop--;
        return *m_StackTop;
    }
}