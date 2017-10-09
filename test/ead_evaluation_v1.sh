#!/bin/bash

#These tests use Mobile system data set.

sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 0 1 50000 > output_folder/mobilesys/ecc.mobile.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 1 0 0 50000 > output_folder/mobilesys/md5.mobile.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 2 0 0 50000 > output_folder/mobilesys/sha256.mobile.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 3 0 0 50000 > output_folder/mobilesys/sha1.mobile.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 0 1 100000 > output_folder/mobilesys/ecc.mobile.new.nvme.100000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 0 1 200000 > output_folder/mobilesys/ecc.mobile.new.nvme.200000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 1 50000 > output_folder/mobilesys/ecc.mobile.old.nvme.50000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 1 100000 > output_folder/mobilesys/ecc.mobile.old.nvme.100000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 1 200000 > output_folder/mobilesys/ecc.mobile.old.nvme.200000
sudo ./dedup.all /mnt/cdrom/app /dev/nvme0n1 0 1 0 50000 > output_folder/mobilesys/ecc.mobile.old.nvme.nocache


#These tests use firefox system data set.

sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 0 1 50000 > output_folder/firefox/ecc.firefox.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 1 0 0 50000 > output_folder/firefox/md5.firefox.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 2 0 0 50000 > output_folder/firefox/sha256.firefox.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 3 0 0 50000 > output_folder/firefox/sha1.firefox.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 0 1 100000 > output_folder/firefox/ecc.firefox.new.nvme.100000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 0 1 200000 > output_folder/firefox/ecc.firefox.new.nvme.200000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 1 50000 > output_folder/firefox/ecc.firefox.old.nvme.50000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 1 100000 > output_folder/firefox/ecc.firefox.old.nvme.100000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 1 200000 > output_folder/firefox/ecc.firefox.old.nvme.200000
sudo ./dedup.all /mnt/cdrom/firefox /dev/nvme0n1 0 1 0 50000 > output_folder/firefox/ecc.firefox.old.nvme.nocache

