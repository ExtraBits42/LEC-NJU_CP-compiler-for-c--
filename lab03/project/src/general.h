#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdarg.h>

# define YYLTYPE_IS_DECLARED 1
typedef struct YYLTYPE
{  
    int first_line;  
    int first_column;  
    int last_line;  
    int last_column;  
} YYLTYPE;

#include"data_struct/spt.h"
#include"utils/utils.h"




