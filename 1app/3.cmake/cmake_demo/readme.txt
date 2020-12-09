备注：

config.h.in需要手动创建，放在工程目录下。
自动生成的config.h文件，main包含此文件时注意路径。

Your Hardware Enablement Stack (HWE) is supported until April 2023.
You have new mail.
Last login: Wed Dec  9 09:39:42 2020 from 192.168.198.1
root@ubuntu:~# cd /mnt/hgfs/branches/clionproj/cmake_demo/build
root@ubuntu:/mnt/hgfs/branches/clionproj/cmake_demo/build# cmake ..
-- The C compiler identification is GNU 4.8.5
-- The CXX compiler identification is GNU 4.8.5
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/hgfs/branches/clionproj/cmake_demo/build
root@ubuntu:/mnt/hgfs/branches/clionproj/cmake_demo/build# ls
CMakeCache.txt  CMakeFiles  cmake_install.cmake  Makefile
root@ubuntu:/mnt/hgfs/branches/clionproj/cmake_demo/build# cd ..
root@ubuntu:/mnt/hgfs/branches/clionproj/cmake_demo# ls
build  CMakeLists.txt  main.c
root@ubuntu:/mnt/hgfs/branches/clionproj/cmake_demo# cd build/
root@ubuntu:/mnt/hgfs/branches/clionproj/cmake_demo/build# make
Scanning dependencies of target hello
[ 50%] Building C object CMakeFiles/hello.dir/main.c.o
[100%] Linking C executable hello
[100%] Built target hello
root@ubuntu:/mnt/hgfs/branches/clionproj/cmake_demo/build# ls
CMakeCache.txt  CMakeFiles  cmake_install.cmake  hello  Makefile
root@ubuntu:/mnt/hgfs/branches/clionproj/cmake_demo/build# ./hello 
Hello World!


