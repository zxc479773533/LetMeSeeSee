#!/bin/bash

# This script is for travis-ci, do not run it locally!!!

mkdir scan_test
cp -r ../test scan_test
cd scan_test

for i in $(seq 1000); do
  name=$(printf test%02d $i);
  cp -r test "$name"
done

cd ..
time ./func_test scan_test
rm -rf scan_test

./func_test &
sleep 1
time ./net_test 100000
ps -ef | grep "func_test" | grep -v grep | awk '{print $2}' | xargs kill -9