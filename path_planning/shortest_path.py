import numpy as np

class Node:
    def __init__(self, label = "", position = (0, 0)):
        self.label = label
        self.position = position

    def __repr__(self):
        return self.label + str(self.position)

def path_length(path = []):
    dist = 0

    for i in range(len(path)-1):
        dist += np.linalg.norm( np.array(path[i].position) - 
                                np.array(path[i+1].position))
    
    return dist

def find_shortest_path(graph, start, end, path=[]):
    path = path + [start]
    if start == end:
        return path
    if not start in graph:
        return None
    shortest = None
    for node in graph[start]:
        if node not in path:
            newpath = find_shortest_path(graph, node, end, path)
            if newpath:
                if not shortest or path_length(newpath) < path_length(shortest):
                    shortest = newpath
    return shortest