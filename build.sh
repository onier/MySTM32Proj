make clean && make -j
while ! st-flash write build/MySTM32Proj.bin 0x8000000
do
    echo waiting for missingfile
    sleep 2
done

sleep 1
st-util