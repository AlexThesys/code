import sys
import r2pipe

if len(sys.argv) < 2 :
    print("Provide file path and optional block size!")
    sys.exit()

visual = [" ", "+", "++", "+++", "++++", "+++++", "++++++",\
        "+++++++", "++++++++", "+++++++++", "++++++++++"]

r2 = r2pipe.open(sys.argv[1])
size = int(r2.cmd('iZ'))

block_size = int(sys.argv[2], 16) if (len(sys.argv) > 2) else 0x0
    
if (block_size > size) or (block_size == 0x0) :
    block_size = size

num_blocks = size // block_size

r2.cmd('b '+ str(block_size))
for _ in range(num_blocks) :
    ent = r2.cmd('ph entropy')
    print(r2.cmd('s').replace("\n", ""))
    print(visual[int(float(ent))])
    print(ent)
    r2.cmd('s++')

