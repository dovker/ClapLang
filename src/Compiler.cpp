#include "Compiler.h"

namespace Clap
{   
    static CompilerData s_Data;

    bool Compiler::Compile(const std::string& source, Chunk* chunk)
    {
        s_Data.CompilerScanner = &Scanner(source);
        s_Data.CompilerParser = &Parser(s_Data.CompilerScanner);
        s_Data.CompilingChunk = chunk;
        
        s_Data.CompilerParser->Advance();
        s_Data.CompilerParser->Expression();
        s_Data.CompilerParser->Consume(TokenType::ENDOFFILE, "Expect end of expression");
        return !s_Data.CompilerParser->HadError;
    }

    void Compiler::EmitByte(uint8_t byte) {
        s_Data.CompilingChunk->Add(byte, s_Data.CompilerParser->Previous.Line);
    }

    void Compiler::EmitReturn()
    {
        EmitByte((uint8_t)OpCode::RETURN);
    }

    void Parser::Advance()
    {
        Previous = Current;

        for (;;) {
            Current = m_Scanner->ScanToken();
            if (Current.Type != TokenType::ERROR) break;

            ErrorAtCurrent(Current.Lexeme);
        }
    }
    void Parser::Expression()
    {
        
    }

    void Parser::ErrorAtCurrent(const std::string& message)
    {
        ErrorAt(Current, message);
    }

    void Parser::Error(const std::string& message)
    {
        ErrorAt(Previous, message);
    }

    void Parser::ErrorAt(Token& token, const std::string& message)
    {
        if (PanicMode) return;
        PanicMode = true;
        std::cerr << "[line " << token.Line << "] Error";

        if (token.Type == TokenType::ENDOFFILE) {
            std::cerr << " at end";
        } else if (token.Type == TokenType::ERROR) {
            // Nothing.
        } else {
            std::cerr << " at " << token.Lexeme;
        }

        std::cerr << ": " << message << '\n';
        HadError = true;
    }

    void Parser::Consume(TokenType type, const std::string& message)
    {
        if (Current.Type == type) {
            Advance();
            return;
        }

        ErrorAtCurrent(message);
    }
}