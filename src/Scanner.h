#pragma once
#include "Common.h"

namespace Clap
{
    enum class TokenType
    {
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
        COMMA, DOT, SEMICOLON, COLON, QUERY, DOLLAR_SIGN, TILDE,

        PLUS, PLUS_PLUS, PLUS_EQUAL,
        MINUS, MINUS_MINUS, MINUS_EQUAL,
        SLASH, SLASH_EQUAL,
        STAR, STAR_EQUAL,
        PERCENT, PERCENT_EQUAL,
        BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL, SHIFT_RIGHT, SHIFT_RIGHT_EQUAL,
        LESS, LESS_EQUAL, SHIFT_LEFT, SHIFT_LEFT_EQUAL,
        CARET, CARET_EQUAL,t 
        AMPERSAND, AMPERSAND_EQUAL, //AMPERSAND_AMPERSAND,
        PIPE, PIPE_EQUAL, //PIPE_PIPE, 

        IDENTIFIER, STRING, NUMBER,
        
        TYPE, ELSE, FALSE, FN, FOR, IF, NIL, SWITCH, CASE, BREAK,
        PRINT, RETURN, STATIC, SELF, TRUE, VAR, WHILE, CONTINUE, DEFAULT,
        AND, OR, IN, ANY, // UNSIGNED, SHORT, LONG, //INT, NUM, BOOL, STRING
        
        ERROR, ENDOFFILE
    };

    struct Token
    {
        TokenType Type;
        std::string Lexeme; //TODO: make this into a reference to source string
        //void* Literal; TODO: IMPLEMENT THIS
        uint32_t Line;

        Token(TokenType type, std::string lexeme, uint32_t line)
            : Type(type), Lexeme(lexeme), Line(line) {}

        std::string ToString()
        {
            std::stringstream str;
            str << "Token: \"" << Lexeme << "\"" << " Type: " << (int)Type;
            return str.str();
        }
    };

    class Scanner
    {
    public:
        Scanner(std::string source)
            : m_Source(source) {}
        
        std::vector<Token> ScanTokens();
    private:
        std::string m_Source;
        uint32_t m_Line = 1;
        uint32_t m_Start = 0;
        uint32_t m_Current = 0;
        
        std::vector<Token> m_Tokens;

    private: 
        void ScanToken();

        Token ErrorToken(std::string message);
        void AddToken(TokenType type);
        void AddToken(Token token);

        char Advance();
        bool End();
        bool Match(char expected);
        char Peek();
        char PeekNext();

        static bool IsDigit(char c);
        static bool IsAlpha(char c);
        TokenType IdentifierType();
        TokenType CheckKeyword(uint32_t start, uint32_t length, const char* rest, TokenType type);

        void String();
        void Identifier();
        void Number();
        void SkipWhitespace();
        void CleanComments();
    };
} // namespace Clap
