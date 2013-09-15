elfdump -ecps elfinfo > out.elfdump
./elfinfo elfinfo > out.elfinfo
diff -b -w -c out.elfinfo out.elfdump > out.diff

echo
echo "DIFF-->"
cat out.diff
echo "DONE-->"
echo

elfdump -ecps /usr/lib/libc.so > out.elfdump
./elfinfo /usr/lib/libc.so > out.elfinfo
diff -b -w -c out.elfinfo out.elfdump > out.diff

echo
echo "DIFF-->"
cat out.diff
echo "DONE-->"
echo

elfdump -ecps /usr/bin/ls > out.elfdump
./elfinfo /usr/bin/ls > out.elfinfo
diff -b -w -c out.elfinfo out.elfdump > out.diff

echo
echo "DIFF-->"
cat out.diff
echo "DONE-->"
echo

