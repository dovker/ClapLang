#include "Common.h"
#include "Scanner.h"

namespace Clap
{
    bool ClapLang::s_HadError = false;

    std::vector<Token> Scanner::ScanTokens()
    {
        CleanComments();
        while (!End())
        {
            ScanToken();
        }
        AddToken(TokenType::ENDOFFILE);
        return m_Tokens; 
    }

    void Scanner::ScanToken()
    {
        SkipWhitespace();
        m_Start = m_Current;

        char c = Advance();

        if(IsAlpha(c)) { Identifier(); return; }
        if(IsDigit(c)) { Number(); return; }
        switch (c)
        {
            case '(': AddToken(TokenType::LEFT_PAREN); break;
            case ')': AddToken(TokenType::RIGHT_PAREN); break;
            case '{': AddToken(TokenType::LEFT_BRACE); break;
            case '}': AddToken(TokenType::RIGHT_BRACE); break;
            case '[': AddToken(TokenType::LEFT_BRACKET); break;
            case ']': AddToken(TokenType::RIGHT_BRACKET); break;
            case ',': AddToken(TokenType::COMMA); break;
            case '.': AddToken(TokenType::DOT); break;
            case ';': AddToken(TokenType::SEMICOLON); break;
            case ':': AddToken(TokenType::COLON); break;
            case '~': AddToken(TokenType::TILDE); break;
            case '?': AddToken(TokenType::QUERY); break;
            case '$': AddToken(TokenType::DOLLAR_SIGN); break;

            case '+':
                if (Match('='))
                    AddToken(TokenType::PLUS_EQUAL);
                else if (Match('+'))
                    AddToken(TokenType::PLUS_PLUS);
                else AddToken(TokenType::PLUS);
                break;
            case '-':
                if (Match('='))
                    AddToken(TokenType::MINUS_EQUAL);
                else if (Match('-'))
                    AddToken(TokenType::MINUS_MINUS);
                else AddToken(TokenType::MINUS);
                break;
            case '*':
                AddToken(Match('=') ? TokenType::STAR_EQUAL : TokenType::STAR); 
                break;
            case '/': 
                AddToken(Match('=') ? TokenType::SLASH_EQUAL : TokenType::SLASH);
                break;
            case '%': 
                AddToken(Match('=') ? TokenType::PERCENT_EQUAL : TokenType::PERCENT);
                break;
            case '!':
                AddToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;	
            case '=':
                AddToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            case '>':
                if (Match('='))
                    AddToken(TokenType::GREATER_EQUAL);
                else if (Match('>'))
                    AddToken(Match('=') ? TokenType::SHIFT_RIGHT_EQUAL : TokenType::SHIFT_RIGHT);
                else AddToken(TokenType::GREATER);
                break;
            case '<':
                if (Match('='))
                    AddToken(TokenType::LESS_EQUAL);
                else if (Match('<'))
                    AddToken(Match('=') ? TokenType::SHIFT_LEFT_EQUAL : TokenType::SHIFT_LEFT);
                else AddToken(TokenType::LESS);
                break;
            case '&':
                if (Match('&'))
                    AddToken(TokenType::AND);
                else if (Match('='))
                    AddToken(TokenType::AMPERSAND_EQUAL);
                else AddToken(TokenType::AMPERSAND);
                break;
            case '|':
                if (Match('|'))
                    AddToken(TokenType::OR);
                else if (Match('='))
                    AddToken(TokenType::PIPE_EQUAL);
                else AddToken(TokenType::PIPE);
                break;
            case '^': 
                AddToken(Match('=') ? TokenType::CARET_EQUAL : TokenType::CARET); 
                break;
            
            case '"': String(); break;

            case '\0': break;
    
            default: 
                AddToken(ErrorToken("Unexpected character")); 
                break;
        }
    }

    char Scanner::Advance()
    {
        m_Current++;
        return m_Source[m_Current-1];
    }

    Token Scanner::ErrorToken(std::string message)
    {
        return Token(TokenType::ERROR, message, m_Line);
    }

    //void AddToken(TokenType type)
    //{
        //AddToken(type, null);
    //}
    void Scanner::AddToken(TokenType type)
    {
        std::string text = m_Source.substr(m_Start, m_Current - m_Start);
        m_Tokens.push_back(Token(type, text, m_Line));
    }
    void Scanner::AddToken(Token token)
    {
        m_Tokens.push_back(token);
    }

    bool Scanner::End()
    { 
        return m_Current >= m_Source.length();
    }

    bool Scanner::Match(char expected)
    {
        if(End()) return false;
        if(m_Source[m_Current] != expected) return false; //TODO CHECK THIS

        m_Current++;
        return true;
    }

    char Scanner::Peek()
    {
        if(End()) return '\0';
        return m_Source[m_Current];
    }

    char Scanner::PeekNext()
    {
        if(End()) return '\0';
        return m_Source[m_Current+1];
    }

    void Scanner::String()
    {
        while(Peek() != '"' && !End())
        {
            if(Peek() == '\n') m_Line++;
            if(Peek() == '\\') Advance(); //TODO:Implement these escape sequences
            Advance();
        }

        if(End())
        {
            AddToken(ErrorToken("Unterminated string"));
            return;
        }

        Advance();
        AddToken(TokenType::STRING);
    }

    bool Scanner::IsDigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    bool Scanner::IsAlpha(char c)
    {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
                c == '_';
    }

