#!/bin/bash

excute_name_50000=dedup.real.write_time.cache50000.pthread6
file_suffix_50000=vmdk.realwritetime.cache50000.pthread6

excute_name_100000=dedup.real.write_time.cache100000.pthread6
file_suffix_100000=vmdk.realwritetime.cache100000.pthread6

excute_name_200000=dedup.real.write_time.cache200000.pthread6
file_suffix_200000=vmdk.realwritetime.cache200000.pthread6

excute_name_nocache=dedup.real.write_time.nocache.pthread6
file_suffix_nocache=vmdk.realwritetime.nocache.pthread6

sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdh1 0 0> vmdk_data/ecc.ssd500G.new.${file_suffix_50000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdh1 1 0> vmdk_data/md5.ssd500G.new.${file_suffix_50000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdh1 2 0> vmdk_data/sha256.ssd500G.new.${file_suffix_50000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdi1 0 0> vmdk_data/ecc.hdd.new.${file_suffix_50000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdi1 1 0> vmdk_data/md5.hdd.new.${file_suffix_50000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdi1 2 0> vmdk_data/sha256.hdd.new.${file_suffix_50000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/nvme0n1 0 0> vmdk_data/ecc.nvme.new.${file_suffix_50000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/nvme0n1 1 0> vmdk_data/md5.nvme.new.${file_suffix_50000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/nvme0n1 2 0> vmdk_data/sha256.nvme.new.${file_suffix_50000} ##sha256 mode, vmdk data set and the time of write is real write time

#########################################################################
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdh1 0 1> vmdk_data/ecc.ssd500G.old.${file_suffix_50000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdh1 1 1> vmdk_data/md5.ssd500G.old.${file_suffix_50000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdh1 2 1> vmdk_data/sha256.ssd500G.old.${file_suffix_50000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdi1 0 1> vmdk_data/ecc.hdd.old.${file_suffix_50000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdi1 1 1> vmdk_data/md5.hdd.old.${file_suffix_50000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/sdi1 2 1> vmdk_data/sha256.hdd.old.${file_suffix_50000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/nvme0n1 0 1> vmdk_data/ecc.nvme.old.${file_suffix_50000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/nvme0n1 1 1> vmdk_data/md5.nvme.old.${file_suffix_50000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_50000} /mnt/cdrom/vmdk/ /dev/nvme0n1 2 1> vmdk_data/sha256.nvme.old.${file_suffix_50000} ##sha256 mode, vmdk data set and the time of write is real write time


#########################################################################
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdh1 0 0> vmdk_data/ecc.ssd500G.new.${file_suffix_100000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdh1 1 0> vmdk_data/md5.ssd500G.new.${file_suffix_100000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdh1 2 0> vmdk_data/sha256.ssd500G.new.${file_suffix_100000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdi1 0 0> vmdk_data/ecc.hdd.new.${file_suffix_100000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdi1 1 0> vmdk_data/md5.hdd.new.${file_suffix_100000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdi1 2 0> vmdk_data/sha256.hdd.new.${file_suffix_100000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/nvme0n1 0 0> vmdk_data/ecc.nvme.new.${file_suffix_100000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/nvme0n1 1 0> vmdk_data/md5.nvme.new.${file_suffix_100000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/nvme0n1 2 0> vmdk_data/sha256.nvme.new.${file_suffix_100000} ##sha256 mode, vmdk data set and the time of write is real write time

#########################################################################
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdh1 0 1> vmdk_data/ecc.ssd500G.old.${file_suffix_100000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdh1 1 1> vmdk_data/md5.ssd500G.old.${file_suffix_100000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdh1 2 1> vmdk_data/sha256.ssd500G.old.${file_suffix_100000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdi1 0 1> vmdk_data/ecc.hdd.old.${file_suffix_100000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdi1 1 1> vmdk_data/md5.hdd.old.${file_suffix_100000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/sdi1 2 1> vmdk_data/sha256.hdd.old.${file_suffix_100000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/nvme0n1 0 1> vmdk_data/ecc.nvme.old.${file_suffix_100000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/nvme0n1 1 1> vmdk_data/md5.nvme.old.${file_suffix_100000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_100000} /mnt/cdrom/vmdk/ /dev/nvme0n1 2 1> vmdk_data/sha256.nvme.old.${file_suffix_100000} ##sha256 mode, vmdk data set and the time of write is real write time


#########################################################################
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdh1 0 0> vmdk_data/ecc.ssd500G.new.${file_suffix_200000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdh1 1 0> vmdk_data/md5.ssd500G.new.${file_suffix_200000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdh1 2 0> vmdk_data/sha256.ssd500G.new.${file_suffix_200000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdi1 0 0> vmdk_data/ecc.hdd.new.${file_suffix_200000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdi1 1 0> vmdk_data/md5.hdd.new.${file_suffix_200000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdi1 2 0> vmdk_data/sha256.hdd.new.${file_suffix_200000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/nvme0n1 0 0> vmdk_data/ecc.nvme.new.${file_suffix_200000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/nvme0n1 1 0> vmdk_data/md5.nvme.new.${file_suffix_200000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/nvme0n1 2 0> vmdk_data/sha256.nvme.new.${file_suffix_200000} ##sha256 mode, vmdk data set and the time of write is real write time

#########################################################################
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdh1 0 1> vmdk_data/ecc.ssd500G.old.${file_suffix_200000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdh1 1 1> vmdk_data/md5.ssd500G.old.${file_suffix_200000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdh1 2 1> vmdk_data/sha256.ssd500G.old.${file_suffix_200000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdi1 0 1> vmdk_data/ecc.hdd.old.${file_suffix_200000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdi1 1 1> vmdk_data/md5.hdd.old.${file_suffix_200000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/sdi1 2 1> vmdk_data/sha256.hdd.old.${file_suffix_200000} ##sha256 mode, vmdk data set and the time of write is real write time

sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/nvme0n1 0 1> vmdk_data/ecc.nvme.old.${file_suffix_200000} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/nvme0n1 1 1> vmdk_data/md5.nvme.old.${file_suffix_200000} ##md5 mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_200000} /mnt/cdrom/vmdk/ /dev/nvme0n1 2 1> vmdk_data/sha256.nvme.old.${file_suffix_200000} ##sha256 mode, vmdk data set and the time of write is real write time


#########################################################################



sudo ./${excute_name_nocache} /mnt/cdrom/vmdk/ /dev/sdh1 0 1> vmdk_data/ecc.ssd500G.${file_suffix_nocache} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_nocache} /mnt/cdrom/vmdk/ /dev/sdi1 0 1> vmdk_data/ecc.hdd.${file_suffix_nocache} ##ecc mode, vmdk data set and the time of write is real write time
sudo ./${excute_name_nocache} /mnt/cdrom/vmdk/ /dev/nvme0n1 0 1> vmdk_data/ecc.nvme.${file_suffix_nocache} ##ecc mode, vmdk data set and the time of write is real write time

