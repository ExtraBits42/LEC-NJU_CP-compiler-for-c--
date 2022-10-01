typedef struct TreeNode{
    enum {LEXICAL, SYNTAX}ntype;
    char* name;
    union{
        struct{int nline;} syn;
        struct{char* val;} lex;
    };
    struct TreeNode* child;
    struct TreeNode* tail;
    struct TreeNode* next;
} Node;