//
// Created by victor on 6/2/17.
//

#include "mt.h"


#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "com_t.h"

static struct aiocb64 aio;
static char *dev_name;
static int mt_fd;
static struct aiocb64 myaio;
cp_t ti;

mt::mt(char *devname) {
    bzero((char *)&myaio, sizeof(struct aiocb64));
    myaio.aio_buf = malloc(4097);
    if(!myaio.aio_buf)
        perror("malloc\n");
    write_time = 0;
    time_total = 0.0;
    ssd_capacity = 300;
    max_size_addr = ssd_capacity * 1024 * 1024 / 4;
    alloc_addr_point = 1;
    dev_name = new char[30];
    strcpy(dev_name, devname);
    mt_fd = open(dev_name, O_RDWR|O_LARGEFILE);
    if(mt_fd == -1){
        std::cout<<"open "<< dev_name <<" error!"<<std::endl;
        exit(-1);
    }
}

mt::~mt() {
//    std::cout<< "write average time is: " << time_total / write_time << std::endl;
    std::map<std::string, struct addr*>::iterator it;
    struct addr *p = NULL, *dp = NULL;
    for(it = mt_container.begin(); it != mt_container.end(); ++it){
        p = it -> second;
        while(p != NULL){
            dp = p;
            p = p -> next;
            delete dp;
        }
    }
    close(mt_fd);
}

mt* mt::mt_instance = NULL;

mt* mt::Get_mt(char *devname){
    static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
    if(mt_instance == NULL){
        pthread_mutex_lock(&mu);
        if(mt_instance == NULL) {
            mt_instance = new mt(devname);
            strcpy(dev_name, devname);
        }
        pthread_mutex_unlock(&mu);
    }
    return mt_instance;
}

struct addr* mt::Get_addr(char *ecc_code, int length_ecc) {
    std::string str;
    ecc_code[length_ecc] = '\0';
    str = ecc_code;
    if(mt_container[str] != NULL){ //exist
        return mt_container[str];
    }
    else
        return NULL;

}

int mt::insert_mt(char *ecc_code, char chunk_reference[], int length_ecc, int cache_flag) {
    struct addr *p = NULL;
    p = new addr;

    if(alloc_addr_point < max_size_addr){
        std::string str;
        ecc_code[length_ecc] = '\0';
        str = ecc_code;
        p -> offset = alloc_addr_point;
        p -> next = NULL;
        if(cache_flag)
            offset_ecc[alloc_addr_point] = ecc_code; //record the ecc of every allocated address.
        alloc_addr_point++; //allocate succeed
        if(mt_container[str] != NULL){
            p -> next = (struct addr*)mt_container[str];
            mt_container[str] = p;
//            write_block(p, chunk_reference);
            return 1;
        }
        else{
            mt_container[str] = p;
//            write_block(p, chunk_reference);
            return 1;
        }

    }
    return 0;
}

int mt::offset_exist(unsigned long offset, std::string &get_str) {
//    std::map<unsigned long, std::string>::iterator iter_offset_ecc;
//    iter_offset_ecc = offset_ecc.find(offset);
//    if(iter_offset_ecc != offset_ecc.end()){
    if(offset < alloc_addr_point){
        get_str = offset_ecc[offset];
        return 1;
    }
    else
        return 0;
}

int write_block(unsigned long offset, char *chunk_reference, double &elpstime) {

    double stat_t = 0.0, end_t = 0.0;
//    std::cout << "11" << std::endl;


//    std::cout << "11" << std::endl;
//    struct aiocb64 *cblist[1];
    bzero((char *)&aio, sizeof(struct aiocb64));
//    bzero( (char *)cblist, sizeof(cblist) );
    aio.aio_buf = myaio.aio_buf;

    aio.aio_fildes = mt_fd;
    aio.aio_nbytes = BLOCK_SIZE;
    aio.aio_offset = offset * BLOCK_SIZE;
    memcpy((void *)aio.aio_buf, (void *)chunk_reference, BLOCK_SIZE);
    stat_t = ti.get_time();
//    cblist[0] = &aio;
//    std::cout << "11" << std::endl;
    aio_write64(&aio);
//    aio_suspend64(cblist, 1, NULL);
    while(EINPROGRESS == aio_error64(&aio));
//    std::cout << "11" << std::endl;
    end_t = ti.get_time();
    elpstime = (end_t - stat_t) * 1000;
//    std::cout<< "write average time is: " << (end_t - stat_t) * 1000 << std::endl;
//    time_total += ((end_t - stat_t) * 1000);
//    write_time++;
//    free((void *)aio.aio_buf);

//    std::cout << "11" << std::endl;
    return 1;
}

int read_block(struct addr *write_addr, char *chunk_reference) {


//    struct aiocb64 *cblist[1];
    bzero((char *)&aio, sizeof(struct aiocb64));
//    bzero( (char *)cblist, sizeof(cblist) );
    aio.aio_buf = myaio.aio_buf;

    aio.aio_fildes = mt_fd;
    aio.aio_nbytes = BLOCK_SIZE;
    aio.aio_offset = write_addr->offset * BLOCK_SIZE;

    aio_read64(&aio);
    while (EINPROGRESS == aio_error64(&aio));
    memset(chunk_reference, 0, BLOCK_SIZE + 1);
    memcpy((void *) chunk_reference, (void *) aio.aio_buf, BLOCK_SIZE);
//    free((void *)aio.aio_buf);
    return 1;
}
