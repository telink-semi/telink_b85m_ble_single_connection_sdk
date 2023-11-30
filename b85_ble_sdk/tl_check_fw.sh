
echo "*****************************************************"
echo "this is post build!! current configure is :$1"

os=$(uname)
echo "OS from uname is: ${os}"


echo $1
echo $2
tc32-elf-objcopy -v -O binary $2.elf  $1.bin
#../tl_check_fw2.exe  $1.bin
if [ "${os}" = "Linux" ] ; then
    echo "Linux OS"
    echo "check_fw in Linux..."
    chmod 755 ../check_fw
    ../check_fw $1.bin
else
    echo "Windows OS"
    echo "check_fw in Windows..."
    ../tl_check_fw2.exe  $1.bin
fi
echo  "---------------------------  SDK version info ---------------------------"
str=$(grep -E "[\$]{3}[a-zA-Z0-9 _.]+[\$]{3}" --text -o $1.bin | sed 's/\$//g')
if [ -z "$str" ]; then
    echo "no SDK version found at the end of firmware, please check sdk_version.c and sdk_version.h"
else
    echo "$str"
fi
echo  "---------------------------  SDK version end  ---------------------------"
echo "**************** end of post build ******************"
