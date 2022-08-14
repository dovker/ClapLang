#include "Compiler.h"

namespace Clap
{
    bool Compile(const std::string& source, Chunk* chunk)
    {
        Parser parser;
        Scanner scanner(source);
        parser.Advance();
        parser.Expression();

    }
    void Parser::Advance()
    {

    }
    void Parser::Expression
    {
        
    }
}