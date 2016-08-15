import sys

def readLine(fileObj):
    """Read a line from the file object and skips empty lines"""
    line = '\n'
    while line == '\n':
        line = fileObj.readline()
    return line

try:
    if len(sys.argv) != 2:
        raise Exception('Please provide a edge list file to be converted to adjacency list!\n' +
                        'USAGE: {0} SOURCE'.format(sys.argv[0]))

    sourceFilePath = sys.argv[1]
    destFilePath = sourceFilePath + '.converted'

    try:
        sourceFileObj = open(sourceFilePath)
    except OSError as err:
        raise Exception('Could not open "{0}" for reading: {1}'.format(sourceFilePath, err))

    try:
        destFileObj = open(destFilePath, 'w')
    except OSError as err:
        raise Exception('Could not open "{0}" for writing: {1}'.format(destFilePath, err))

    line = readLine(sourceFileObj)
    tokens = line.split()
    nvertices = int(tokens[0])
    nedges = -1
    if len(tokens) > 1:
        nedges = int(tokens[1])

    edges = {}

    while True:
        line = readLine(sourceFileObj)
        if line == '':
            break;

        vertices = line.split()
        u = int(vertices[0]) - 1
        v = int(vertices[1]) - 1
        cost = int(vertices[2])

        if u in edges:
            edges[u].append((v, cost))
        else:
            edges[u] = [(v, cost)]

    keys = edges.keys()

#    if int(nvertices) != len(keys):
#        raise Exception('nvertices:{0} != len(keys):{1}'.format(nvertices, len(keys)))

    line = str(nvertices)
    if nedges > -1:
        line += '\t' + str(nedges)
    line += '\n\n'
    destFileObj.write(line)
    #print(line, '\n')

    for vertex in sorted(keys):
        line = str(vertex)
        line += '\t'
        line += '\t'.join([str(v) + ',' + str(c) for (v, c) in edges[vertex]])
        line += '\n'
        destFileObj.write(line)
        #print(line)

except Exception as e:
    print(e)
finally:
    try:
        sourceFileObj.close()
        destFileObj.close()
    except NameError:
        pass
