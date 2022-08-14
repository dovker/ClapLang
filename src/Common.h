#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>

#include "Scanner.h"

namespace Clap
{
    //TODO: SIMD OPTIMIZATIONS AND VECTORS
    enum class OpCode : uint8_t
    {
        NULL = 0,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        BSR,
        BSL,
        XOR,
        BAND, //BIT AND
        BOR, //BIT OR
        NEG,
        CONSTANT,
        CONSTANT_LONG,
        RETURN,
    };

    struct LineStart
    {
        int Offset;
        uint32_t Line;
    };

    class Chunk
    {
        void Add(uint8_t byte, uint32_t line);
        {
            Code.push_back(byte);

            if(Lines.size() > 0 && Lines[Lines.size() - 1].Line == line)
                return;
            
            Lines.push_back({Size() - 1, line});
        }

        size_t AddConstant(Value value)
        {
            Constants.push_back(value);
            return Constants.size() - 1;
        }
        void WriteConstant(Value value, int line)
        {
            int index = AddConstant(value);
            int(index < 256)
            {
                Add(OpCode::CONSTANT, line);
                Add(index, line);
            } else
            {
                Add(OpCode::CONSTANT_LONG, line);
                Add((uint8_t)(index & 0xff), line);
                Add((uint8_t)(index >> 8) & 0xff), line);
                Add((uint8_t)(index >> 16) & 0xff), line);
            }
        }
        int GetLine(int instruction) 
        {
            int start = 0;
            int end = Lines.size() - 1;

            for (;;) {
                int mid = (start + end) / 2;
                LineStart* line = &Lines[mid];
                if (instruction < line->Offset) 
                    end = mid - 1;
                else if (mid == Lines.size() - 1 || instruction < Lines[mid + 1].Offset) 
                    return line->Line;
                else 
                    start = mid + 1;
            }
        }
        Value GetConstant(size_t index)
        {
            return Constants[index];
        }

        size_t Size()
        {
            return Code.size();
        }

        size_t ConstantSize()
        {
            return Constants.size();
        }

        uint8_t operator [] (size_t offset)
        {
            return Code[offset];
        }

        uint8_t* Begin()
        {
            return &(*Code.begin());
        }
    private:
        std::vector<uint8_t> Code;
        std::vector<Value> Constants;
        std::vector<LineStart> Lines;
    };

    class ClapLang
    {
    public:
        static void Run(std::string source)
        {
            Scanner scanner(source);
            std::vector<Token> tokens = scanner.ScanTokens();
            for(Token t : tokens)
            {
                //std::cout<<t.ToString()<<std::endl;
            }
        }
        static void RunFile(const std::string& path)
        {
            std::ifstream ifs(path);
            std::string content( (std::istreambuf_iterator<char>(ifs) ),
                                 (std::istreambuf_iterator<char>()    ) );
            Run(content);
            if(s_HadError)
                std::exit(1);
        }
        static void RunCommand()
        {
            for(;;)
            {
                std::cout<<">> ";
                std::string line;
                getline(std::cin, line);
                if(line == "") break;
                Run(line);
                s_HadError = false;
            }
        }
        static void Error(int line, std::string message)
        {
            Report(line, "", message); s_HadError = true;
        }
        static void Report(int line, std::string where, std::string message)
        {
            std::cout<<"[Line " << line << "] Error " << where << ": " << message << '\n';
        }
        static bool s_HadError;
    };
} // namespace Clap
