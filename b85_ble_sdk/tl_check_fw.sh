
echo "*****************************************************"
echo "this is post build!! current configure is :$1"

os=$(uname)
echo "OS from uname is: ${os}"


echo $1
echo $2
tc32-elf-objcopy -v -O binary $2.elf  $1.bin
python ../utils/memap/memap.py -t GCC_ARM -e json -o $1.mem $1.map
python ../utils/memap/memap.py -t GCC_ARM -e table $1.map 2>&1 | tee $1.tbl
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

echo "**************** end of post build ******************"
