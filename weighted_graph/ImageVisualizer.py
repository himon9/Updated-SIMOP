import networkx as nx
import matplotlib.pyplot as plt
import random

fileCounter = int(input("Enter the fileCounter "))

for i in range(fileCounter):#for(int i=0;i<fileCounter;i++)

    filename = './DataSet/data%d.txt' % (i+1)

    g=nx.null_graph(create_using=nx.DiGraph())
    g = nx.read_edgelist(filename, create_using=nx.DiGraph(), nodetype=int)
    pos = nx.kamada_kawai_layout(g)
    arc_rad=0.15
    # possible layouts: planar,circular,kamada_kawai,random,spring,shell,spectral,spiral
    nx.draw_networkx_nodes(g, pos, node_size=500)
    nx.draw_networkx_edges(g, pos, edgelist=g.edges(), edge_color='Black',connectionstyle=f'arc3, rad = {arc_rad}')
    nx.draw_networkx_labels(g, pos)

    #plt.figure(figsize=(7,7)) 
    plt.savefig("./ImageSet/Graph%d.png" % (i+1), format="PNG")
    plt.close()
    
