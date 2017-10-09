#!/bin/bash

#Mobile system data set
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 0 50000 > output_folder/prefetch/ecc.mobile.old.nvme.nocache
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 1 50000 > output_folder/prefetch/ecc.mobile.old.nvme.50000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 1 100000 > output_folder/prefetch/ecc.mobile.old.nvme.100000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 1 150000 > output_folder/prefetch/ecc.mobile.old.nvme.150000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 1 200000 > output_folder/prefetch/ecc.mobile.old.nvme.200000
#sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 0 50000 > output_folder/prefetch/ecc.mobile.old.nvme.nocache

#firefox data set
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 0 50000 > output_folder/prefetch/ecc.firefox.old.nvme.nocache
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 1 50000 > output_folder/prefetch/ecc.firefox.old.nvme.50000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 1 100000 > output_folder/prefetch/ecc.firefox.old.nvme.100000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 1 150000 > output_folder/prefetch/ecc.firefox.old.nvme.150000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 1 200000 > output_folder/prefetch/ecc.firefox.old.nvme.200000
#sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 0 50000 > output_folder/prefetch/ecc.firefox.old.nvme.nocache


