//
// Created by victor on 6/6/17.
//

#include "dedup.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

void random_string(char str[], int length){
    int i;
    for(i = 0; i < length; i++){
        str[i] = 'a' + (rand() % 26);
    }
    str[length] = '\0';
}

int main(int argc, char **argv){
    char path[256];
    char devname[30];
    int mode = 0;
    int cache_mode = 0;
    int cache_size = 0;
    int is_cache = 0;
    int prefetch_length = 0;
    dedup ded;
    strcpy(path, argv[1]);
    strcpy(devname, argv[2]);
    mode = atoi(argv[3]);
    cache_mode = atoi(argv[4]);
    is_cache = atoi(argv[5]);
    cache_size = atoi(argv[6]);
    prefetch_length = atoi(argv[7]);


/////////////////////////////////////////////////////////////////////////////
///                              test travel                              ///
/////////////////////////////////////////////////////////////////////////////
//    ded.travel_dir(path);

/////////////////////////////////////////////////////////////////////////////
///                              test dedup                               ///
/////////////////////////////////////////////////////////////////////////////
    ded.dedup_func(path, devname, mode, cache_mode, is_cache, cache_size, prefetch_length);
    return 0;
}