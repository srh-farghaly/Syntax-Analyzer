
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include "LEXER.hpp"
#include "PARSER.hpp"
using namespace std;


/*
===============================================  CFG ============================================= 
     stmts            ----> stmts stmt | E 
     stmt             ----> while_stmt | if_stmt | assign_stmt | declaration_stmt
     declaration_stmt ----> declaration_keyword definition_stmt
     declaration_keyword--> int | char | float | double | short | long long
     definition_stmt  ----> id = expr; |
                            id;
     while_stmt       ----> while(cond){stmts}
     assign_stmt      ----> id = expr;
     if_stmt          ----> if(cond) stmt |
                            if(cond) stmt else_stmt|
                            if(cond) stmt elseif_stmt
     else_stmt        ----> else stmt 
     elseif_stmt      ----> elseif (cond) stmt elseif_stmt | else_stmt | e
     cond             ----> id op factor 
     expr             ----> expr + term | expr - term | term
     term             ----> term * factor | term / factor | factor
     factor           ----> id | digit | (expr)
     id               ----> a-z|A-Z
     digit            ----> 0|..9
     op               ----> >|>=|<|<=|=|==|!=|+|-|*|/

======================= CFG after left recursion left factoring elimination =======================
     stmts            ----> stmt rest
     rest             ----> stmt rest | E
     stmt             ----> while_stmt | if_stmt | assign_stmt | declaration_stmt
     declaration_stmt ----> declaration_keyword definition_stmt
     declaration_keyword--> int | char | float | double | short | long long
     definition_stmt  ----> id rest4;
     rest4            ----> e | = expr 
     while_stmt       ----> while(cond){stmts}
     assign_stmt      ----> id = expr;
     if_stmt          ----> if(cond)stmt rest3
     rest3            ----> elseif_stmt| else_stmt | E
     else_stmt        ----> else stmt
     elseif_stmt      ----> else if (cond) stmt rest3
     cond             ----> id op factor
     expr             ----> term  Rest1
     Rest1            ----> + term Rest1 | - term Rest1 | E
     term             ----> factor Rest2
     Rest2            ----> * factor Rest2 | / factor Rest2 | E
     factor           ----> id | digit | (expr)
     id               ----> a|..|z|A|..|Z
     digit            ----> 0|..|9
     op               ---->>|>=|<|<=|=|==|!=|+|-|*|/
*/
int main()
{
    int i = 1;
    std::ifstream file("D:\\Predictive_Parser\\Predictive_Parser\\InputFile.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }
    std::string source_code((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    std::cout << "============================ Source Code ============================ \n";
    std::cout << source_code << std::endl;
    

    Lexer lexer(source_code);
    std::vector<Token> tokens = lexer.tokenize();

    cout << endl << endl;
    cout << "============================ Tokens ============================ \n";
    for (const auto& it : tokens)
    {
        std::cout << i << ".  "<< "Value: " << it.value << "             ,Type:  " << static_cast<int>(it.type) << " ---> "<< lexer.getTokenTypeName(it.type) << std::endl;
        i++;
    }

    cout << endl << endl;
    parser parser(tokens);
    Node* root = parser.GetRoot();
    generateParseTreeVisualization(root);


    delete root;
    return 0;
}

