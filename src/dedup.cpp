//
// Created by victor on 6/1/17.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <iomanip>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <openssl/sha.h>
#include <openssl/md5.h>

#include "blake2.h"
#include "dedup.h"





pthread_mutex_t mutex_filereader = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_num_control = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_file_num = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutex_start_pthread = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutex_delete_pthread = PTHREAD_MUTEX_INITIALIZER;
int code_mode; //0(default) means ecc, 1 means md5, 2 means sha256, 3 means sha1, 4 means BLAKE2b
int divide_mode; //0(default) means new divide algorithm, 1 means old divide algorithm

dedup::dedup() {
    for(int i = 0; i < 1000; ++i){
        time_collect_num[i] = 0;
        time_collect_num_less[i] = 0;
    }
    mid_elpstime = 0.0;
    chunk_num = 0;
    read_number = 0;
    time_total = 0.0;
    time_aver = 0.0;
    time_total_read = 0.0;
    time_total_write = 0.0;
    time_total_cache = 0.0;
    time_total_xr = 0.0;
    hash_time = 0.0;
    chunk_not_dup = 0;
    block_id = 0;
    head_10000_time = 0.0;
    prefetch_offset = 0;
    prefetch_flag = 0;
    prefetch_num = 10;
    collision_write_num = 0;
    only_ecc = 0;
    total_hash_num_in_sample = 0;
    CAFTL_hash_number = 0;

    code_mode = 0;
    divide_mode = 0;
    cache_hit_num = 0;

    file_number = 0;

    head_pthread_para = NULL;

    fade_crash_number = 0;
    crash_number = 0;


    blf = bloom::Get_bloom();
    cra_t = NULL;
    bch = init_bch(CONFIG_M, CONFIG_T, 0);

    std::cout<<"************************************************************************************"
             <<"test start!"
             <<"************************************************************************************"<<std::endl;
    std::cout<<std::left<<std::setw(30)<<"Block Number"
             <<std::left<<std::setw(30)<<"10000 Average Time(ms)"
             <<std::left<<std::setw(30)<<"Dedup Ratio(%)"
             <<std::left<<std::setw(30)<<"Average Time(ms)"
             <<std::left<<std::setw(30)<<"Total Time(s)"
             <<std::left<<std::setw(30)<<"10000 Read number"<<std::endl;
}

