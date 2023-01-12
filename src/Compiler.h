#pragma once
#include "VM.h"
#include "Scanner.h"

namespace Clap
{
    class Parser
    {
    public:

        Parser(Scanner* scanner)
            : m_Scanner(scanner) {}
        void Advance();
        void Expression();

        void ErrorAtCurrent(const std::string& message);
        void Error(const std::string& message);
        void ErrorAt(Token& token, const std::string& message);

        void Consume(TokenType type, const std::string& message);

        bool HadError = false;
        bool PanicMode = false;

        Token Current = {TokenType::ERROR, "", 0};
        Token Previous = {TokenType::ERROR, "", 0};
    private:
        Scanner* m_Scanner;
    };

    struct CompilerData //Uses Scoped Pointers since it gets reset everytime Compile is pressed.
    {
        Parser* CompilerParser = nullptr;
        Scanner* CompilerScanner = nullptr;
        Chunk* CompilingChunk = nullptr;
    };

    class Compiler{
    public:
        static bool Compile(const std::string& source, Chunk* chunk);

        static void EmitByte(uint8_t byte);
        static void EmitReturn();

        template<typename... Args> 
        static void EmitByte(uint8_t byte, Args ...rest)
        {
            EmitByte(byte);
            EmitByte(rest...);
        }
    };

    
}