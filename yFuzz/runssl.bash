version=$1
bin=openssl$version
cd testcases/$bin && ../../afl-fuzz -i in -o out -m none -f input -t 400  -p 1 ./$bin



