#ifndef LEXER_H
#define LEXER_H


enum class TokenType {
    Identifier,
    Number,
    Declaration_keyword,
    Keyword,
    Operator,
    Punctuation,
    Error
};

struct Token {
    TokenType type;
    std::string value;
    Token(TokenType type, const std::string& value);
};

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::string getTokenTypeName(const TokenType& token);
    std::vector<Token> tokenize();
private:
    TokenType getTokenType(const std::string& value);
    void removeComments();
    std::string source;
};



#endif