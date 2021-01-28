import sys
import os
import random
import string


def get_random_string(length):
    letters = string.ascii_lowercase
    result_str = ''.join(random.choice(letters) for i in range(length))
    return result_str

def copyFileToAnother(finput,foutput):
    with open(finput, "r") as f:
        with open(foutput, "w") as f1:
            for line in f:
                f1.write(line)
    os.chmod(foutput,0o775)
    foutput.close()
    finput.close()


def main(argv):
    if len(argv) != 1:
        print("usage: python {0}".format(argv[0]))
        sys.exit(1)

    newFilename = get_random_string(8)+".py"
    copyFileToAnother(argv[0],newFilename)

if __name__ == "__main__":
    main(sys.argv)