cd ~/openFrameworks/apps/myapps/myapp/bin/
ret=1
while [ $ret -ne 0 ]; do
./myapp
ret=$?
done