dedup::~dedup() {
    std::string test_title;
    std::string test_title_on = "";
    char aver_time_section[30];
    if(code_mode ==0)
        test_title = "The EaD scheme with new divide performance";
    else if(code_mode == 1)
        test_title = "The md5 scheme's performance";
    else if(code_mode == 2)
        test_title = "The sha256 scheme's performance";
    else if(code_mode == 3)
        test_title = "The sha1 scheme's performance";
    else if(code_mode == 4)
        test_title = "The BLAKE2b scheme's performance";
    else if(code_mode == 5)
        test_title = "The sample+hash deduplication performance";
    else if(code_mode == 6)
        test_title = "The sample(CAFTL) deduplication performance";
    else{
        std::cout<< "Wrong test scheme." << std::endl;
        exit(0);
    }
    if(divide_mode == 1)
        test_title_on = "The scheme with old divide performance. ";
    para *p = head_pthread_para, *hp = NULL;
    while(p != NULL){
        pthread_join(p -> pid, NULL);
        hp = p;
        p = p -> next;
        delete hp;

    }

    if(time_total > 0) {
        std::cout<<"************************************************************************************"
                 <<"Average time distribution"
                 <<"************************************************************************************"<<std::endl;
        std::cout<<std::left<<std::setw(30)<<"Average time section"
                 <<std::left<<std::setw(30)<<"Average time ratio(%)" << std::endl;

        for(int i = 0; i < 1000; ++i){
            if(i < 999)
                sprintf(aver_time_section, "%f-%f", i*0.001, (i+1)*0.001);
            else
                sprintf(aver_time_section, "%f-~", i*0.001);
            std::cout<<std::left<<std::setw(30)<<aver_time_section
                     <<std::left<<std::setw(30)<< ((double)time_collect_num[i] / chunk_num) << std::endl;
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::cout<<"************************************************************************************"
                 <<"Average time distribution(Less version)"
                 <<"************************************************************************************"<<std::endl;
        std::cout<<std::left<<std::setw(30)<<"Average time section"
                 <<std::left<<std::setw(30)<<"Average time ratio(%)" << std::endl;

        for(int i = 0; i < 1000; ++i){
            if(i < 999)
                sprintf(aver_time_section, "%f-%f", i*0.001, (i+1)*0.001);
            else
                sprintf(aver_time_section, "%f-~", i*0.001);
            std::cout<<std::left<<std::setw(30)<<aver_time_section
                     <<std::left<<std::setw(30)<< 100 * ((double)time_collect_num_less[i] / chunk_num) << std::endl;
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        time_aver = time_total / chunk_num;
        std::cout <<std::endl<< "**************************"<< test_title_on << test_title << "***********************" <<std::endl;
//        mt *mp = mt::Get_mt();
//        std::cout<< "write average time is: " << mp->time_total / mp->write_time << std::endl;
        std::cout << "The total time is " << time_total <<"ms"<< std::endl;
        std::cout << "The chunk number is " << chunk_num << std::endl;
        if(cache_hit_num > 0){
            std::cout << "Cache hit number is: " << cache_hit_num << std::endl;
            std::cout << "Cache hit rate is: " << 100 * cache_hit_num / chunk_num << "%" <<std::endl;
        }
        if(chunk_not_dup > 0){
            std::cout << "The dedupe rate is " <<  (chunk_num - chunk_not_dup) * 100.0 / chunk_num <<"%"<<std::endl;
            std::cout << "The not dedupe chunk number is " << chunk_not_dup << std::endl;
            std::cout << "The collision write number is " << collision_write_num << std::endl;
            if(only_ecc > 0) {
                std::cout << "The ECC number is " << only_ecc << std::endl;
                std::cout << "The ratio that use ECC as fingerprint is: " << (chunk_num - only_ecc) * 100.0 / chunk_num
                          << "%" << std::endl;
            }
        }
        if(total_hash_num_in_sample > 0){
            std::cout << "The number that hashed by 4KB chunk is: " << total_hash_num_in_sample << std::endl;
            std::cout << "The ratio of 4KB hash number is: " << total_hash_num_in_sample * 100.0 / chunk_num <<"%"<<std::endl;
        }
        if(CAFTL_hash_number > 0)
            std::cout << "The number that hashed by 4KB chunk which was stored its fingerprint is: " << CAFTL_hash_number << std::endl;
        std::cout << "The average time is " << time_aver <<"ms"<< std::endl<<std::endl;
        if(hash_time > 0){
            std::cout << "The average hash time is " << hash_time / chunk_num <<"ms"<< std::endl;
        }
        //test_all_crash();
        std::cout << "Fade crash number is " << fade_crash_number << std::endl;
        std::cout << "Crash number is " << crash_number << std::endl;
        std::cout << "Read number is: " << read_number << std::endl;
        if(read_number != 0) {
            std::cout << "Read average time is: " << time_total_read / read_number << std::endl;
            std::cout << "Read + XOR average time is: " << time_total_xr / read_number << std::endl;
        }
        std::cout << "Write average time is: " << time_total_write / chunk_not_dup << std::endl;
    }
}
/*
 * Path: the file path that you want dedupe.
 * dev: the device that you want write.
 * mode: choose hash or ecc scheme.
 * cac_mode: the mode of chunk scheme.
 * cache_flag: use cache or not use it.
 * cache_size: decide the size of the cache.
 * sample_l: the sample length of chunks.
 */

int dedup::dedup_func(char *path, char *dev, int mode, int divide_m, int cache_flag, int cache_size, int prefetch_length, int sample_l) {
    mp = mt::Get_mt(dev);
    if(sample_l < 0){
        std::cout<< "Error sample length!" << std::endl;
        exit(0);
    }
    sample_length = sample_l;
    if(cache_size <0){
        std::cout<< "Error cache size!" << std::endl;
        exit(0);
    }
    if(cache_flag < 0 || cache_flag > 1){
        std::cout<< "Error cache flag!" << std::endl;
        exit(0);
    }
    is_cache = cache_flag;
    if(cache_size == 0)
        is_cache = 0;
    if(is_cache) {
        cac = new_cache::Get_cache(cache_size);
        prefetch_num = prefetch_length;
    }
    if(mode >= 0 && mode <=6)
        code_mode = mode;
    else{
        std::cout<< "There are nothing to do with the mode that you input!" << std::endl;
        exit(-3);
    }
    if((code_mode == 5 || code_mode == 6) && sample_length <= 0){
        std::cout<< "Error sample length in the sample scheme." << std::endl;
        exit(-3);
    }
    if(divide_m >= 0 && divide_m <=1)
        divide_mode = divide_m;
    else{
        std::cout<< "There are nothing to do with the cache mode that you input!" << std::endl;
        exit(-4);
    }
    if(divide_mode == 0)
        travel_dir(path);
    else
        travel_dir_noparallel(path);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void dedup::travel_dir_noparallel(char *path) {
    DIR *pdir;
    struct dirent *ent;
    char child_path[512];
    pdir = opendir(path);
    if(pdir == NULL){
        std::cout<<"Open dir error!"<<std::endl;
        exit(-1);
    }

//    std::cout<<"1"<<std::endl;
    while((ent = readdir(pdir)) != NULL){
        memset(child_path, 0, 512);
        if(ent->d_type & DT_DIR){ //if the ent is dir
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;
            sprintf(child_path,"%s/%s",path,ent->d_name);
            travel_dir_noparallel(child_path);
        }
        else{
            sprintf(child_path,"%s/%s",path,ent->d_name);
//            std::cout<<child_path <<std::endl;
            if(code_mode == 0)
                file_reader_noparallel(child_path);
            else if(code_mode == 1)
                md5_file_reader_noparallel(child_path);
            else if(code_mode == 2)
                sha256_file_reader_noparallel(child_path);
            else if(code_mode == 3)
                sha1_file_reader_noparallel(child_path);
            else if(code_mode == 4)
                BLAKE2b_file_reader_noparallel(child_path);
            else if(code_mode == 5)
                sample_md5(child_path);
            else if(code_mode == 6)
                sample(child_path);
            else{
                std::cout<< "Wrong test scheme." << std::endl;
                exit(0);
            }
        }
    }
}

int dedup::file_reader_noparallel(char *path) {
    uint8_t hv[CODE_LENGTH + 1];
    char bch_result[2 * CODE_LENGTH + 1];
    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;

    char blk_num_str[30];
    std::string mid_str;
    cp_t ti;
    read_10000_num = 0;
    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
    while(1){
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0)
            break;
        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){

            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000
                     <<std::left<<std::setw(30)<< read_10000_num <<std::endl;
            head_10000_time = time_total;
            read_10000_num = 0;
        }
        memset(hv, 0, CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * CODE_LENGTH + 1);
        encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference



////////////////////////////////////////////////////////////////////////////////


//        stat_t = ti.get_time();
//        MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
//        end_t = ti.get_time();
//        ti.cp_all((end_t - stat_t) * 1000, 0);
        /////////////////////////////////////////////////////////////
        ByteToHexStr(hv, bch_result, CODE_LENGTH);
        bch_result[2 * CODE_LENGTH] = '\0';
        mid_str = bch_result;
        bloom_flag = dedup_bloom(bch_result, 2 * CODE_LENGTH);
        /////////////////////////////////////////////////////////////
        if(is_cache)
            cache_flag = dedup_cache(mid_str, (char *)chk_cont, bloom_flag);
        if(cache_flag == 1)
            cache_hit_num++;
        stat_t = ti.get_time();
        mt_flag = dedup_mt(bch_result, (char *)chk_cont, 2 * CODE_LENGTH, cache_flag, bloom_flag);

        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        if(mt_flag == 3){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            ++mp->alloc_addr_point;
            time_total_write += write_elps;
            ++collision_write_num;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        end_t = ti.get_time();
        if(is_cache && prefetch_flag)
            prefetch();
        time_total += 0.0005;//Add cache time
        //ti.cp_all((end_t - stat_t) * 1000, 1);
        time_total += (end_t - stat_t) * 1000;
        mid_elpstime = (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);
//        dedup_process(bch_result, (char *)chk_cont, 2 * CODE_LENGTH);


    }
    //ti.cp_aver(path);
    //time_total += ti.time_total;
    fclose(fp);
    return 0;
}

int dedup::md5_file_reader_noparallel(char *path) {
    uint8_t hv[MD5_CODE_LENGTH + 1];
    char bch_result[2 * MD5_CODE_LENGTH + 1];
    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    char blk_num_str[30];
    std::string mid_str;
    cp_t ti;

    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
    while(1){
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0)
            break;
        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
            head_10000_time = time_total;
        }



////////////////////////////////////////////////////////////////////////////////
        memset(hv, 0, MD5_CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * MD5_CODE_LENGTH + 1);
        stat_t = ti.get_time();
        MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        end_t = ti.get_time();
        //ti.cp_all((end_t - stat_t) * 1000, 0);
        time_total += (end_t - stat_t) * 1000;
        hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        /////////////////////////////////////////////////////////////
        ByteToHexStr(hv, bch_result, MD5_CODE_LENGTH);
        bloom_flag = dedup_bloom(bch_result, 2 * MD5_CODE_LENGTH);
        /////////////////////////////////////////////////////////////
        stat_t = ti.get_time();
//        cache_flag = dedup_cache(bch_result, (char *)chk_cont, 2 * CODE_LENGTH, bloom_flag);
        mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * MD5_CODE_LENGTH, cache_flag, bloom_flag);

        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        end_t = ti.get_time();

        //ti.cp_all((end_t - stat_t) * 1000, 1);
//        dedup_process(bch_result, (char *)chk_cont, 2 * CODE_LENGTH);
        time_total += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);

    }
    //ti.cp_aver(path);

    fclose(fp);
    return 0;
}