    TokenType Scanner::IdentifierType()
    {
        switch(m_Source[m_Start])
        {
            case 'a': 
                if(CheckKeyword(1, 2, "nd", TokenType::AND) == TokenType::IDENTIFIER)
                {
                    return CheckKeyword(1, 2, "ny", TokenType::ANY);
                }
                else return TokenType::AND;
                break;
            case 'c': 
                if(m_Current - m_Start > 1)
                {
                    switch(m_Source[m_Start + 1])
                    {
                        case 'a': return CheckKeyword(2, 2, "se", TokenType::CASE); break;
                        case 'o': return CheckKeyword(2, 6, "ntinue", TokenType::CONTINUE); break;
                    }
                }
                break;
            case 'd': return CheckKeyword(1, 6, "efault", TokenType::DEFAULT); break;
            case 'e': return CheckKeyword(1, 3, "lse", TokenType::ELSE); break;
            case 'f':
                if(m_Current - m_Start > 1)
                {
                    switch(m_Source[m_Start + 1])
                    {
                        case 'a': return CheckKeyword(2, 3, "lse", TokenType::FALSE); break;
                        case 'o': return CheckKeyword(2, 1, "r", TokenType::FOR); break;
                        case 'n': return CheckKeyword(2, 0, "", TokenType::FN); break;
                    }
                }
                break;
            case 'i': 
                if(m_Current - m_Start > 1)
                {
                    switch(m_Source[m_Start + 1])
                    {
                        case 'f': return CheckKeyword(2, 0, "", TokenType::IF); break;
                        case 'n': return CheckKeyword(2, 0, "", TokenType::IN); break;
                    }
                }
                break;
            case 'n': return CheckKeyword(1, 2, "il", TokenType::NIL); break;
            case 'o': return CheckKeyword(1, 1, "r", TokenType::OR); break;
            case 'p': return CheckKeyword(1, 4, "rint", TokenType::PRINT); break; 
            case 'r': return CheckKeyword(1, 5, "eturn", TokenType::RETURN); break;
            case 's': 
                if(m_Current - m_Start > 1)
                {
                    switch(m_Source[m_Start + 1])
                    {
                        case 'e': return CheckKeyword(2, 2, "lf", TokenType::SELF); break;
                        case 't': return CheckKeyword(2, 4, "atic", TokenType::STATIC); break;
                        case 'w': return CheckKeyword(2, 4, "itch", TokenType::SWITCH); break;
                    }
                }
                break;
            case 't': 
                if(m_Current - m_Start > 1)
                {
                    switch(m_Source[m_Start + 1])
                    {
                        case 'r': return CheckKeyword(2, 2, "ue", TokenType::TRUE); break;
                        case 'y': return CheckKeyword(2, 2, "pe", TokenType::TYPE); break;
                    }
                }
                break;
            case 'v': return CheckKeyword(1, 2, "ar", TokenType::VAR); break;
            case 'w': return CheckKeyword(1, 4, "hile", TokenType::WHILE); break;
        }
        return TokenType::IDENTIFIER;
    }

    TokenType Scanner::CheckKeyword(uint32_t start, uint32_t length, const char* rest, TokenType type)
    {
        if(m_Current - m_Start == start + length &&
            memcmp(m_Source.c_str() + m_Start + start, rest, length) == 0)
        {
            return type;
        }
        return TokenType::IDENTIFIER;
    }

    void Scanner::Identifier()
    {
        while(IsAlpha(Peek()) || IsDigit(Peek())) Advance();
        AddToken(IdentifierType());
    }

    void Scanner::Number()
    {
        while(IsDigit(Peek())) Advance();
        if (Peek() == '.' && IsDigit(PeekNext()))
        {
            Advance();
            while (IsDigit(Peek())) Advance();
        }

        AddToken(TokenType::NUMBER);
    }

    void Scanner::SkipWhitespace()
    {
        for(;;)
        {
            char c = Peek();
            switch(c)
            {
                case ' ':
                case '\r':
                case '\t':
                    Advance();
                    break;
                case '\n':
                    m_Line++;
                    Advance();
                    break;
                default:
                    return;
            }
        }
    }

    void Scanner::CleanComments()
    {
        std::stringstream cleanSource;
        std::stringstream currentSource(m_Source);
        
        bool isBeingCommented = false;
        std::string line;
        while(std::getline(currentSource, line))
        {
            if(line.length() > 0)
            {
                std::string newLine = line;
                int commentStart = 0;
                bool stringLiteral = false;
                int offset = 0;
                for (int i = 0; i < newLine.length() - 1; i++)
                {
                    if(newLine[i] == '"') stringLiteral = !stringLiteral;
                    if(!stringLiteral)
                    {
                        if(isBeingCommented)
                        {
                            if(newLine[i] == '*' && newLine[i + 1] == '/')
                            {
                                isBeingCommented = false;
                                newLine.erase(commentStart, i + 2 - commentStart);
                                offset += i - commentStart;
                                i -= offset;
                                continue;
                            }
                        }
                        if(newLine[i] == '/')
                        {
                            if(newLine[i+1] == '/' && !isBeingCommented)
                            {
                                newLine.erase(i);
                                break;
                            }
                            if(newLine[i+1] == '*')
                            {
                                commentStart = i;
                                isBeingCommented = true;
                            }
                        }
                    }
                }
                if(isBeingCommented) newLine.erase(commentStart); newLine.append("\n"); //Dunno if this is needed
                if(newLine.length() > 0) cleanSource << newLine;
            }
            else cleanSource << line;
        }
        m_Source = cleanSource.str();
    }
}