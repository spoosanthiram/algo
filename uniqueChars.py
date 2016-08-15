import sys

try:
    if len(sys.argv) != 2:
        raise Exception("Need a text file input!")

    sourceFilePath = sys.argv[1]

    try:
        sourceFileObj = open(sourceFilePath)
    except OSError as err:
        raise Exception('Could not open "{0}" for reading: {1}'.format(sourceFilePath, err))

    charDict = {}

    for line in sourceFileObj:
        for c in line:
            if c in charDict:
                charDict[c] = charDict[c] + 1
            else:
                charDict[c] = 1

    print("unique chars: {}".format(len(charDict)))

    for c, v in sorted(charDict.items(), key=lambda x: x[1]):
        if c == "\n":
            c = "nl"
        elif c == "\t":
            c = "tab"
        elif c == " ":
            c = "sp"

        print("{0:<3} --> {1:>7}".format(c, v))

except Exception as e:
    print(e)
finally:
    try:
        sourceFileObj.close()
    except NameError:
        pass
