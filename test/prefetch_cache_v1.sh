#!/bin/bash

#Kernel data set
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 0 50000 > output_folder/prefetch/ecc.kernel.old.nvme.nocache
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 1 50000 > output_folder/prefetch/ecc.kernel.old.nvme.50000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 1 100000 > output_folder/prefetch/ecc.kernel.old.nvme.100000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 1 150000 > output_folder/prefetch/ecc.kernel.old.nvme.150000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 1 200000 > output_folder/prefetch/ecc.kernel.old.nvme.200000
#sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 0 50000 > output_folder/prefetch/ecc.kernel.old.nvme.nocache

#vmdk data set
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 0 50000 > output_folder/prefetch/ecc.vmdk.old.nvme.nocache
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 1 50000 > output_folder/prefetch/ecc.vmdk.old.nvme.50000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 1 100000 > output_folder/prefetch/ecc.vmdk.old.nvme.100000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 1 150000 > output_folder/prefetch/ecc.vmdk.old.nvme.150000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 1 200000 > output_folder/prefetch/ecc.vmdk.old.nvme.200000
#sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 0 50000 > output_folder/prefetch/ecc.vmdk.old.nvme.nocache

