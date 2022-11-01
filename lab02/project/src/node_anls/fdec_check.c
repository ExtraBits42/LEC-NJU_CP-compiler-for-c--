#include"fdec_check.h"
#include"../utils/utils.h"

#include"../data_struct/sym_node.h"
#include"../data_struct/sym_table.h"
#include"../data_struct/type_node.h"

#include"vdec_check.h"
#include"sdec_check.h"
#include"fdec_check.h"
#include"stmt_check.h"

extern SymNode* cur_func;
extern SymHashStk* sym_table;
extern SymHashStk* dec_table;

void fdecs_anls(Node* extdef, int*pass, int ifdec){
    /*顶层函数:分析函数定义*/
    /**
     * Extef->Specifier FunDec CompSt
     * **/
    Node* specifier = extdef->children;
    Node* fundec = extdef->children->next;
    Node* compst = extdef->children->next->next;

    // Step01.分析函数返回值
    int ftype_pass = 1;
    char* return_type = vdecs_specifier_anls(specifier, &ftype_pass);
    stack_push(sym_table);      // 建立新作用域
    // Step02.分析函数原型:函数名、形式参数列表
    int ffundec_pass = 1;
    SymNode* func = fdecs_fundec_anls(fundec, &ffundec_pass, ifdec);
    func->FuncInfo.func_return_type = new_constant_type_node(return_type);
    // ## 函数定义-后续处理
    if(ifdec == 0){
        sym_table->st_ptr--;        // trick: 形参表分析后并不清空作用域符号表，给函数体第一层继续使用

        // Step03.分析函数体
        int fbody_pass = 1;
        cur_func = func;
        // 调用函数体分析函数
        stmt_compst_anls(compst, &fbody_pass);

        // Step04.验证函数定义是否有误
        if(ftype_pass == 1 && ffundec_pass == 1 && fbody_pass == 1){
            hash_insert(sym_table, func);
        }
    }
    // ## 函数声明-后续处理
    else{
        stack_pop(sym_table);
        SymNode* found = hash_found_global(sym_table, func->name);
        if(found == NULL || found->type != FUNC){  // 声明但未定义
            LOG_SEMANTIC_ANLS_ERROR(18, specifier->u.syn_line, "Declaration for a undefined function.");
            assert(dec_table != NULL);
        }
        int confilc_check = 1;
        if(found != NULL){
            if(!equal_type_nodes(func->FuncInfo.func_return_type, found->FuncInfo.func_return_type, pass)){
                confilc_check = 0;
            }
            SymNode* params1 = func->FuncInfo.params_list;
            SymNode* params2 = found->FuncInfo.params_list;
            while(params1 != NULL && params2 != NULL){
                if(!(STREQ(params1->name, params2->name)) || 
                !equal_type_nodes(params1->VarInfo.var_type, params2->VarInfo.var_type, pass)){
                    confilc_check = 0;
                    break;
                }
                params1 = params1->FuncInfo.params_list;
                params2 = params2->FuncInfo.params_list;
            }
            if(!(params1 == NULL && params2 == NULL)) confilc_check = 0;
            if(confilc_check == 0){
                LOG_SEMANTIC_ANLS_ERROR(19, specifier->u.syn_line, "Functions Declaration and Definition conflic.");
            }
        }
        found = hash_found_global(dec_table, func->name);
        while(found != NULL){
            if(STREQ(found->name, func->name)){
                if(!equal_type_nodes(func->FuncInfo.func_return_type, found->FuncInfo.func_return_type, pass)){
                    confilc_check = 0;
                }
                SymNode* params1 = func->FuncInfo.params_list;
                SymNode* params2 = found->FuncInfo.params_list;
                while(params1 != NULL && params2 != NULL){
                    if(!(STREQ(params1->name, params2->name)) || 
                    !equal_type_nodes(params1->VarInfo.var_type, params2->VarInfo.var_type, pass)){
                        confilc_check = 0;
                        break;
                    }
                    params1 = params1->FuncInfo.params_list;
                    params2 = params2->FuncInfo.params_list;
                }
                if(!(params1 == NULL && params2 == NULL)) confilc_check = 0;
                if(confilc_check == 0){
                    LOG_SEMANTIC_ANLS_ERROR(19, specifier->u.syn_line, "Functions Declaration and Definition conflic.");
                }
            }
            found = found->hash_next;
        }
        hash_insert(dec_table, func);
    }
}


SymNode* fdecs_fundec_anls(Node* fundec, int*pass, int ifdec){
    /*分析节点FunDec,构建函数原型节点*/
    /**
     * FunDec->ID LP VarList RP | ID LP RP
     * **/
    Node* id_node = fundec->children;
    Node* tmp = id_node->next->next;
    Node* varlist = (STREQ(tmp->name, "RP")? NULL : tmp);

    // Step01.获取函数名
    char* func_name;
    STRCP(func_name, id_node->u.lex_val);
    // 检查函数名是否已被使用
    SymNode* found = hash_found_global(sym_table, func_name);
    int repeat = 0;
    if(ifdec == 0){
        if(found != NULL){
            LOG_SEMANTIC_ANLS_ERROR(4, fundec->u.syn_line, "The function name has already beed used.");
            (*pass) = 0;
        }
    }
    
    SymNode* p = new_func_sym_node(func_name);
    // Step02.获取函数形式参数列表
    p->FuncInfo.params_list = fdecs_varlist_anls(varlist, pass);
    return p;
}

SymNode* fdecs_varlist_anls(Node* varlist, int*pass){
    /*分析节点Varist，获取函数形式参数表*/
    /**
     * VarList->ParamDec COMMA VarList | ParamDec
     * **/
    if(varlist == NULL) return NULL;
    SymNode* params_list = NULL;
    SymNode* params_tail = NULL;

    Node* cursor = varlist;
    while(cursor != NULL){
        Node* paramdec = cursor->children;
        // 构建参数链表【
        SymNode* p = fdecs_paramdec_anls(paramdec, pass);
        if(p == NULL){
            // TODO：变量重名的后续处理
            LOG_SEMANTIC_ANLS_ERROR(3, varlist->u.syn_line, "The var has already beed defined.");
        }
        else{
            if(params_list == NULL){
                params_list = p;
                params_tail = p;
            }
            else{
                params_tail->FuncInfo.params_list = p;
                params_tail = params_tail->FuncInfo.params_list;
            }
        }
        // 】
        if(cursor->children->next == NULL) cursor = NULL;
        else cursor = cursor->children->next->next;
    }
    return params_list;
}

SymNode* fdecs_paramdec_anls(Node* paramdec, int*pass){
    /*分析节点Paramec，获取单个形式参数*/
    /**
     * ParamDec->Specifier VarDec
     * **/
    Node* specifier = paramdec->children;
    Node* vardec = paramdec->children->next;

    char* type = vdecs_specifier_anls(specifier, pass);
    SymNode* p = vdecs_vardec_anls(type, vardec, pass);
    
    if(p == NULL){
        // TODO: 变量重名的后续处理
        LOG_SEMANTIC_ANLS_ERROR(3, vardec->u.syn_line, "The var has already beed defined.");
    }
    else{
        hash_insert(sym_table, p);
    }
    return clone_var_sym_node(p);
}


