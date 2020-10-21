#!/bin/python3

import os, sys, glob

if len(sys.argv) < 2:
    raise Exception("Please provide the path to the directory containing .elf files!")

bin_dir = sys.argv[1]
# add symbol files
for file in os.listdir(bin_dir):
    if file.endswith(".elf"):
        fp = bin_dir + "/" + file
        # get .text offset
        stream = os.popen('echo $(readelf -WS {}'.format(fp) + "| grep '.text ' | awk '{ print $5 }')")
        section_text_str = "0x" + stream.read()
        section_text = int(section_text_str, 16)
        # get load0 segment address
        proc_name = file.rstrip(".")
        stream = os.popen("echo $(pidof " + proc_name + " | awk '{ print $1 }')")
        pid = stream.read().rstrip("\n")
        proc = "/proc/" + pid + "/maps"
        stream = os.popen("echo $(cat " + proc + ")")
        load0_str = "0x" + stream.read().split("-", 1)[0]
        load0 = int(load0_str, 16)
        # combine the two
        address = str(hex(load0 + section_text))
        # add symbols
        gdb.execute("add-symbol-file " + fp + " " + address)
