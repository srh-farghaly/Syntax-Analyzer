
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include "LEXER.hpp"
#include "PARSER.hpp"

using namespace std;



int nodeCount = 0;


Node* createNode(const string& label, const string& token = "") {
    Node* node = new Node;
    node->id = nodeCount++;
    node->label = label;
    node->token = token;
    return node;
}


 void parser::nextToken() {
        if (token.size() - 1 == index)
        {
            return;
        }
        else {
            index++;
            Lookahead.value = token[index].value;
            Lookahead.type = token[index].type;
        }
 }

    void parser::stmts(Node* StatementsNode)
    {
        Node* statementNode = createNode("Statement");
        StatementsNode->children.push_back(statementNode);
        stmt(statementNode);

        Node* RestNode = createNode("rest");
        StatementsNode->children.push_back(RestNode);
        rest(RestNode);
    }
    void parser::rest(Node* RestNode)
    {
        if (Lookahead.value == "while" || Lookahead.type == TokenType::Identifier || Lookahead.value == "if" || Lookahead.type == TokenType::Declaration_keyword)
        {
            Node* statementNode = createNode("Statement");
            RestNode->children.push_back(statementNode);
            stmt(statementNode);

            Node* NewRestNode = createNode("rest");
            RestNode->children.push_back(NewRestNode);
            rest(NewRestNode);
        }
        else
        {
            Node* NullNode = createNode("Null");
            RestNode->children.push_back(NullNode);
            return;
        }
    }
    void parser::if_stmt(Node* IfStatementNode)
    {
        Token temp = { TokenType::Keyword ,"if" };
        Node* childNode = nullptr;
        add_children(match(temp), IfStatementNode);


        temp.value = "(";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), IfStatementNode);

        Node* CondNode = createNode("CondStatement");
        IfStatementNode->children.push_back(CondNode);
        cond(CondNode);

        temp.value = ")";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), IfStatementNode);

        Node* StatementNode = createNode("Statement");
        IfStatementNode->children.push_back(StatementNode);
        stmt(StatementNode);

        Node* Rest3Node = createNode("Rest3");
        IfStatementNode->children.push_back(Rest3Node);
        Rest3(Rest3Node);

    }
    void parser::Rest3(Node* Rest3Node) {
        if (Lookahead.value == "else if")
        {
            Node* statementNode = createNode("elseIfStatement");
            Rest3Node->children.push_back(statementNode);
            else_if_stmt(statementNode);
        }
        else if (Lookahead.value == "else")
        {
            Node* statementNode = createNode("elseStatement");
            Rest3Node->children.push_back(statementNode);
            else_stmt(statementNode);
        }
        else {
            Node* NullNode = createNode("Null");
            Rest3Node->children.push_back(NullNode);
        }
    }
    void parser::else_stmt(Node* elseStatement) {

        Token temp = { TokenType::Keyword ,"else" };
        Node* childNode = nullptr;
        add_children(match(temp), elseStatement);

        Node* statementNode = createNode("Statement");
        elseStatement->children.push_back(statementNode);
        stmt(statementNode);

    }
    void parser::else_if_stmt(Node* elseIfStatement) {

        Token temp = { TokenType::Keyword ,"else if" };
        Node* childNode = nullptr;
        add_children(match(temp), elseIfStatement);


        temp.value = "(";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), elseIfStatement);

        Node* CondNode = createNode("CondStatement");
        elseIfStatement->children.push_back(CondNode);
        cond(CondNode);

        temp.value = ")";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), elseIfStatement);

        Node* StatementNode = createNode("Statement");
        elseIfStatement->children.push_back(StatementNode);
        stmt(StatementNode);

        Node* Rest3Node = createNode("Rest3");
        elseIfStatement->children.push_back(Rest3Node);
        Rest3(Rest3Node);

    }
    
    void parser::stmt(Node* statementNode)
    { 
        if (Lookahead.value == "while" )
        {
            Node* whileNode = createNode("whileStatement");
            statementNode->children.push_back(whileNode);
            while_stmt(whileNode);
        }
        else if (Lookahead.value == "if" )
        {
            Node* IfNode = createNode("IfStatement");
            statementNode->children.push_back(IfNode);
            if_stmt(IfNode);
        }
        else if (Lookahead.type == TokenType::Identifier)
        {
            Node* assignNode = createNode("AssignStatement");
            statementNode->children.push_back(assignNode);
            assign_stmt(assignNode);
        }
        else if (Lookahead.type == TokenType::Declaration_keyword)
        {

            Node* DeclarationStatement = createNode("DeclarationStatement");
            statementNode->children.push_back(DeclarationStatement);


            Node* DeclarationKeyword = createNode("DeclarationKeyword");
            DeclarationStatement->children.push_back(DeclarationKeyword);

            Token temp = { TokenType::Declaration_keyword ,Lookahead.value };
            add_children(match(temp), DeclarationKeyword);

            Node* DefinitionNode = createNode("DefinitionStatement");
            DeclarationStatement->children.push_back(DefinitionNode);
            definition_stmt(DefinitionNode);
        }                                 
        else
            error();
    }
    void parser::definition_stmt(Node* DefinitionNode)
    {
        Token temp = { TokenType::Identifier ,Lookahead.value };
        Node* IdentifierNode = createNode("Identifier");
        DefinitionNode->children.push_back(IdentifierNode);
        add_children(match(temp), IdentifierNode);

        Node* Rest4Node = createNode("Rest4");
        DefinitionNode->children.push_back(Rest4Node);
        Rest4(Rest4Node);

        temp.value = ";";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), DefinitionNode);

    }
    void  parser::Rest4(Node* Rest4Node) {
        if (Lookahead.value == "=")
        {
            Token temp = { TokenType::Operator ,"="};
            add_children(match(temp), Rest4Node); 


            Node* exprNode = createNode("expression");
            Rest4Node->children.push_back(exprNode);
            expr(exprNode);       
        }
        else {
            Node* NullNode = createNode("Null");
            Rest4Node->children.push_back(NullNode);
            return;
        }
    }
    void parser::assign_stmt(Node* assignNode)
    {
        
        Token temp = { TokenType::Identifier ,Lookahead.value };
        Node* IdentifierNode = createNode("Identifier");
        assignNode->children.push_back(IdentifierNode);
        add_children(match(temp), IdentifierNode);

        temp.value = "=";
        temp.type = TokenType::Operator;
        add_children(match(temp), assignNode);

        Node* exprNode = createNode("expression");
        assignNode->children.push_back(exprNode);
        expr(exprNode);

        temp.value = ";";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), assignNode);
    }

    void parser::add_children(Node* childNode, Node* ParentNode)
    {
        if (childNode != nullptr)
            ParentNode->children.push_back(childNode);
    }
    void parser::while_stmt(Node* whileStatementNode) {
        Token temp = { TokenType::Keyword ,"while" };
        Node* childNode = nullptr;
        add_children(match(temp), whileStatementNode);


        temp.value = "(";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), whileStatementNode);

        Node* CondNode = createNode("CondStatement");
        whileStatementNode->children.push_back(CondNode);
        cond(CondNode);

        temp.value = ")";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), whileStatementNode);

        temp.value = "{";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), whileStatementNode);

        Node* StatementsNode = createNode("Statements");
        whileStatementNode->children.push_back(StatementsNode);
        stmts(StatementsNode);

        temp.value = "}";
        temp.type = TokenType::Punctuation;
        add_children(match(temp), whileStatementNode);
    }
    void parser::cond(Node* ConditionNode) {
        Token temp = { TokenType::Identifier,Lookahead.value };

        Node* IdentifierNode = createNode("Identifier");
        ConditionNode->children.push_back(IdentifierNode);
        add_children(match(temp), IdentifierNode);

        Node* operatorNode = createNode("operator");
        ConditionNode->children.push_back(operatorNode);
        op(operatorNode);

        Node* factorNode = createNode("factor");
        ConditionNode->children.push_back(factorNode);
        factor(factorNode);
    }

    void parser::expr(Node* exprNode)
    {
        Node* termNode = createNode("Term");
        exprNode->children.push_back(termNode);
        term(termNode);

        Node* Rest1Node = createNode("Rest1");
        exprNode->children.push_back(Rest1Node);
        Rest1(Rest1Node);
    }
    void parser::term(Node* termNode)
    {
        Node* factorNode = createNode("Factor");
        termNode->children.push_back(factorNode);
        factor(factorNode);

        Node* Rest2Node = createNode("Rest2");
        termNode->children.push_back(Rest2Node);
        Rest2(Rest2Node);
    }
    void parser::Rest1(Node* Rest1Node) {
        Token temp(TokenType::Operator, "+");
        if (Lookahead.value == "+")
        {
            add_children(match(temp), Rest1Node);

            Node* termNode = createNode("Term");
            Rest1Node->children.push_back(termNode);
            term(termNode);

            Node* Rest1Node = createNode("Rest1");
            Rest1Node->children.push_back(Rest1Node);
            Rest1(Rest1Node);
        }
        if (Lookahead.value == "-")
        {
            temp.value = "-";
            add_children(match(temp), Rest1Node);

            Node* termNode = createNode("Term");
            Rest1Node->children.push_back(termNode);
            term(termNode);

            Node* Rest1Node = createNode("Rest1");
            Rest1Node->children.push_back(Rest1Node);
            Rest1(Rest1Node);
        }
        else {
            Node* NullNode = createNode("Null");
            Rest1Node->children.push_back(NullNode);
            return;
        }

    }
    void parser::Rest2(Node* Rest2Node) {
        Token temp(TokenType::Operator, "*");
        if (Lookahead.value == "*")
        {
            add_children(match(temp), Rest2Node);

            Node* factorNode = createNode("Factor");
            Rest2Node->children.push_back(factorNode);
            factor(factorNode);

            Node* Rest2Node = createNode("Rest2");
            Rest2Node->children.push_back(Rest2Node);
            Rest2(Rest2Node);
        }
        if (Lookahead.value == "/")
        {
            temp.value = "/";
            add_children(match(temp), Rest2Node);

            Node* factorNode = createNode("Factor");
            Rest2Node->children.push_back(factorNode);
            factor(factorNode);

            Node* Rest2Node = createNode("Rest2");
            Rest2Node->children.push_back(Rest2Node);
            Rest2(Rest2Node);
        }
        else {
            Node* NullNode = createNode("Null");
            Rest2Node->children.push_back(NullNode);
            return;
        }
    }


    void parser::factor(Node* factorNode)
    {
        Token temp(TokenType::Identifier, Lookahead.value);
        if (Lookahead.type == TokenType::Identifier)
        {
            Node* IdentifierNode = createNode("Identifier");
            factorNode->children.push_back(IdentifierNode);
            add_children(match(temp), IdentifierNode);
        }
        else if (Lookahead.type == TokenType::Number)
        {
            temp.type = TokenType::Number;
            temp.value = Lookahead.value;
            Node* NumberNode = createNode("Number");
            factorNode->children.push_back(NumberNode);
            // number(NumberNode);
            add_children(match(temp), NumberNode);
        }
        else if (Lookahead.value == "(")
        {
            temp.type = TokenType::Punctuation;
            temp.value = "(";
            add_children(match(temp), factorNode);

            Node* exprNode = createNode("expression");
            factorNode->children.push_back(exprNode);
            expr(exprNode);

            temp.value = ")";
            temp.type = TokenType::Punctuation;
            add_children(match(temp), factorNode);
        }
        else
            error();
    }
    void parser::op(Node* OperatorNode)
    {
        Token temp = { TokenType::Operator ,Lookahead.value };
        add_children(match(temp), OperatorNode);
    }
    Node* parser::match(const Token& token) {
        cout << "AT INDEX:       \t" << index << endl;;
        cout << "Lookahead:      \t" << "value: " << Lookahead.value << "   " << "type: " << static_cast<int>(Lookahead.type) << endl;
        cout << "Expected element:\t" << "value: " << token.value << "   " << "type: " << static_cast<int>(token.type) << endl;
        if ((Lookahead.value == token.value) && (Lookahead.type == token.type))
        {
            Node* NewNode = createNode(Lookahead.value);
            cout << "match" << endl;
            cout << "------------------------------------------------------------------------------------ \n";
            nextToken();
            return NewNode;
        }
        else
        {
            error();
        }
        return nullptr;
    }
    void parser::error() {
        cout << "Syntax Error  at " << Lookahead.value << endl;
        cout << "------------------------------------------------------------------------------------ \n";
        Error_status = 1;
    }
    Node* parser::GetRoot() {
        return this->root;
    }
    parser::parser(const vector<Token>& mytoken) :Lookahead(TokenType::Identifier, "none"), token(mytoken) {
        parser::nextToken();
        root = createNode("Statements");
        stmts(root);
        if (Error_status != 1)
            cout << "Syntax Correct\n";
    }

void generateDotCode(const Node* node, ofstream& file) {
    if (node) {
        file << "  " << node->id << " [label=\"" << node->label;
        if (!node->token.empty()) { // Include token in label if available
            file << "\\n" << node->token;
        }
        file << "\"];" << endl;
        for (const Node* child : node->children) {
            file << "  " << node->id << " -> " << child->id << ";" << endl;
            generateDotCode(child, file);
        }
    }
}

void generateParseTreeVisualization(const Node* root) {
    ofstream file("parsetree.dot");
    if (file.is_open()) {
        file << "digraph ParseTree {" << endl;
        generateDotCode(root, file);
        file << "}" << endl;
        file.close();

        system("dot -Tpng parsetree.dot -o parsetree.png");
        cout << "Parse tree visualization generated as parsetree.png" << endl;
    }
    else {
        cout << "Failed to create parsetree.dot file" << endl;
    }
}