int dedup::sha256_file_reader_noparallel(char *path) {
    uint8_t hv[SHA256_CODE_LENGTH + 1];
    char bch_result[2 * SHA256_CODE_LENGTH + 1];
    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    char blk_num_str[30];
    std::string mid_str;
    cp_t ti;

    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
    while(1){
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0)
            break;
        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
            head_10000_time = time_total;
        }


        memset(hv, 0, SHA256_CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * SHA256_CODE_LENGTH + 1);
//        encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference


////////////////////////////////////////////////////////////////////////////////


        stat_t = ti.get_time();
        //MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        SHA256((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        end_t = ti.get_time();
        //ti.cp_all((end_t - stat_t) * 1000, 0);
        time_total += (end_t - stat_t) * 1000;
        hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        /////////////////////////////////////////////////////////////
        ByteToHexStr(hv, bch_result, SHA256_CODE_LENGTH);
        bloom_flag = dedup_bloom(bch_result, 2 * SHA256_CODE_LENGTH);
        /////////////////////////////////////////////////////////////
        stat_t = ti.get_time();
//        cache_flag = dedup_cache(bch_result, (char *)chk_cont, 2 * CODE_LENGTH, bloom_flag);
        mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * SHA256_CODE_LENGTH, cache_flag, bloom_flag);

        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        end_t = ti.get_time();

        //ti.cp_all((end_t - stat_t) * 1000, 1);
//        dedup_process(bch_result, (char *)chk_cont, 2 * CODE_LENGTH);
        time_total += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);

    }
    //ti.cp_aver(path);

    fclose(fp);
    return 0;
}

int dedup::sha1_file_reader_noparallel(char *path) {
    uint8_t hv[SHA1_CODE_LENGTH + 1];
    char bch_result[2 * SHA1_CODE_LENGTH + 1];
    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    char blk_num_str[30];
    std::string mid_str;
    cp_t ti;
    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
    while(1){
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0)
            break;
        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
            head_10000_time = time_total;
        }


        memset(hv, 0, SHA1_CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * SHA1_CODE_LENGTH + 1);
//        encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference


////////////////////////////////////////////////////////////////////////////////


        stat_t = ti.get_time();
        //MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        SHA1((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        end_t = ti.get_time();
        //ti.cp_all((end_t - stat_t) * 1000, 0);
        time_total += (end_t - stat_t) * 1000;
        hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        /////////////////////////////////////////////////////////////
        ByteToHexStr(hv, bch_result, SHA1_CODE_LENGTH);
        bloom_flag = dedup_bloom(bch_result, 2 * SHA1_CODE_LENGTH);
        /////////////////////////////////////////////////////////////
        stat_t = ti.get_time();
//        cache_flag = dedup_cache(bch_result, (char *)chk_cont, 2 * CODE_LENGTH, bloom_flag);
        mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * SHA1_CODE_LENGTH, cache_flag, bloom_flag);

        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        end_t = ti.get_time();

        //ti.cp_all((end_t - stat_t) * 1000, 1);
//        dedup_process(bch_result, (char *)chk_cont, 2 * CODE_LENGTH);
        time_total += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);

    }
    //ti.cp_aver(path);

    fclose(fp);
    return 0;
}

int dedup::BLAKE2b_file_reader_noparallel(char *path) {
	uint8_t hv[BLAKE2B_CODE_LENGTH + 1];
	char bch_result[2 * BLAKE2B_CODE_LENGTH + 1];
	FILE *fp = NULL;
	uint8_t chk_cont[4097];
	int bloom_flag = 0;
	int cache_flag = 0;
	int mt_flag = 0;
	double stat_t = 0.0;
	double end_t = 0.0;
	double write_elps = 0.0;
	char blk_num_str[30];
	uint8_t key[BLAKE2B_KEYBYTES];
	std::string mid_str;
	cp_t ti;

	for( int i = 0; i < BLAKE2B_KEYBYTES; ++i )
		key[i] = ( uint8_t )i;

	if((fp = fopen(path, "r")) == NULL){
		std::cout<<"Open file error!The file name is: "<<path<<std::endl;
		return 0;
	}
	while(1){
		mid_elpstime = 0.0;
		memset(chk_cont, 0, READ_LENGTH);
		if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0)
			break;
		chunk_num++;
		block_id ++;
		if(block_id % 10000 == 0){
			sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
			std::cout.setf(std::ios::fixed);
			std::cout<<std::left<<std::setw(30)<< blk_num_str
							 <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
							 <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
							 <<std::left<<std::setw(30)<< time_total / chunk_num
							 <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
			head_10000_time = time_total;
		}


		memset(hv, 0, BLAKE2B_CODE_LENGTH + 1);
		memset(bch_result, 0, 2 * BLAKE2B_CODE_LENGTH + 1);
//        encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference


////////////////////////////////////////////////////////////////////////////////


		stat_t = ti.get_time();
		//MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
		//SHA1((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
		blake2b( hv, BLAKE2B_CODE_LENGTH, chk_cont, READ_LENGTH, NULL, 0 );
		end_t = ti.get_time();
		//ti.cp_all((end_t - stat_t) * 1000, 0);
		time_total += (end_t - stat_t) * 1000;
		hash_time += (end_t - stat_t) * 1000;
		mid_elpstime += (end_t - stat_t) * 1000;
		/////////////////////////////////////////////////////////////
		ByteToHexStr(hv, bch_result, BLAKE2B_CODE_LENGTH);
		bloom_flag = dedup_bloom(bch_result, 2 * BLAKE2B_CODE_LENGTH);
		/////////////////////////////////////////////////////////////
		stat_t = ti.get_time();

		mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * BLAKE2B_CODE_LENGTH, cache_flag, bloom_flag);

		if(mt_flag == 2){
			chunk_not_dup++;
			write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
			time_total_write += write_elps;
			//ti.cp_all(0.2, 0);
			//time_total += 0.2;
		}
		end_t = ti.get_time();

		time_total += (end_t - stat_t) * 1000;
		mid_elpstime += (end_t - stat_t) * 1000;
		avertime_distribute(mid_elpstime);
		avertime_distribute_less(mid_elpstime);

	}

	fclose(fp);
	return 0;
}

