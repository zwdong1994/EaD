#!/bin/bash

#These tests use kernel system data set.

sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 0 1 50000 > output_folder/kernel/ecc.kernel.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 1 0 0 50000 > output_folder/kernel/md5.kernel.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 2 0 0 50000 > output_folder/kernel/sha256.kernel.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 3 0 0 50000 > output_folder/kernel/sha1.kernel.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 0 1 100000 > output_folder/kernel/ecc.kernel.new.nvme.100000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 0 1 200000 > output_folder/kernel/ecc.kernel.new.nvme.200000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 1 50000 > output_folder/kernel/ecc.kernel.old.nvme.50000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 1 100000 > output_folder/kernel/ecc.kernel.old.nvme.100000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 1 200000 > output_folder/kernel/ecc.kernel.old.nvme.200000
sudo ./dedup.all /mnt/cdrom/kernel /dev/nvme0n1 0 1 0 50000 > output_folder/kernel/ecc.kernel.old.nvme.nocache


#These tests use vmdk system data set.

sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 0 1 50000 > output_folder/vmdk/ecc.vmdk.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 1 0 0 50000 > output_folder/vmdk/md5.vmdk.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 2 0 0 50000 > output_folder/vmdk/sha256.vmdk.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 3 0 0 50000 > output_folder/vmdk/sha1.vmdk.new.nvme.50000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 0 1 100000 > output_folder/vmdk/ecc.vmdk.new.nvme.100000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 0 1 200000 > output_folder/vmdk/ecc.vmdk.new.nvme.200000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 1 50000 > output_folder/vmdk/ecc.vmdk.old.nvme.50000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 1 100000 > output_folder/vmdk/ecc.vmdk.old.nvme.100000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 1 200000 > output_folder/vmdk/ecc.vmdk.old.nvme.200000
sudo ./dedup.all /mnt/cdrom/vmdk /dev/nvme0n1 0 1 0 50000 > output_folder/vmdk/ecc.vmdk.old.nvme.nocache

