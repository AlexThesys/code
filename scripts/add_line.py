import os, sys

if len(sys.argv) < 2:
    raise Exception("Please provide the path to the directory containing .*** files!")

attn_dir = sys.argv[1]

for file in os.listdir(attn_dir):
    if file.endswith(".lua"):
        fp = attn_dir + "/" + file
        f = open(fp, 'r+')
        contents = f.readlines()
        match = "some_text"
        for index, line in enumerate(contents):
            if match in line:
                contents.insert(index+1, "some_new_text\n")
                break
        f.seek(0)
        f.writelines(contents)