int dedup::sample_md5(char *path){
    uint8_t hv[MD5_CODE_LENGTH + 1];
    char bch_result[2 * MD5_CODE_LENGTH + 1];
    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    int sample_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    char blk_num_str[30];
    std::set<std::string>::iterator iter;

    unsigned char sample_buf[SAMPLE_LENGTH + 1];
    uint8_t key[BLAKE2B_KEYBYTES];
    std::string mid_str;
    cp_t ti;

    for( int i = 0; i < BLAKE2B_KEYBYTES; ++i )
        key[i] = ( uint8_t )i;

    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
    while(1){
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        memset(sample_buf, 0, SAMPLE_LENGTH);
        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0)
            break;
        memcpy(sample_buf, chk_cont, SAMPLE_LENGTH);

        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
            head_10000_time = time_total;
        }


        memset(hv, 0, MD5_CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * MD5_CODE_LENGTH + 1);
//        encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference


////////////////////////////////////////////////////////////////////////////////
/*
 * Hash sample buf.
*/
        stat_t = ti.get_time();
        MD5((unsigned char *)sample_buf, (size_t)SAMPLE_LENGTH, (unsigned char *)hv); // hash sample
        //SHA1((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        end_t = ti.get_time();
        time_total += (end_t - stat_t) * 1000;
        hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        ByteToHexStr(hv, bch_result, MD5_CODE_LENGTH);
        bch_result[2 * MD5_CODE_LENGTH] = '\0';
        mid_str = bch_result;

        stat_t = ti.get_time();
        if((iter = sample_hash_vector.find(mid_str)) != sample_hash_vector.end()){ //sample hash exist
            memset(hv, 0, MD5_CODE_LENGTH + 1);
            memset(bch_result, 0, 2 * MD5_CODE_LENGTH + 1);
            MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
            ++ total_hash_num_in_sample;
            sample_flag = 1;
        } else{ // sample hash not exist
            sample_hash_vector.insert(mid_str);
            sample_flag = 0;
        }
        end_t = ti.get_time();
        time_total += (end_t - stat_t) * 1000;
        if(sample_flag == 1)
            hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;


//////////////////////////////////////////////////////////////////////////////////
        //ti.cp_all((end_t - stat_t) * 1000, 0);

        if(sample_flag == 1){
/////////////////////////////////////////////////////////////
            ByteToHexStr(hv, bch_result, MD5_CODE_LENGTH);
            bloom_flag = dedup_bloom(bch_result, 2 * MD5_CODE_LENGTH);
/////////////////////////////////////////////////////////////
            stat_t = ti.get_time();
            mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * MD5_CODE_LENGTH, cache_flag, bloom_flag);
            end_t = ti.get_time();

            time_total += (end_t - stat_t) * 1000;
            mid_elpstime += (end_t - stat_t) * 1000;
        } else{
            memset(hv, 0, MD5_CODE_LENGTH + 1);
            memset(bch_result, 0, 2 * MD5_CODE_LENGTH + 1);
            MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
            ByteToHexStr(hv, bch_result, MD5_CODE_LENGTH);
            dedup_bloom(bch_result, 2 * MD5_CODE_LENGTH);
            dedup_noread_mt(bch_result, (char *)chk_cont, 2 * MD5_CODE_LENGTH, 0, 0);
            mt_flag = 2;
        }
        stat_t = ti.get_time();
        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        end_t = ti.get_time();

        time_total += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);

    }

    fclose(fp);
    return 0;
}


int dedup::sample(char *path){
    uint8_t hv[MD5_CODE_LENGTH + 1];
    char bch_result[2 * MD5_CODE_LENGTH + 1];
    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    int sample_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    char blk_num_str[30];
    std::set<std::string>::iterator iter;

    unsigned char sample_buf[SAMPLE_LENGTH + 1];
    uint8_t key[BLAKE2B_KEYBYTES];
    std::string mid_str;
    cp_t ti;

    for( int i = 0; i < BLAKE2B_KEYBYTES; ++i )
        key[i] = ( uint8_t )i;

    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
    while(1){
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        memset(sample_buf, 0, SAMPLE_LENGTH);
        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0)
            break;
        memcpy(sample_buf, chk_cont, SAMPLE_LENGTH);

        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
            head_10000_time = time_total;
        }



        memset(bch_result, 0, 2 * MD5_CODE_LENGTH + 1);
//        encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference


////////////////////////////////////////////////////////////////////////////////
/*
 * Hash sample buf.
*/
/*        stat_t = ti.get_time();
        MD5((unsigned char *)sample_buf, (size_t)SAMPLE_LENGTH, (unsigned char *)hv); // hash sample
        //SHA1((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        end_t = ti.get_time();
        time_total += (end_t - stat_t) * 1000;
        hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;*/
        ByteToHexStr(sample_buf, bch_result, SAMPLE_LENGTH);
        bch_result[2 * SAMPLE_LENGTH] = '\0';
        mid_str = bch_result;

        stat_t = ti.get_time();
        if((iter = sample_hash_vector.find(mid_str)) != sample_hash_vector.end()){ //sample hash exist
            memset(hv, 0, MD5_CODE_LENGTH + 1);
            memset(bch_result, 0, 2 * MD5_CODE_LENGTH + 1);
            MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
            ++ total_hash_num_in_sample;
            sample_flag = 1;
        } else{ // sample hash not exist
            sample_hash_vector.insert(mid_str);
            sample_flag = 0;
        }
        end_t = ti.get_time();
        time_total += (end_t - stat_t) * 1000;
        if(sample_flag == 1)
            hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;


//////////////////////////////////////////////////////////////////////////////////
        //ti.cp_all((end_t - stat_t) * 1000, 0);

        if(sample_flag == 1){
/////////////////////////////////////////////////////////////
            ByteToHexStr(hv, bch_result, MD5_CODE_LENGTH);
            bloom_flag = dedup_bloom(bch_result, 2 * MD5_CODE_LENGTH);
/////////////////////////////////////////////////////////////
            stat_t = ti.get_time();
            mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * MD5_CODE_LENGTH, cache_flag, bloom_flag);
            end_t = ti.get_time();

            if(mt_flag == 2)
                CAFTL_hash_number ++;

            time_total += (end_t - stat_t) * 1000;
            mid_elpstime += (end_t - stat_t) * 1000;
        } else{
/*            memset(hv, 0, MD5_CODE_LENGTH + 1);
            memset(bch_result, 0, 2 * MD5_CODE_LENGTH + 1);
            MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
            ByteToHexStr(hv, bch_result, MD5_CODE_LENGTH);
            dedup_bloom(bch_result, 2 * MD5_CODE_LENGTH);
            dedup_noread_mt(bch_result, (char *)chk_cont, 2 * MD5_CODE_LENGTH, 0, 0);
*/            mt_flag = 2;
        }
        stat_t = ti.get_time();
        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        end_t = ti.get_time();

        time_total += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);

    }

    fclose(fp);
    return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////////
