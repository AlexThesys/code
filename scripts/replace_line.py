import os, sys

if len(sys.argv) < 2:
    raise Exception("Please provide the path to the directory containing .*** files!")

attn_dir = sys.argv[1]

for file in os.listdir(attn_dir):
    if file.endswith(".lua"):
        fp = attn_dir + "/" + file
        f = open(fp, 'r+')
        contents = f.readlines()
        old = "old_line,"
        new = "new_line"
        new_text = ""
        for line in contents:
            new_line = line.replace(old, new)
            new_text += new_line
        f.seek(0)
        f.writelines(new_text)
