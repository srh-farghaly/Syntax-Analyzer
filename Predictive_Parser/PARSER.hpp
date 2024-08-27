#ifndef PARSER_H
#define PARSER_H


// Node structure for the parse tree
struct Node {
    int id;
    std::string label;
    std::string token; // Additional token field
    std::vector<Node*> children;
};

// Function to create a new node in the parse tree
//Node* createNode(const string& label, const string& token = "");


class parser {
private:
    Node* root;
    Token Lookahead;
    int index = -1;
    int Error_status = 0;
    const std::vector<Token>& token;

    void nextToken();
    void stmts(Node* StatementsNode);
    void rest(Node* RestNode);

    void stmt(Node* statementNode);
    void assign_stmt(Node* assignNode);

    void add_children(Node* childNode, Node* ParentNode);

    void while_stmt(Node* whileStatementNode);
    void if_stmt(Node* IfStatementNode);
    void cond(Node* ConditionNode);

    void expr(Node* exprNode);
    void term(Node* termNode);
    void Rest1(Node* Rest1Node);
    void Rest2(Node* Rest2Node);
    void Rest3(Node* Rest3Node);
    void else_stmt(Node* elseStatement);
    void else_if_stmt(Node* elseIfStatement);
    void factor(Node* factorNode);
    void op(Node* OperatorNode);
    Node* match(const Token& token);
    void error();
    void definition_stmt(Node* DefinitionNode);
    void Rest4(Node* Rest4Node);


public:
    Node* GetRoot();
    parser(const std::vector<Token>& mytoken);
};


// Function to generate DOT code for the parse tree
void generateDotCode(const Node* node, std::ofstream& file);


// Function to generate a parse tree visualization using Graphviz
void generateParseTreeVisualization(const Node* root);


#endif