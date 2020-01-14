strace -tt -yy -y -f -e trace=file,pipe,socket,dup2,clone,close,ipc,signal,desc,memory,process -s 10000 -o ~/tmp/fuzz/strace-real `cat ./run1.bash`
