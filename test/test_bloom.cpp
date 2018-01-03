//
// Created by victor on 6/2/17.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "bloom_func.h"
#include "com_t.h"
#define CODE_LENGTH 32
#define CODE_NUM 1000

void random_string(char str[], int length){
    int i;
    for(i = 0; i < length; i++){
        str[i] = 'a' + (rand() % 26);
    }
    str[length] = '\0';
}

int main(){
    bloom *bl = bloom::Get_bloom();
    char str1[CODE_NUM][CODE_LENGTH + 1];
    char str2[CODE_NUM][CODE_LENGTH + 1];
    double stat_t = 0.0;
    double end_t = 0.0;
    double result;
    cp_t cp_time;
    std::string str;
    int i;
    int exist_time = 0;
    srand((unsigned)time(NULL));
    for( i = 0; i < CODE_NUM; i++){
        random_string(str1[i], CODE_LENGTH);
    }
    for( i = 0; i < CODE_NUM; i++){
        random_string(str2[i], CODE_LENGTH);
    }


    std::cout.setf(std::ios::fixed);
/////////////////////////////////////////////////////////////////////////////
    stat_t = cp_time.get_time();
    for( i = 0; i < CODE_NUM; i++){
        str = str1[i];
        bl -> bloom_insert(str);
    }
    end_t = cp_time.get_time();
    result = 1000 * (end_t - stat_t) / CODE_NUM;
    std::cout<<"Insert average time: "<<result<< "ms" <<std::endl;

    stat_t = cp_time.get_time();
    for( i = 0; i < CODE_NUM; i++){
        str = str1[i];
        if(bl -> bloom_exist(str)) {
            exist_time++;
        }
    }
    std::cout<<exist_time<<std::endl;
    end_t = cp_time.get_time();
    result = 1000 * (end_t - stat_t) / CODE_NUM;
    if(exist_time == CODE_NUM)
        std::cout << "Str1 success to find!" << std::endl;
    else
        std::cout << "Str2 failed to find!" << std::endl;
    std::cout<<"Find average time: "<<result<< "ms" <<std::endl;
    exist_time = 0;
////////////////////////////////////////////////////////////////////////////
    stat_t = cp_time.get_time();
    for( i = 0; i < CODE_NUM; i++){
        str = str2[i];
        if(bl -> bloom_exist(str)) {
            exist_time++;
        }
    }
    end_t = cp_time.get_time();
    result = 1000 * (end_t - stat_t) / CODE_NUM;
    if(exist_time == 0)
        std::cout << "Str2 success to find!" << std::endl;
    else
        std::cout << "Str2 failed to find!" << std::endl;
    std::cout<<"Find average time: "<<result<< "ms" <<std::endl;
    return 0;
}
