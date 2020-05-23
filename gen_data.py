import random
import sys




LINES = int(sys.argv[2])

with open(sys.argv[1], 'w') as f:
    linebuf = []
    for i in range(LINES):
        r = int(random.random() * 10000000000)
        for j in range(100):
            num = r+j
            linebuf.append(f"this is in on line ({str(i)}, {str(j)}): {str(num)} printed on line ({str(i)}, {str(j)})\n")
        f.writelines(linebuf)
        linebuf = []
     



    
