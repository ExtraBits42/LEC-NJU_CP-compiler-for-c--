#include"utils.h"

int max(int a, int b){
    /*最大值*/
    return (a > b ? a : b);
}

int min(int a, int b){
    /*最小值*/
    return (a > b ? b : a);
}

int int_o2d(char *v){
    /*整数：八进制转十进制*/
    int len = strlen(v);
    int ans = 0;
    for(int i = 1; i < len; i++){
        ans = (ans << 3) + (v[i] - '0');
    }
    return ans;
}

int int_h2d(char *v){
    /*整数：十六进制转十进制*/
    int len = strlen(v);
    int ans = 0;
    for(int i = 2; i < len; i++){
        int tmp = 0;
        if(v[i] >= '0' && v[i] <= '9') tmp = v[i] - '0';
        else if(v[i] >= 'a' && v[i] <= 'f') tmp = 10 + v[i] - 'a';
        else if(v[i] >= 'A' && v[i] <= 'F') tmp = 10 + v[i] - 'A';
        ans = (ans << 4) + tmp;
    }
    return ans;
}

char* pnotation_type(SymType x){
    /*Debug-打印符号节点*/
    if(x == VAR) return "Var";
    else if(x == FUNC) return "Func";
    else if(x == STRUCTT) return "Struct";
};

void pdata_type(TypeNode* p){
    /*Debug-打印类型节点*/
    if(p->type == BASIC || p->type == STRUCTTYPE) printf("%s", p->type_name);
    else{
        TypeNode* basic_type = p;
        while(basic_type->type == ARRAY){
            basic_type = basic_type->arr_ele_type;
        }
        printf("%s", basic_type->type_name);
        printf("Arr[%d]", p->arr_size);
        p = p->arr_ele_type;
        while(p->type == ARRAY){
            printf("[%d]", p->arr_size);
            p = p->arr_ele_type;
        }
    }
}


void show_me_sym_table_scope(SymHashStk* table){
    /*Debug-以作用域层次展示符号表内容*/
    printf("\n----Now, show you symbol_table by stack layers:\n");
    for(int i = 0; i < table->stack_size; i++){
        printf("Layer %2d: ", i);
        SymNode* cursor = table->stack_data[i];
        while(cursor != NULL){
            printf("(%d, %s)\t", cursor->type, cursor->name);
            cursor = cursor->scope_next;
        }
        printf("\n");
    }
    printf("----End symbol_table show\n");
}

void show_me_sym_table_hash(SymHashStk* table){
    /*Debug-以哈希表层次展示符号表内容*/
    printf("\n----Now, show you symbol_table by hash code:\n");
    for(int i = 0; i < table->hash_size; i++){
        printf("Hashcode %2d: ", i);
        SymNode* cursor = table->hash_data[i];
        while(cursor != NULL){
            printf("(%d, %s)\t", i, cursor->name);
            cursor = cursor->hash_next;
        }
        printf("\n");
    }
    printf("----End symbol_table show\n");
}

void show_me_sym_table_layer_details(SymHashStk* table){
    /*以作用域层次展示符号表内容，并且展示表项的详细信息*/
    printf("\n----Now, show you symbol_table by detail:\n");
    for(int i = 0; i < table->stack_size; i++){
        printf("Layer %2d:\n", i);
        SymNode* cursor = table->stack_data[i];
        while(cursor != NULL){
            printf("\t(Name: %s, Type: %s): ", cursor->name, pnotation_type(cursor->type));
            if(cursor->type == VAR){
                printf("ValType: ");
                pdata_type(cursor->VarInfo.var_type);
            }
            else if(cursor->type == STRUCTT){
                SymNode* member = cursor->StructInfo.field_next;
                printf("\n");
                while(member != NULL){
                    printf("\t\t");
                    printf("(Name: %s, Type: %s): ", member->name, pnotation_type(member->type));
                    printf("ValType: ");
                    pdata_type(member->VarInfo.var_type);
                    member = member->StructInfo.field_next;
                    printf("\n");
                }
            }
            else if(cursor->type == FUNC){
                printf("ReturnType: ");
                pdata_type(cursor->FuncInfo.func_return_type);
                printf(", ");
                printf("Parmas: [");
                SymNode* param = cursor->FuncInfo.params_list;
                while(param != NULL){
                    printf("(%s,", pnotation_type(param->type));
                    pdata_type(param->VarInfo.var_type);
                    printf(", %s) ", param->name);
                    param = param->FuncInfo.params_list;
                }
                printf("]");
            }
            printf("\n");
            cursor = cursor->scope_next;
        }
    }
    printf("----End symbol_table show\n");
}