void dedup::travel_dir(char *path) {
    DIR *pdir;
    struct dirent *ent;

    pdir = opendir(path);

    if(pdir == NULL){
        std::cout<<"Open dir error!"<<std::endl;
        exit(-1);
    }

    while((ent = readdir(pdir)) != NULL){
        char child_path[512];
        memset(child_path, 0, 512);
        if(ent->d_type & DT_DIR){ //if the ent is dir
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;
            sprintf(child_path,"%s/%s",path,ent->d_name);
            travel_dir(child_path);
        }
        else{
            pthread_t pid;
            int err_p = 0;
            para *file_para;
            file_para = new para;

            sprintf(child_path,"%s/%s",path,ent->d_name);

            memcpy(file_para->path, child_path, sizeof(child_path));

            file_para->this_ = this;
            if(file_number >= PTHREAD_NUM - 1){
                pthread_mutex_lock(&mutex_num_control);

            }

            err_p = pthread_create(&pid, NULL, &start_pthread, (void *)file_para);
            file_para->pid = pid;
            if(err_p){
                std::cout<< "Create pthread error!" <<std::endl;
                exit(-2);
            }

            file_para -> next = head_pthread_para;
            head_pthread_para = file_para;

            pthread_mutex_lock(&mutex_file_num);
            file_number ++;
            pthread_mutex_unlock(&mutex_file_num);

        }
    }
}

void *dedup::start_pthread(void *arg) {

    para *mid = (para *)arg;
    if(code_mode == 0)
        mid -> this_ ->file_reader(mid -> path);
    else if(code_mode == 1)
        mid -> this_ ->md5_file_reader(mid -> path);
    else if(code_mode == 2)
        mid -> this_ ->sha256_file_reader(mid -> path);
    else if(code_mode == 3)
        mid -> this_ -> sha1_file_reader(mid -> path);
    else
        mid -> this_ -> BLAKE2b_file_reader_noparallel(mid -> path);
    return nullptr;
}

int dedup::file_reader(char *path) {
    uint8_t hv[CODE_LENGTH + 1];
    char bch_result[2 * CODE_LENGTH + 1];

    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    double end_cache_t = 0.0;
    char blk_num_str[30];
    std::string mid_str;
    read_10000_num = 0;
//    std::cout<< path << std::endl;
    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
//    pthread_mutex_unlock(&mutex_start_pthread);
//    std::cout<< path << std::endl;
    while(1){
        pthread_mutex_lock(&mutex_filereader);
        memset(chk_cont, 0, READ_LENGTH);
        //pthread_mutex_lock

        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0){
            pthread_mutex_unlock(&mutex_filereader);
//            std::cout<< path << std::endl;
            break;
        }

        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000
                     <<std::left<<std::setw(30)<< read_10000_num <<std::endl;
            head_10000_time = time_total;
            read_10000_num = 0;
        }
        memset(hv, 0, CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * CODE_LENGTH + 1);
        encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference



////////////////////////////////////////////////////////////////////////////////


//        stat_t = ti.get_time();
//        MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
//        end_t = ti.get_time();
//        ti.cp_all((end_t - stat_t) * 1000, 0);
        /////////////////////////////////////////////////////////////
        ByteToHexStr(hv, bch_result, CODE_LENGTH);
        mid_str = bch_result;
        bloom_flag = dedup_bloom(bch_result, 2 * CODE_LENGTH);
        /////////////////////////////////////////////////////////////
        if(is_cache)
            cache_flag = dedup_cache(mid_str, (char *)chk_cont, bloom_flag);
        if(cache_flag == 1)
            cache_hit_num++;
        //end_cache_t = ti.get_time();
        //time_total_cache += (end_cache_t - stat_t) * 1000;
        stat_t = ti.get_time();
        mt_flag = dedup_mt(bch_result, (char *)chk_cont, 2 * CODE_LENGTH, cache_flag, bloom_flag);
        //end_t = ti.get_time();
        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        if(mt_flag == 3){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point -1, (char *)chk_cont, write_elps);
            ++mp->alloc_addr_point;
            time_total_write += write_elps;
            ++collision_write_num;
            //ti.cp_all(0.2, 0);
            //time_total += 0.2;
        }
        end_t = ti.get_time();
        if(is_cache && prefetch_flag)
            prefetch();
        time_total += 0.0005;//Add cache time
        //ti.cp_all((end_t - stat_t) * 1000, 1);
        time_total += (end_t - stat_t) * 1000;
        mid_elpstime = (end_t - stat_t) * 1000 + 0.0005;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);
//        dedup_process(bch_result, (char *)chk_cont, 2 * CODE_LENGTH);

        //pthread_mutex_unlock
        pthread_mutex_unlock(&mutex_filereader);

    }
    //ti.cp_aver(path);
    //time_total += ti.time_total;
    fclose(fp);
//   std::cout<< path << std::endl;
//    std::cout<< file_number << std::endl;
    pthread_mutex_lock(&mutex_file_num);
    file_number --;
    pthread_mutex_unlock(&mutex_num_control);
//    std::cout<< "Between mutex file number " <<file_number << std::endl;

    pthread_mutex_unlock(&mutex_file_num);

/*    pthread_mutex_lock(&mutex_delete_pthread);
    para *p = head_pthread_para -> next, *hp = head_pthread_para;
    if(strcmp(head_pthread_para -> path, path) == 0){
        head_pthread_para = p -> next;
        delete p;
        pthread_mutex_unlock(&mutex_delete_pthread);
        return 0;
    }
    while(p != NULL){
        if(strcmp(p -> path, path) == 0){
            hp -> next = p -> next;
            delete p;
            pthread_mutex_unlock(&mutex_delete_pthread);
            return 0;
        }
        hp = p;
        p = p -> next;
    }
    pthread_mutex_unlock(&mutex_delete_pthread);*/

    return 0;
}



int dedup::md5_file_reader(char *path){
    uint8_t hv[CODE_LENGTH + 1];
    char bch_result[2 * CODE_LENGTH + 1];

    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    double end_cache_t = 0.0;
    char blk_num_str[30];
    std::string mid_str;

//    std::cout<< path << std::endl;
    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
//    pthread_mutex_unlock(&mutex_start_pthread);
//    std::cout<< path << std::endl;
    while(1){
        pthread_mutex_lock(&mutex_filereader);
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        //pthread_mutex_lock

        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0){
            pthread_mutex_unlock(&mutex_filereader);
//            std::cout<< path << std::endl;
            break;
        }

        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
            head_10000_time = time_total;
        }
        memset(hv, 0, CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * CODE_LENGTH + 1);
