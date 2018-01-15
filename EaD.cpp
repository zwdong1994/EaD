#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "dedup.h"

void usage(){
    std::cout << "-a: The data set path."<< std::endl;
    std::cout << "-b: The device path."<< std::endl;
    std::cout << "-c: The deduplication scheme."<< std::endl;
    std::cout << "-d: The strategy to read data sets."<< std::endl;
    std::cout << "-e: Whether use cache."<< std::endl;
    std::cout << "-f: The cache size."<< std::endl;
    std::cout << "-g: The prefetch cache length."<< std::endl;
}

int main(int argc, char *argv[]) {
    char path[256] = {};
    char devname[30] = {};
    int mode = 0;
    int parallel_mode = 0;
    int cache_size = 0;
    int is_cache = 0;
    int prefetch_length = 0;
    int sample_l = 0;
    int ch;

    while((ch = getopt(argc, argv, "a:b:c:d:e:f:g:s:") ) != -1){
        switch (ch){
            case 'a':
                strcpy(path, optarg);
                break;
            case 'b':
                strcpy(devname, optarg);

                break;
            case 'c':
                mode = atoi(optarg);

                break;
            case 'd':
                parallel_mode = atoi(optarg);

                break;
            case 'e':
                is_cache = atoi(optarg);
                break;
            case 'f':
                cache_size = atoi(optarg);

                break;
            case 'g':
                prefetch_length = atoi(optarg);

                break;
            case 's':
                sample_l = atoi(optarg);

                break;
            default:
                usage();
                exit(-1);

        }
    }
    if(strlen(path) == 0){
        std::cout << "There is no data set path input." << std::endl;
        exit(0);
    } else{
        std::cout << "The data set path is: " << path << std::endl;
    }
    if(strlen(devname) == 0){
        std::cout << "There is no device path input." << std::endl;
        exit(0);
    } else{
        std::cout << "The device path is: " << devname << std::endl;
    }

    if(mode == 0)
        std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: EaD" << std::endl;
    else if(mode == 1)
        std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: MD5" << std::endl;
    else if(mode == 2)
        std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: SHA256" << std::endl;
    else if(mode == 3)
        std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: SHA1" << std::endl;
    else if(mode == 4)
        std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: BLAKE2b" << std::endl;
    else if(mode == 5)
        std::cout << "The deduplication scheme(EaD or traditional deduplication schemes) is: Sample_md5" << std::endl;
    else{
        std::cout << "Error deduplication scheme parameter!" << std::endl;
        return 0;
    }

    if(parallel_mode == 0)
        std::cout << "The read file strategy is sequential read." << std::endl;
    else if(parallel_mode == 1)
        std::cout << "The read file strategy is parallel read." << std::endl;
    else{
        std::cout << "Error parameter!" << std::endl;
        return 0;
    }

    if(is_cache == 0)
        std::cout << "This test doesn't adapt the cache strategy." << std::endl;
    else if(is_cache == 1)
        std::cout << "This test adapt the cache strategy." << std::endl;
    else{
        std::cout << "Error parameter!" << std::endl;
        return 0;
    }

    if(cache_size > 0)
        std::cout << "Cache size is: " << cache_size << std::endl;

    if(prefetch_length > 0)
        std::cout << "The prefetch cache length is: " << prefetch_length << std::endl;

    if(sample_l < 0){
        std::cout << "Error parameter! 'sample_l' need to be larger than 0." << std::endl;
        return 0;
    }
    if(sample_l > 0)
        std::cout << "Sample length is: " << sample_l << std::endl;

    if(mode == 5 && sample_l <= 0){
        std::cout<< "Error sample length in the sample_md5 scheme." << std::endl;
        return 0;
    }
    //std::cout << "Please input the enter to start the test! " << std::endl;
    //getchar();
    dedup ded;
    ded.dedup_func(path, devname, mode, parallel_mode, is_cache, cache_size, prefetch_length, sample_l);
    return 0;
}