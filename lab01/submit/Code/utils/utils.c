#include"utils.h"

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