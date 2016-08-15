import sys

def readLine(fileObj):
    """Read a line from the file object and skips empty lines"""
    line = "\n"
    while line == "\n":
        line = fileObj.readline()
    return line

try:
    if len(sys.argv) != 2:
        raise Exception("Please provide a adjacency list file to be converted!\n" +
                        "USAGE: {0} SOURCE".format(sys.argv[0]))

    sourceFilePath = sys.argv[1]
    destFilePath = sourceFilePath + ".converted"

    try:
        sourceFileObj = open(sourceFilePath)
    except OSError as err:
        raise Exception("Could not open '{0}' for reading: {1}".format(sourceFilePath, err))

    try:
        destFileObj = open(destFilePath, "w")
    except OSError as err:
        raise Exception("Could not open '{0}' for writing: {1}".format(destFilePath, err))

    # read number of vertices and edges
    nvertices = 0
    nedges = 0
    line = readLine(sourceFileObj)
    tokens = line.split()
    nvertices = tokens[0]
    if len(tokens) > 1:
        nedges = tokens[1]

    print("nvertices: {0}, nedges: {1}".format(nvertices, nedges))

    adjListDict = {} # like {vertex: [(vertex, cost), ...]

    while True:
        line = readLine(sourceFileObj)
        if line == "":
            break;

        tokens = line.split()
        #u = int(tokens[0]) - 1
        u = int(tokens[0])
        del tokens[0]
        for token in tokens:
            if token.find(",") == -1:
                v = token
                cost = None
            else:
                (v, cost) = token.split(",")

            #v = int(v) - 1
            v = int(v)

            vuEdge = (u, cost)
            uvEdge = (v, cost)

            if v in adjListDict and vuEdge in adjListDict[v]:
                print("({0}, {1}):{2} is available as ({1}, {0}):{2}".format(u, v, cost))
                continue
            else:
                if u in adjListDict:
                    adjListDict[u].append(uvEdge)
                else:
                    adjListDict[u] = [uvEdge]

    keys = adjListDict.keys()

    line = str(nvertices)
    if int(nedges) > 0:
        line += "\t" + str(nedges)
    line += "\n\n"
    destFileObj.write(line)
    #print(line, "\n")

    for vertex in sorted(keys):
        edgeList = []
        for (v, cost) in adjListDict[vertex]:
            if cost is None:
                edgeList.append(str(v))
            else:
                edgeList.append(str(v) + "," + str(cost))
        line = str(vertex)
        line += "\t"
        line += "\t".join(edgeList)
        line += "\n"
        destFileObj.write(line)
        #print(line)

except Exception as e:
    print(e)
finally:
    sourceFileObj.close()
    destFileObj.close()
