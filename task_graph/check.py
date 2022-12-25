import sys

def read_file(filename) -> set:
    res = set()
    with open(filename, 'r') as fileData:
        for line in fileData:
            edge = tuple(map(int, line.split()))
            node1, node2 = min(edge[0], edge[1]), max(edge[0], edge[1])
            res.add((node1, node2, edge[2]))
    return res


if __name__ == "__main__":
    vals1 = read_file(sys.argv[1])
    vals2 = read_file(sys.argv[2])
    print("Files are equal:", vals1 == vals2, "\nDifference is:", vals1.symmetric_difference(vals2))