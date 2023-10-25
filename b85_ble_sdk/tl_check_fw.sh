
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
echo '---------------------------------------------'
echo "Section Header of elf:"
tc32-elf-readelf -S  $2.elf
echo
echo "Section list:"
SEC=$(tc32-elf-readelf -S  $2.elf | grep '\[' | sed 's#.*]##' | awk -F ' '  '{print $1}' | grep -v NULL | grep -v Name  | xargs)
echo "${SEC}"
echo
echo "Section size total:"
SUM_EXPRE=$(tc32-elf-readelf -S  $2.elf | awk '{print $6}' | tr -d '[:blank:]' | grep "[[:digit:]]" |  tr '[:lower:]' '[:upper:]'  | bc | xargs |  sed 's/0 //')
SUM=$(echo "${SUM_EXPRE}" | sed 's# #\+#g' | bc)
echo "${SUM_EXPRE} : ${SUM} bytes"
echo '---------------------------------------------'
echo  "---------------------------  SDK version info ---------------------------"
str=$(grep -E "[\$]{3}[a-zA-Z0-9 _.]+[\$]{3}" --text -o $1.bin | sed 's/\$//g')
if [ -z "$str" ]; then
    echo "no SDK version found at the end of firmware, please check sdk_version.c and sdk_version.h"
else
    echo "$str"
fi
echo  "---------------------------  SDK version end  ---------------------------"
echo "**************** end of post build ******************"
