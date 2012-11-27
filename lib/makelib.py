#! /usr/bin/env python
# Instructions
# 1. copy all static libraries to directory this script is in
# 2. run this script

import os

os.chdir(".")

liblist = []

#unpack everything
for files in os.listdir("."):
    if files.endswith(".a"):
        print(files)
        liblist.append(files)
        cmd = "ar -x " + files
        print("cmd : " + cmd)
        os.system(cmd)
            

#pack everything to a new lib
cmd = "ar -rv libfinal.a *.o"
os.system(cmd)
os.system("rm -rf *.o")
