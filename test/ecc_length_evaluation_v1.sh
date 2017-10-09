#!/bin/bash

#These tests use Mobile system data set.

sudo ./test.all.m8t10 /mnt/cdrom/app /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.mobile.old.nvme.nocache.m8t10
sudo ./test.all.m8t8 /mnt/cdrom/app /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.mobile.old.nvme.nocache.m8t8



#These tests use firefox system data set.

sudo ./test.all.m8t10 /mnt/cdrom/firefox /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.firefox.old.nvme.nocache.m8t10
sudo ./test.all.m8t8 /mnt/cdrom/firefox /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.firefox.old.nvme.nocache.m8t8
