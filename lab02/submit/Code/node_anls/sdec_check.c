#include"sdec_check.h"
#include"../utils/utils.h"

#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"
#include"../data_struct/type_node.h"

#include"vdec_check.h"
#include"sdec_check.h"
#include"stmt_check.h"

static int annoy_num = 0;

extern SymHashStk* sym_table;

void sdecs_anls(Node* extdef, int* pass){
    /*顶层函数-结构体定义*/ 
    /**
     * ExtDef->Specifier SEMI
     * **/
    Node* specifier = extdef->children;
    // Specifier->TYPE | StructSpecifier

    // Case01: Specifier-> TYPE
    if(STREQ(specifier->children->name, "TYPE")) return;
    // Case02: Specifier-> StructSpecifier
    Node* struct_spec = specifier->children;

    // StructSpecifier->STRUCT Tag | STRUCT OptTag LC DefList RC
    sdecs_structspecifier_anls(struct_spec, pass);
}

char* sdecs_structspecifier_anls(Node* struct_spec, int* pass){
    /*分析StructSpecifier节点，获取结构体定义*/
    char* rst = "";
    // Case02.01: StructSpecifier->STRUCT Tag
    if(STREQ(struct_spec->children->next->name, "Tag")) return rst;
    // Case02.02: StructSpecifier->STRCUT OptTag LC DefList RC
    Node* opt_tag = struct_spec->children->next;
    Node* deflist = opt_tag->next->next;

    char* struct_name = sdecs_opttag_anls(opt_tag);
    // 检测结构体名是否已被使用
    if(hash_found_global(sym_table, struct_name) != NULL){
        LOG_SEMANTIC_ANLS_ERROR(16, struct_spec->u.syn_line, "The struct name has already been used.");
        return rst;
    }
    // 新建作用域，将结构体成员追加至作用域符号表中
    Node* struct_fields = NULL;
    stack_push(sym_table);
    sdecs_deflist_anls(deflist, pass, 1);

    // 根据当前作用域符号表构建结构体，添加结构体到符号表
    if((*pass) == 0){
        stack_pop(sym_table);
        return rst;
    }
    else{
        SymNode* p = new_struct_sym_node(struct_name);
        SymNode* tail = p;

        SymNode* cursor = sym_table->stack_data[sym_table->st_ptr];
        while(cursor != NULL){
            if(cursor->type == VAR){
                tail->StructInfo.field_next = clone_var_sym_node(cursor);
                tail = tail->StructInfo.field_next;
            }
            cursor = cursor->scope_next;
        }
        tail->StructInfo.field_next = NULL;
        stack_pop(sym_table);
        hash_insert(sym_table, p);

        STRCP(rst, p->name);
        return rst;
    }
}


char* sdecs_opttag_anls(Node* opt_tag){
    /*分析Optag节点，获取结构体名称*/
    /**
     * OptTag->ID | epsilon
     * **/
    Node* id_node = opt_tag->children;
    char* rst = "";
    if(id_node != NULL){    // OptTag->ID
        STRCP(rst, id_node->u.lex_val);
    }
    else{   // OptTag->epsilon      Annoy struct: Name: Annoyxxx
        assert(annoy_num < 26*26*26);
        STRCP(rst, "Annoy   ");
        char ch1 = (char)((int)'a' + annoy_num % 26);
        char ch2 = (char)((int)'a' + (annoy_num / 26) % 26);
        char ch3 = (char)((int)'a' + (annoy_num / (26 * 26)) % 26);
        rst[7] = ch1;
        rst[6] = ch2;
        rst[5] = ch3;
        annoy_num++;
    }
    return rst;
}

void sdecs_declist_anls(char* type, Node* declist, int* pass, int ifstruct){
    /*分析Decist节点，获取结构体域成员定义*/
    /**
     * DecList->Dec | Dec COMMA DecList
     * **/
    Node* cursor = declist;
    while(1){
        Node* dec = cursor->children;
        // Dec->VarDec | VarDec ASSIGNOP Exp
        Node* vdec = dec->children;
        if(dec->children->next == NULL){    // Case01.Dec->VarDec
            SymNode* p = vdecs_vardec_anls(type, vdec, pass);
            if(p == NULL){
                // TODO: 发现变量重名的后续处理
                if(ifstruct == 1){
                    LOG_SEMANTIC_ANLS_ERROR(15, dec->u.syn_line, "member name repeat or initialized in struct.");
                }
                else{
                    LOG_SEMANTIC_ANLS_ERROR(3, dec->u.syn_line, "The var has already beed defined.");
                }
            }
            else{
                if((*pass) == 1) hash_insert(sym_table, p);
            }
        }
        else{   // TODO: Case02.Dec->VarDec ASSIGNOP Exp
            if(ifstruct == 1){  // 在结构体中使用
                LOG_SEMANTIC_ANLS_ERROR(15, dec->u.syn_line, "member name repeat or initialized in struct.");
                (*pass) = 0;
                SymNode* p = vdecs_vardec_anls(type, vdec, pass);
                if(p == NULL){
                    // TODO: 发现变量重名的后续处理
                    LOG_SEMANTIC_ANLS_ERROR(15, dec->u.syn_line, "member name repeat or initialized in struct.");
                }
                else{
                    hash_insert(sym_table, p);
                }
            }
            else{
                SymNode* p = vdecs_vardec_anls(type, vdec, pass);
                Node* exp = dec->children->next->next;
                TypeNode* right_val_type = stmt_exp_anls(exp, pass);
                if(p == NULL){
                    // TODO: 发现变量重名的后续处理、赋值操作右侧分析
                    LOG_SEMANTIC_ANLS_ERROR(3, dec->u.syn_line, "The var has already beed defined.");
                }
                else{
                    if(!equal_type_nodes(p->VarInfo.var_type, right_val_type, pass)){
                        LOG_SEMANTIC_ANLS_ERROR(5, exp->u.syn_line, "Types of two operations are not matched.");
                        return;
                    };
                    hash_insert(sym_table, p);
                }
            }
        }

        if(cursor->children->next == NULL) break;
        else cursor = cursor->children->next->next;
    }
}

void sdecs_deflist_anls(Node* deflist, int* pass, int ifstruct){
    /*分析Defist节点，获取结构体域成员定义*/
    /**
     * DefList->Def DefList | epsilon
     * **/
    Node* cursor = deflist;
    while(1){
        if(cursor->children == NULL) break;
        Node* def = cursor->children;

        // Def->Specifier DecList SEMI
        Node* specifier = def->children;
        Node* declist = def->children->next;
        int dec_pass = 1;
        char* type = vdecs_specifier_anls(specifier, &dec_pass);
        sdecs_declist_anls(type, declist, &dec_pass, ifstruct);

        cursor = cursor->children->next;
    }
}