//        encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference



////////////////////////////////////////////////////////////////////////////////


        stat_t = ti.get_time();
        MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        end_t = ti.get_time();
        time_total += (end_t - stat_t) * 1000;
        hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        /////////////////////////////////////////////////////////////
        ByteToHexStr(hv, bch_result, CODE_LENGTH);
        mid_str = bch_result;
        bloom_flag = dedup_bloom(bch_result, 2 * CODE_LENGTH);
        /////////////////////////////////////////////////////////////
        stat_t = ti.get_time();
        //cache_flag = dedup_cache(bch_result, (char *)chk_cont, bloom_flag);
        //end_cache_t = ti.get_time();
        //time_total_cache += (end_cache_t - stat_t) * 1000;
        mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * CODE_LENGTH, cache_flag, bloom_flag);
        //end_t = ti.get_time();
        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //    time_total += 0.2;
        }
        end_t = ti.get_time();

        //ti.cp_all((end_t - stat_t) * 1000, 1);
        time_total += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);
//        dedup_process(bch_result, (char *)chk_cont, 2 * CODE_LENGTH);

        //pthread_mutex_unlock
        pthread_mutex_unlock(&mutex_filereader);

    }
    //ti.cp_aver(path);
    //time_total += ti.time_total;
    fclose(fp);
//   std::cout<< path << std::endl;
//    std::cout<< file_number << std::endl;
    pthread_mutex_lock(&mutex_file_num);
    file_number --;
    pthread_mutex_unlock(&mutex_num_control);
//    std::cout<< "Between mutex file number " <<file_number << std::endl;

    pthread_mutex_unlock(&mutex_file_num);

/*    pthread_mutex_lock(&mutex_delete_pthread);
    para *p = head_pthread_para -> next, *hp = head_pthread_para;
    if(strcmp(head_pthread_para -> path, path) == 0){
        head_pthread_para = p -> next;
        delete p;
        pthread_mutex_unlock(&mutex_delete_pthread);
        return 0;
    }
    while(p != NULL){
        if(strcmp(p -> path, path) == 0){
            hp -> next = p -> next;
            delete p;
            pthread_mutex_unlock(&mutex_delete_pthread);
            return 0;
        }
        hp = p;
        p = p -> next;
    }
    pthread_mutex_unlock(&mutex_delete_pthread);*/

    return 0;
}



int dedup::sha256_file_reader(char *path){
    uint8_t hv[SHA256_CODE_LENGTH + 1];
    char bch_result[2 * SHA256_CODE_LENGTH + 1];

    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    double end_cache_t = 0.0;
    char blk_num_str[30];
    std::string mid_str;

//    std::cout<< path << std::endl;
    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
//    pthread_mutex_unlock(&mutex_start_pthread);
//    std::cout<< path << std::endl;
    while(1){
        pthread_mutex_lock(&mutex_filereader);
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        //pthread_mutex_lock

        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0){
            pthread_mutex_unlock(&mutex_filereader);
//            std::cout<< path << std::endl;
            break;
        }

        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
            head_10000_time = time_total;
        }
        memset(hv, 0, SHA256_CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * SHA256_CODE_LENGTH + 1);
        //encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference



////////////////////////////////////////////////////////////////////////////////


        stat_t = ti.get_time();
        //MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        SHA256((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        end_t = ti.get_time();
        //ti.cp_all((end_t - stat_t) * 1000, 0);
        time_total += (end_t - stat_t) * 1000;
        hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        /////////////////////////////////////////////////////////////
        ByteToHexStr(hv, bch_result, SHA256_CODE_LENGTH);
        mid_str = bch_result;
        bloom_flag = dedup_bloom(bch_result, 2 * SHA256_CODE_LENGTH);
        /////////////////////////////////////////////////////////////
        stat_t = ti.get_time();
        //cache_flag = dedup_cache(bch_result, (char *)chk_cont, bloom_flag);
        //end_cache_t = ti.get_time();
        //time_total_cache += (end_cache_t - stat_t) * 1000;
        mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * SHA256_CODE_LENGTH, cache_flag, bloom_flag);
        //end_t = ti.get_time();
        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //    time_total += 0.2;
        }
        end_t = ti.get_time();

        //ti.cp_all((end_t - stat_t) * 1000, 1);
        time_total += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);
//        dedup_process(bch_result, (char *)chk_cont, 2 * CODE_LENGTH);

        //pthread_mutex_unlock
        pthread_mutex_unlock(&mutex_filereader);

    }
    //ti.cp_aver(path);
    //time_total += ti.time_total;
    fclose(fp);
//   std::cout<< path << std::endl;
//    std::cout<< file_number << std::endl;
    pthread_mutex_lock(&mutex_file_num);
    file_number --;
    pthread_mutex_unlock(&mutex_num_control);
//    std::cout<< "Between mutex file number " <<file_number << std::endl;

    pthread_mutex_unlock(&mutex_file_num);

/*    pthread_mutex_lock(&mutex_delete_pthread);
    para *p = head_pthread_para -> next, *hp = head_pthread_para;
    if(strcmp(head_pthread_para -> path, path) == 0){
        head_pthread_para = p -> next;
        delete p;
        pthread_mutex_unlock(&mutex_delete_pthread);
        return 0;
    }
    while(p != NULL){
        if(strcmp(p -> path, path) == 0){
            hp -> next = p -> next;
            delete p;
            pthread_mutex_unlock(&mutex_delete_pthread);
            return 0;
        }
        hp = p;
        p = p -> next;
    }
    pthread_mutex_unlock(&mutex_delete_pthread);*/

    return 0;
}


