#!/bin/bash

#These tests use Mobile system data set.
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.kernel.old.nvme.nocache.m8t13
sudo ./test.all.m8t10 /mnt/cdrom/kernel /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.kernel.old.nvme.nocache.m8t10
sudo ./test.all.m8t8 /mnt/cdrom/kernel /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.kernel.old.nvme.nocache.m8t8



#These tests use firefox system data set.

sudo ./test.all.m8t10 /mnt/cdrom/vmdk /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.vmdk.old.nvme.nocache.m8t10
sudo ./test.all.m8t8 /mnt/cdrom/vmdk /dev/nvme0n1 0 1 0 50000 > output_folder/ecc_length/ecc.vmdk.old.nvme.nocache.m8t8
