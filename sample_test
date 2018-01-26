#!/usr/bin/env bash

DEVICE_PATH="/dev/nvme0n1"
SAMPLE_FLAG=6
PARALLEL_MODE=1
SAMPLE_LENGHT=4

PREFIX="sample_md5.old_divide.${SAMPLE_LENGHT}B"

OUTPUT_FOLDER=$1


for var in "$@"
do
    option=${var%=*}
    if [ "$option" == "kernel" ]; then
        KERNEL_PATH=${var#kernel=}
    elif [ "$option" == "huawei" ]; then
        HUAWEI_PATH=${var#huawei=}
    elif [ "$option" == "firefox" ]; then
        FIREFOX_PATH=${var#firefox=}
    elif [ "$option" == "vmdk" ]; then
        VMDK_PATH=${var#vmdk=}
    elif [ "$option" == "test_folder" ]; then
        OUTPUT_FOLDER=${var#test_folder=}
    else
        echo "Wrong parameters!"
        exit 1
    fi
done


if [ ! -d "$OUTPUT_FOLDER" ]; then
    mkdir "$OUTPUT_FOLDER"
fi

###################################################################
#                   The sample length is 4
###################################################################

PREFIX="sample.old_divide.${SAMPLE_LENGHT}B"
echo "./EaD -a $KERNEL_PATH -b $DEVICE_PATH -c $SAMPLE_FLAG -d $PARALLEL_MODE -s $SAMPLE_LENGHT > ${OUTPUT_FOLDER}/${PREFIX}.kernel"
./EaD -a $KERNEL_PATH -b $DEVICE_PATH -c $SAMPLE_FLAG -d $PARALLEL_MODE -s $SAMPLE_LENGHT > ${OUTPUT_FOLDER}/${PREFIX}.kernel

echo "./EaD -a $FIREFOX_PATH -b $DEVICE_PATH -c $SAMPLE_FLAG -d $PARALLEL_MODE -s $SAMPLE_LENGHT > ${OUTPUT_FOLDER}/${PREFIX}.firefox"
./EaD -a $FIREFOX_PATH -b $DEVICE_PATH -c $SAMPLE_FLAG -d $PARALLEL_MODE -s $SAMPLE_LENGHT > ${OUTPUT_FOLDER}/${PREFIX}.firefox

echo "./EaD -a $HUAWEI_PATH -b $DEVICE_PATH -c $SAMPLE_FLAG -d $PARALLEL_MODE -s $SAMPLE_LENGHT > ${OUTPUT_FOLDER}/${PREFIX}.huawei"
./EaD -a $HUAWEI_PATH -b $DEVICE_PATH -c $SAMPLE_FLAG -d $PARALLEL_MODE -s $SAMPLE_LENGHT > ${OUTPUT_FOLDER}/${PREFIX}.huawei

echo "./EaD -a $VMDK_PATH -b $DEVICE_PATH -c $SAMPLE_FLAG -d $PARALLEL_MODE -s $SAMPLE_LENGHT > ${OUTPUT_FOLDER}/${PREFIX}.vmdk"
./EaD -a $VMDK_PATH -b $DEVICE_PATH -c $SAMPLE_FLAG -d $PARALLEL_MODE -s $SAMPLE_LENGHT > ${OUTPUT_FOLDER}/${PREFIX}.vmdk
