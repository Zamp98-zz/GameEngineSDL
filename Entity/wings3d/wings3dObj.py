import pathlib
import glob
path = pathlib.Path().resolve()
files = []


def parseFiles():
    for file in files:
        buffer=[]
        f = open(file,'r+')
        data = f.readlines()
        for line in data:
            if(line[0] == 'f'):
                temp = line.replace("//","/1/")
                buffer.append(temp)
            else:
                buffer.append(line)
        f.close()
        name = "parsed_"+file
        f = open(name, 'w')
        f.writelines(buffer)

for file in glob.glob("*.obj"):
    files.append(file)
print(path, files)
parseFiles()
