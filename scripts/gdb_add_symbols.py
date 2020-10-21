#!/bin/python/

import os, sys, glob

root_dir = "~/dev/project/"
source_dir = root_dir + "source/"
bin_dir = root_dir + "build/"

# add source directories
gdb.execute("directory " + source_dir)
src_subdirs = [x[0] for x in os.walk(source_dir)]
for sd in src_subdirs:
    gdb.execute("directory " + sd)

# connect to target
gdb.execute("target remote localhost:9999")

# set debug file directory
bin_subdirs = [x[0] for x in os.walk(bin_dir)]
dfd = ""
for bd in bin_subdirs:
    dfd += bd + ":"
gdb.execute("set debug-file-directory " + dfd)

#gdb.execute("symbol-file " + bin_dir + "<specific_address>")
# add symbol files
for bd in bin_subdirs:
    for file in os.listdir(bd):
        if file.endswith(".elf"):
            fp = bd + "/" + file
            stream = os.popen('echo $(readelf -WS {}'.format(fp) + "| grep '.text ' | awk '{ print $5 }')")
            output = stream.read()
            gdb.execute("add-symbol-file " + fp + "0x" + output)
