#include "Debug.h"

#include <iostream>
#include <iomanip>

namespace Clap
{
    void DisassembleChunk(const Chunk& chunk, std::string name)
    {
        std::cout << "___ " << name << " ____" << std::endl;
        for(int offset = 0; i < chunk.Size();)
        {
            offset = DisassembleInstruction(chunk, offset);
        }
    }

    int SimpleInstruction(std::string name, int offset)
    {
        std::cout<<name<<std::endl;
        return offset + 1;
    }

    int ConstantInstruction(std::string name, const Chunk& chunk, int offset)
    {
        uint8_t constant = chunk[offset+1];
        std::cout<<std::setw(16)<<name<< " " <<std::setw(4)<< constant << " '";
        std::cout<<chunk.GetConstant(constant)<<"'"<<std::endl;
        return offset + 2;
    }
    int ConstantLongInstruction(std::string name, const Chunk& chunk, int offset)
    {
        uint32_t constant = chunk[offset+1] |
                           (chunk[offset+2] << 8) |
                           (chunk[offset+3] << 16);
        std::cout<<std::setw(16)<<name<< " " <<std::setw(4)<< constant << " '";
        std::cout<<chunk.GetConstant(constant)<<"'"<<std::endl;
        return offset + 4;
    }

    int DisassembleInstruction(const Chunk& chunk, int offset)
    {
        std::cout<<std::setw(4)<<offset<<' ';
        uint32_t line = chunk.GetLine(offset);

        if(offset > 0 && line == chunk.GetLine(chunk, offset-1))
            std::cout<<"   | ";
        else
            std::cout<<std::setw(4)<<line<<' ';

        uint8_t instruction = chunk[offset];

        switch (instruction)
        {
            case OpCode::ADD:  return SimpleInstruction("OP_ADD", offset); break;
            case OpCode::SUB:  return SimpleInstruction("OP_SUB", offset); break;
            case OpCode::DIV:  return SimpleInstruction("OP_DIV", offset); break;
            case OpCode::MUL:  return SimpleInstruction("OP_MUL", offset); break;
            case OpCode::MOD:  return SimpleInstruction("OP_MOD", offset); break;
            case OpCode::BSR:  return SimpleInstruction("OP_BSR", offset); break;
            case OpCode::BSL:  return SimpleInstruction("OP_BSL", offset); break;
            case OpCode::XOR:  return SimpleInstruction("OP_XOR", offset); break;
            case OpCode::BAND: return SimpleInstruction("OP_BAND", offset); break;
            case OpCode::BOR:  return SimpleInstruction("OP_BOR", offset); break;
            case OpCode::NEGATE:
                return SimpleInstruction("OP_NEG", offset);
            case OpCode::CONSTANT:
                return ConstantInstruction("OP_CONSTANT", chunk, offset);
            case OpCode::CONSTANT_LONG:
                return ConstantLongInstruction("OP_CONSTANT_LONG", chunk, offset);
            case OpCode::RETURN:
                return SimpleInstruction("OP_RETURN", offset);
            default:
                std::cout<<"Unknown opcode " << instruction << std::endl;
                return offset + 1;
        }
    }
}