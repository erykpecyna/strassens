make &>/dev/null
for i in {600..700}
do 
echo "CUTOFF=" $i >> results.txt
  make clean &>/dev/null
  echo "STANDARD" >> results.txt
  make CUTOFF=1000 &>/dev/null
  { time ./strassen 0 $((i+1)) test3.txt &>/dev/null; } 2>> results.txt
  make clean &>/dev/null
  echo "STRASSENS" >> results.txt
  make CUTOFF=$i &>/dev/null
  { time ./strassen 0 $((i+1)) test3.txt  &>/dev/null; } 2>> results.txt
  echo ------------------ >> results.txt
done