int dedup::sha1_file_reader(char *path){
    uint8_t hv[SHA1_CODE_LENGTH + 1];
    char bch_result[2 * SHA1_CODE_LENGTH + 1];

    FILE *fp = NULL;
    uint8_t chk_cont[4097];
    int bloom_flag = 0;
    int cache_flag = 0;
    int mt_flag = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double write_elps = 0.0;
    double end_cache_t = 0.0;
    char blk_num_str[30];
    std::string mid_str;

//    std::cout<< path << std::endl;
    if((fp = fopen(path, "r")) == NULL){
        std::cout<<"Open file error!The file name is: "<<path<<std::endl;
        return 0;
    }
//    pthread_mutex_unlock(&mutex_start_pthread);
//    std::cout<< path << std::endl;
    while(1){
        pthread_mutex_lock(&mutex_filereader);
        mid_elpstime = 0.0;
        memset(chk_cont, 0, READ_LENGTH);
        //pthread_mutex_lock

        if(fread(chk_cont, sizeof(char), READ_LENGTH, fp) == 0){
            pthread_mutex_unlock(&mutex_filereader);
//            std::cout<< path << std::endl;
            break;
        }

        chunk_num++;
        block_id ++;
        if(block_id % 10000 == 0){
            sprintf(blk_num_str, "%ld-%ld", block_id - 10000, block_id - 1);
            std::cout.setf(std::ios::fixed);
            std::cout<<std::left<<std::setw(30)<< blk_num_str
                     <<std::left<<std::setw(30)<< (time_total - head_10000_time) / 10000
                     <<std::left<<std::setw(30)<< (chunk_num - chunk_not_dup) * 100.0 / chunk_num
                     <<std::left<<std::setw(30)<< time_total / chunk_num
                     <<std::left<<std::setw(30)<< time_total / 1000 <<std::endl;
            head_10000_time = time_total;
        }
        memset(hv, 0, SHA1_CODE_LENGTH + 1);
        memset(bch_result, 0, 2 * SHA1_CODE_LENGTH + 1);
        //encode_bch(bch, chk_cont, READ_LENGTH, hv); //get bch code from a block reference



////////////////////////////////////////////////////////////////////////////////


        stat_t = ti.get_time();
        //MD5((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        SHA1((unsigned char *)chk_cont, (size_t)4096, (unsigned char *)hv);
        end_t = ti.get_time();
        //ti.cp_all((end_t - stat_t) * 1000, 0);
        time_total += (end_t - stat_t) * 1000;
        hash_time += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        /////////////////////////////////////////////////////////////
        ByteToHexStr(hv, bch_result, SHA1_CODE_LENGTH);
        mid_str = bch_result;
        bloom_flag = dedup_bloom(bch_result, 2 * SHA1_CODE_LENGTH);
        /////////////////////////////////////////////////////////////
        stat_t = ti.get_time();
        //cache_flag = dedup_cache(bch_result, (char *)chk_cont, bloom_flag);
        //end_cache_t = ti.get_time();
        //time_total_cache += (end_cache_t - stat_t) * 1000;
        mt_flag = dedup_noread_mt(bch_result, (char *)chk_cont, 2 * SHA1_CODE_LENGTH, cache_flag, bloom_flag);
        //end_t = ti.get_time();
        if(mt_flag == 2){
            chunk_not_dup++;
            write_block(mp -> alloc_addr_point - 1, (char *)chk_cont, write_elps);
            time_total_write += write_elps;
            //ti.cp_all(0.2, 0);
            //    time_total += 0.2;
        }
        end_t = ti.get_time();

        //ti.cp_all((end_t - stat_t) * 1000, 1);
        time_total += (end_t - stat_t) * 1000;
        mid_elpstime += (end_t - stat_t) * 1000;
        avertime_distribute(mid_elpstime);
        avertime_distribute_less(mid_elpstime);
//        dedup_process(bch_result, (char *)chk_cont, 2 * CODE_LENGTH);

        //pthread_mutex_unlock
        pthread_mutex_unlock(&mutex_filereader);

    }
    //ti.cp_aver(path);
    //time_total += ti.time_total;
    fclose(fp);
//   std::cout<< path << std::endl;
//    std::cout<< file_number << std::endl;
    pthread_mutex_lock(&mutex_file_num);
    file_number --;
    pthread_mutex_unlock(&mutex_num_control);
//    std::cout<< "Between mutex file number " <<file_number << std::endl;

    pthread_mutex_unlock(&mutex_file_num);

/*    pthread_mutex_lock(&mutex_delete_pthread);
    para *p = head_pthread_para -> next, *hp = head_pthread_para;
    if(strcmp(head_pthread_para -> path, path) == 0){
        head_pthread_para = p -> next;
        delete p;
        pthread_mutex_unlock(&mutex_delete_pthread);
        return 0;
    }
    while(p != NULL){
        if(strcmp(p -> path, path) == 0){
            hp -> next = p -> next;
            delete p;
            pthread_mutex_unlock(&mutex_delete_pthread);
            return 0;
        }
        hp = p;
        p = p -> next;
    }
    pthread_mutex_unlock(&mutex_delete_pthread);*/

    return 0;
}


void dedup::ByteToHexStr(const unsigned char *source, char *dest, int sourceLen) {
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;

        highByte += 0x30;

        if (highByte > 0x39)
            dest[i * 2] = highByte + 0x07;
        else
            dest[i * 2] = highByte;

        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;
}


int dedup::dedup_bloom(char *bch_result, int bch_length) {

    std::string str;
    bch_result[bch_length] = '\0';
    str = bch_result;
    if(blf -> bloom_exist(str)) { //the str is exist in the bloom filter
        return 1;
    }
    else
        return 0;

}

int dedup::dedup_cache(std::string bch_result, char *chk_cont, int bloom_flag) {


    if(bloom_flag == 1) {
        int res = cac -> cache_find(bch_result, chk_cont);
        if (res == 1) {
            return 1;
        } /*else if (res == 2) {
            return 2;  //ecc crash
        }*/ else {
            return 3; //cache missed
        }
    }
    else{//it is also means that the block is not in the mapping table.
        cac -> cache_insert(bch_result, chk_cont); //insert the block into the cache
        return 4; //bloom missed
    }
}

