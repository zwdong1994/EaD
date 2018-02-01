#EaD

EaD is an ECC assisted Deduplication scheme, which exploits the ECC values of data chunks and it completely avoiding the
costly cryptographic hash computing. In our evaluation, comparing with existing MD5/SHA- and sampling-based 
deduplication schemes, EaD's I/O performance is up to 4.2x, with and average of 2.5x.

#Abstract

With the advent of NVMe and 3D NAND flash technologies, the performance of flash-based storage systems has been improved
significantly. This has brought about a noticeable change in the performance landscape of flash-based deduplication 
storage systems in that the performance bottleneck is observed to shift from the I/O stack to the compute layer due to
the costly computing of cryptographic hash functions. Moreover, these functions, such as MD5, SHA-1, and SHA-256, have 
nonzero probabilities of hash-collision, which in theory can cause unique/non-redundant data chunks to be falsely 
detected and removed as redundant ones and thus lead to unrecoverable data loss. To address these problems, we propose 
an ECC assisted Deduplication scheme, called EaD, which exploits the ECC property within each data chunk of modern 
flash-based storage to first identify data similarity based on the ECC values of data chunks, completely avoiding the 
costly cryptographic hash computing. Based on the identification results, similar data chunks are read from the flash to
perform a byte-to-byte comparison to definitively identify and remove redundant data chunks. Experiments conducted on 
our lightweight prototype implementation of the EaD system show that the hash-collision-free EaD significantly 
outperforms the existing MD5/SHA- and sampling-based deduplication schemes in terms of I/O performance by up to 4.2×, 
with an average of 2.5×.

#ABout this repository

This repository is a lightweight prototype which are aimed to evaluate the performance of EaD compared to existing 
deduplication systems. Besides, this project is a dataset-driven evaluation. We have chosen MD5, sha1 
and SHA256 as hash functions in the traditional deduplication system. Moreover, we also adopt sampling-based 
deduplication schems to our evaluation for a more comprehensive evaluation.


##Start

To start our evaluation, we need to have several operations as follows:

    cmake .
    make
    sudo bash run    //This is a test shell script, and if you want to run it, you need modify some parameters in it.


##Setup

    Chunk Size:             4KB
    ECC Algorithm:          BCH
    ECC Value Length:       8B
    Prefetch Chunk Count:   2
    Hash Functions:         MD5, SHA1, SHA256
    Sampling Length:        the first 4B of each chunks

##Datasets



| Applications | Size(GB) | Dedup.Ratio | Comments |  
| :-------------: |:-------------:| :-----:| :------: |
| VMDK | 332 | 49.1% | VM disk image files |
| Kernel | 190 | 92.6% | Linux Kernel source code |
| MobileSys | 256 | 36.2% | System files fo Smartphone |
| Firefox | 287 | 74.8% | Firefox installation files |


##Perfomance

Here is a performance report from run the 'run' shell script with specify parameters in the setup.

##Average response time 

Our experimental results show that EaD outperforms the MD5-, SHA-1-, SHA-256-, and sampling-based deduplication schemes 
by up to 2.8×, 2.6×, 4.2× and 2.7×, respectively, with an average of 2.5×. The reason is that EaD does not incur any 
cryptographic hash computing latency on the critical write path.




