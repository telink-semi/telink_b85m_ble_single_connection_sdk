#!/bin/bash 
echo "*****************************************************"
tc32-elf-objcopy -v -O binary $2.elf  $1.bin

echo "check_fw in Windows..."
../../../../script/tl_check_fw/tl_check_fw2.exe  $1.bin
	
../../../../script/bin_append_crc/bin_append.exe $1.bin $1_NEW.bin 
if [ $? == 0 ];then echo "$1.bin tail add crc finish !";fi;echo "*****************************************************"