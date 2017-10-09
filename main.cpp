#include <iostream>
#include <unistd.h>
#include "dedup.h"


int main(int argc, char *argv[]) {
    char path[256];
    char devname[30];
    int mode = 0;
    int parallel_mode = 0;
    int cache_size = 0;
    int is_cache = 0;
    int prefetch_length = 0;
    int ch;
    dedup ded;
    while((ch = getopt(argc, argv, "a:b:c:d:e:f:g:") ) != 1){
        switch (ch){
            case 'a':
                strcpy(path, optarg);
                std::cout << "The data set path is: " << path << std::endl;
                break;
            case 'b':
                strcpy(devname, optarg);
                std::cout << "The device path is: " << devname << std::endl;
                break;
            case 'c':
                mode = atoi(optarg);
                if(mode == 0)
                    std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: EaD" << std::endl;
                else if(mode == 1)
                    std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: MD5" << std::endl;
                else if(mode == 2)
                    std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: SHA256" << std::endl;
                else
                    std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: SHA1" << std::endl;
                break;
            case 'd':
                parallel_mode = atoi(optarg);
                if(parallel_mode == 0)
                    std::cout << "The read file strategy is sequential read." << std::endl;
                else if(parallel_mode == 1)
                    std::cout << "The read file strategy is parallel read." << std::endl;
                else{
                    std::cout << "Error parameter!" << std::endl;
                    return 0;
                }
                break;
            case 'e':
                is_cache = atoi(optarg);
                if(is_cache == 0)
                    std::cout << "This test adapt cache strategy." << std::endl;
                else if(is_cache == 1)
                    std::cout << "This test doesn't adapt cache strategy." << std::endl;
                else{
                    std::cout << "Error parameter!" << std::endl;
                    return 0;
                }
                break;
            case 'f':
                cache_size = atoi(optarg);
                std::cout << "Cache size is: " << cache_size << std::endl;
                break;
            case 'g':
                prefetch_length = atoi(optarg);
                std::cout << "The prefetch cache length is: " << prefetch_length << std::endl;
                break;
        }
    }
    ded.dedup_func(path, devname, mode, parallel_mode, is_cache, cache_size, prefetch_length);
    return 0;
}