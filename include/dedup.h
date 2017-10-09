//
// Created by victor on 6/1/17.
//

#ifndef ED_DEDUP_H
#define ED_DEDUP_H

#include <stdint.h>

#include "com_t.h"
#include "mt.h"
#include "bch.h"
#include "bloom_func.h"
//#include "cache.h"
#include "new_cache.h"

#define READ_LENGTH 4096
#define PTHREAD_NUM 6
#define CONFIG_M 8
#define CONFIG_T 13
#define CODE_LENGTH 13
#define MD5_CODE_LENGTH 16
#define SHA256_CODE_LENGTH 32
#define SHA1_CODE_LENGTH 20

/*struct crash_test{
    char reference1[READ_LENGTH];
    char reference2[READ_LENGTH];
    struct crash_test *next;
};*/
class dedup{
public:
    dedup();
    ~dedup();

    void travel_dir(char path[]);
    void travel_dir_noparallel(char path[]);
    int dedup_func(char path[], char dev[], int mode, int cache_mode, int cache_flag, int cache_size, int prefetch_length);
    int file_reader(char *path);
    int file_reader_noparallel(char *path);
    int md5_file_reader(char *c_path);
    int md5_file_reader_noparallel(char *c_path);
    int sha256_file_reader(char *c_path);
    int sha256_file_reader_noparallel(char *c_path);
    int sha1_file_reader(char *c_path);
    int sha1_file_reader_noparallel(char *c_path);
    //int dedup_process(char bch_result[], char *chk_cont, int bch_length);
    uint64_t chunk_num;
    double time_total;
    double time_aver;
    double time_total_read;
    double time_total_write;
    double time_total_cache;
    double time_total_xr;
    double hash_time;
    uint64_t chunk_not_dup;
    uint64_t only_ecc;
    uint64_t read_number;
    uint64_t read_10000_num;
    uint64_t collision_write_num;

private:

    typedef struct mid_para{
        pthread_t pid;
        dedup *this_;
        char path[512];
        struct mid_para *next;
    }para;
    cp_t ti;
    uint64_t time_collect_num[1000]; //this array record the number of average time in every 0.001ms(0.001ms - 0.1ms -......)
    uint64_t time_collect_num_less[1000]; //this array record the number of average time in every 0.001ms(0.001ms - 0.1ms -......)
    static void *start_pthread(void *arg);
    void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
    int avertime_distribute(double &elpstime);
    int avertime_distribute_less(double &elpstime);
    struct bch_control *bch;
    int dedup_bloom(char bch_result[], int bch_length);
    int dedup_cache(std::string bch_result, char *chk_cont, int bloom_flag);
    int dedup_mt(char bch_result[], char *chk_cont, int bch_lengh, int cache_flag, int bloom_flag);
    int dedup_noread_mt(char bch_result[], char *chk_cont, int bch_lengh, int cache_flag, int bloom_flag);
    int test_crash(char *reference1, char *reference2);
//    int test_all_crash();
    void prefetch();
    int crash_number;
    int fade_crash_number;
    int is_cache;
    struct crash_test *cra_t;
    uint64_t block_id;
    uint64_t cache_hit_num;
    double head_10000_time;
    double mid_elpstime;

    int file_number;

    unsigned long prefetch_offset;
    int prefetch_flag;
    int prefetch_num;

    para *head_pthread_para;

    mt *mp;
    new_cache *cac;
    bloom *blf;
};




#endif //ED_DEDUP_H