int dedup::dedup_mt(char *bch_result, char *chk_cont, int bch_lengh, int cache_flag, int bloom_flag){
    int mid_crash_num = 0;
    double stat_t = 0.0;
    double end_t = 0.0;
    double stat_xr_t = 0.0;
    double end_xr_t = 0.0;
    if(bloom_flag == 1 && cache_flag != 4) { //bloom hit
        if (cache_flag == 1) { //cache hit
            return 1;
        }
        else if (cache_flag == 2) {//cache crash

            if (mp->insert_mt(bch_result, chk_cont, bch_lengh, is_cache)) {
                return 2;
            }
            else {
                std::cout << "insert mt error!" << std::endl;
                exit(-1);
            }
        }
        else {//cache missed

            struct addr *head_addr = NULL;
            char read_buf[READ_LENGTH+1];
            head_addr = mp -> Get_addr(bch_result, bch_lengh);
            if(head_addr == NULL){ //ecc and its' block is not in the mt table
//                chunk_not_dup++; //count the block that not deduplicate
                if(mp -> insert_mt(bch_result, chk_cont, bch_lengh, is_cache)){
                    ++only_ecc;
                    return 2;
                }
                else{
                    std::cout << "insert mt error!" << std::endl;
                    exit(-1);
                }
            }
            else{ //ecc exist, so we need read the block from ssd
                stat_xr_t = ti.get_time();
                {
                    //memset(read_buf, 0, READ_LENGTH+1);
                    ++read_10000_num;
                    ++read_number ;
                    stat_t = ti.get_time();
                    read_block(head_addr, read_buf);
                    end_t = ti.get_time();
                    time_total_read += (end_t - stat_t) * 1000;
                    mid_crash_num ++;
                    if(memcmp(read_buf, chk_cont, READ_LENGTH) == 0){ //chunk exist
                        end_xr_t = ti.get_time();
                        time_total_xr += (end_xr_t - stat_xr_t) * 1000;
                        prefetch_flag = 1;
                        prefetch_offset = head_addr -> offset;
                        return 1;
                    }
//                    head_addr = head_addr -> next;
                }
                end_xr_t = ti.get_time();
                time_total_xr += (end_xr_t - stat_xr_t) * 1000;
//                chunk_not_dup++; //count the block that not deduplicate
                { //insert succeed and ecc crash
                    //struct crash_test *cp = NULL;
                    //cp = new crash_test;
                   // memcpy(cp -> reference1, read_buf, READ_LENGTH);
                   // memcpy(cp -> reference2, chk_cont, READ_LENGTH);
                    //cp -> next = cra_t;
                   // cra_t = cp;
                    ++fade_crash_number;
                    return 3;
                }
/*                else{
                    std::cout << "insert mt error!" << std::endl;
                    exit(-1);
                }
*/           }
        }
    }
    else{ //bloom missed

        //cac -> cache_insert(bch_result, chk_cont, bch_lengh); //insert the block into the cache
//        chunk_not_dup++; //count the block that not deduplicate
        if(mp -> insert_mt(bch_result, chk_cont, bch_lengh, is_cache)) { //insert succeed
            ++only_ecc;
            return 2;
        }
        else{
            std::cout << "insert mt error!" << std::endl;
            exit(-1);
        }
    }
}





int dedup::dedup_noread_mt(char *bch_result, char *chk_cont, int bch_lengh, int cache_flag, int bloom_flag){
    //int mid_crash_num = 0;

    if(bloom_flag == 1 && cache_flag != 4) { //bloom hit
        if (cache_flag == 1) { //cache hit
            return 1;
        }
        else if (cache_flag == 2) {//cache crash

            if (mp->insert_mt(bch_result, chk_cont, bch_lengh, is_cache)) {
                return 2;
            }
            else {
                std::cout << "insert mt error!" << std::endl;
                exit(-1);
            }
        }
        else {//cache missed

            struct addr *head_addr = NULL;
            char read_buf[READ_LENGTH+1];
            head_addr = mp -> Get_addr(bch_result, bch_lengh);
            if(head_addr == NULL){ //ecc and its' block is not in the mt table
//                chunk_not_dup++; //count the block that not deduplicate
                if(mp -> insert_mt(bch_result, chk_cont, bch_lengh, is_cache)){
                    return 2;
                }
                else{
                    std::cout << "insert mt error!" << std::endl;
                    exit(-1);
                }
            }
            else{ //ecc exist, so we need read the block from ssd
                /*while(head_addr != NULL){
                    memset(read_buf, 0, READ_LENGTH+1);
                    read_number ++;
                    stat_t = ti.get_time();
                    read_block(head_addr, read_buf);
                    end_t = ti.get_time();
                    time_total_read += (end_t - stat_t) * 1000;
                    mid_crash_num ++;
                    if(memcmp(read_buf, chk_cont, READ_LENGTH) == 0){ //chunk exist
                        return 1;
                    }
                    head_addr = head_addr -> next;
                }
//                chunk_not_dup++; //count the block that not deduplicate
                if(mp -> insert_mt(bch_result, chk_cont, bch_lengh)){ //insert succeed and ecc crash
                    struct crash_test *cp = NULL;
                    cp = new crash_test;
                    memcpy(cp -> reference1, read_buf, READ_LENGTH);
                    memcpy(cp -> reference2, chk_cont, READ_LENGTH);
                    cp -> next = cra_t;
                    cra_t = cp;
                    fade_crash_number += mid_crash_num;
                    return 2;
                }
                else{
                    std::cout << "insert mt error!" << std::endl;
                    exit(-1);
                }*/
                return 1;
            }
        }
    }
    else{ //bloom missed

        //cac -> cache_insert(bch_result, chk_cont, bch_lengh); //insert the block into the cache
//        chunk_not_dup++; //count the block that not deduplicate
        if(mp -> insert_mt(bch_result, chk_cont, bch_lengh, is_cache)) //insert succeed
            return 2;
        else{
            std::cout << "insert mt error!" << std::endl;
            exit(-1);
        }
    }
}





int dedup::test_crash(char *reference1, char *reference2) {


    if(memcmp(reference1, reference2, BLOCK_SIZE) != 0){
            crash_number ++;
    }

    return 0;
}

/*int dedup::test_all_crash() {
    struct crash_test *p = cra_t;
    while(p != NULL){
        test_crash(p -> reference1, p ->reference2);
        p = p -> next;
    }
    return 0;
}*/

int dedup::avertime_distribute(double &elpstime) {
    int integer;
    integer = (int)(elpstime / 0.001);
    if(integer < 1000) {
        ++time_collect_num[integer];
//        std::cout << elpstime<< "   "<<integer<< "   " <<time_collect_num[integer] << std::endl;
    }
    else {
        ++time_collect_num[999];
//        std::cout << elpstime<< "   "<<integer<< "   " <<time_collect_num[999] << std::endl;
    }

    return 0;
}

int dedup::avertime_distribute_less(double &elpstime) {
    int integer;
    integer = (int)(elpstime / 0.001);
    if(integer < 1000) {
        for(int i = integer; i <= 999; ++i) {
            ++time_collect_num_less[i];
        }
//        std::cout << elpstime<< "   "<<integer<< "   " <<time_collect_num[integer] << std::endl;
    }
    else {
        ++time_collect_num_less[999];
//        std::cout << elpstime<< "   "<<integer<< "   " <<time_collect_num[999] << std::endl;
    }

    return 0;
}

void dedup::prefetch() {
    int i = 0;
    std::string ecc;
    char read_buf[4097];
    struct addr *head_addr = new addr;
    for(; i < prefetch_num; ++i){
        if(mp -> offset_exist(prefetch_offset + i, ecc)){//offset is exist in the storage device
            head_addr -> offset = prefetch_offset + i;
            read_block(head_addr, read_buf);
            dedup_cache(ecc, (char *)read_buf, 1);
        } else{
            break;
        }
    }
    prefetch_flag = 0;
    delete head_addr;
}


