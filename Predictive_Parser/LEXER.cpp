

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include "LEXER.hpp"
using namespace std;




Token:: Token(TokenType type, const std::string& value) : type(type), value(value){}

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::tokenize() { 
          removeComments();
          std::vector<Token> tokens;
          std::regex regex_pattern(
              "(int|"
              "float|"
              "double|"
              "char|"
              "short|"
              "long long|"
              "long|"
              "while|"
              "else if|"
              "if|"
              "else|"
              "[0-9\\+\\*\\/\\&\\?@#$%^-]*[a-zA-Z_][a-zA-Z0-9\\+\\*\\/\\&\\?@#$%^-]*|" 
              "\\d+(\\.\\d+)?(e\\d+)?|"    
              "\\+|-|\\*|\\/|>=|<=|>|<|!=|==|=|!|;|"
              "\\(|\\)|\\{|\\})"
          );

          auto words_begin = std::sregex_iterator(source.begin(), source.end(), regex_pattern);
          auto words_end = std::sregex_iterator();

          for (auto it = words_begin; it != words_end; ++it) {
              std::string token_value = it->str();
              TokenType token_type = getTokenType(token_value);
              tokens.push_back(Token(token_type, token_value));
          }

          return tokens;
      }

TokenType Lexer::getTokenType(const std::string& value) {
    if (value == "else if" || value == "if" || value == "else" || value == "while") {
        return TokenType::Keyword;
    }
    else if (value == "int" || value == "float" || value == "double" || value == "char" || value == "short" || value == "long long" || value == "long") {
              return TokenType::Declaration_keyword;
    }
    else if (std::regex_match(value, std::regex("\\d+(\\.\\d+)?(e\\d+)?"))) {
        return TokenType::Number;
    }
    else if (std::regex_match(value, std::regex("[a-zA-Z_][a-zA-Z0-9_]*"))) { 
        return TokenType::Identifier;
    }
    else if (value == "+" || value == "-" || value == "*" || value == "/" || value == "=" || value == "==" || value == ">" || value == ">=" || value == "<" || value == "<=" || value == "!=") {
        return TokenType::Operator;
    }
    else if (value == ";" || value == "(" || value == ")" || value == "{" || value == "}") {
        return TokenType::Punctuation;
    }
    return TokenType::Error;
}

std::string Lexer::getTokenTypeName(const TokenType & token)
{
    switch (token)
    {
       case TokenType::Identifier:
            return "Identifier";
            break;
       case TokenType::Number:
            return "Number";
            break;
       case TokenType::Declaration_keyword:
            return "Declaration Keyword";
            break;
       case TokenType::Keyword:
            return "Keyword";
            break;
       case TokenType::Operator:
            return "Operator";
            break;
       case TokenType::Punctuation:
            return "Punctuation";
            break;
       case TokenType::Error:
            return "Error";
            break;
    }
}

void Lexer::removeComments() {
    std::regex singleLineRegex("//.*");
    std::regex multiLineRegex("/\\*([^*]|(\\*+[^*/]))*\\*+/");

    std::string result;
    std::sregex_token_iterator iter(source.begin(), source.end(), singleLineRegex, -1);
    std::sregex_token_iterator end;

    while (iter != end) {
        result += *iter++;
    }

    source = std::regex_replace(result, multiLineRegex, "");
}