#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum NodeType {LEXICAL, SYNTAX};

typedef struct TreeNode
{
    enum NodeType type;
    int eletype;
    int line;
    char* val;
    struct TreeNode* children;
    struct TreeNode* tail;
    struct TreeNode* next;
};

// Set attributions of specific tree node
void set_node(struct TreeNode* root, int l, char* v, char* elet){
    root->line = l;
    root->val = (char*)malloc(sizeof(v));
    strcpy(root->val, v);
    root->eletype = (char*)malloc(sizeof(elet));
    strcpy(root->eletype, elet);
}
// Add children node to specific tree node
void add_child(struct TreeNode* root, struct TreeNode* p){
    if(p == NULL){
        root->children = NULL;
        root->tail = NULL;
        return;
    }
    if(root->children == NULL){
        root->children = p;
        root->tail = p;
    }
    else{
        root->tail->next = p;
        root->tail = p;
    }
}