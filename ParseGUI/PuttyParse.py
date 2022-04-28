import os
fin = open("U:\Documents\putty.txt", "r")
fout = open("U:\Documents\parsePutty.txt", "w")

lines = fin.readlines()
for num, line in enumerate(reversed(lines)):
    if(line.startswith('Obj')):
        Cnum = num
        while Cnum >= 0:
            fout.write(lines[len(lines)-Cnum])
            Cnum-=1

fin.close()
fout.close()

os.system("C:\Program Files\MATLAB\R2021a\bin\matlab.exe" -r "run('U:\Documents\GUI.m'); exit;")