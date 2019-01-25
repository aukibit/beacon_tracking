#!/usr/bin/env python
import urllib2, numpy
from libs import beacon, blufinode, packetrcv, circle_intersection

import matplotlib.pyplot as plt
import matplotlib.animation as animation

def node_scan(beacons, nodes):
    for node in nodes:
        res = urllib2.urlopen("http://" + node.ip).read().split("<br />");
        mac = res[0].split(" ")[-1]
        res_bcns = res[1:]

        for res_bcn in res_bcns:
            if not res_bcn:
                continue

            res_bcn = res_bcn.split(" - ")
            major = res_bcn[0].split(" ")[-1]
            minor = res_bcn[1].split(" ")[-1]
            rssi = res_bcn[2].split(" ")[-1]
            timestamp = res_bcn[3].split(" ")[-1]

            bcn_index = -1
            for i in range(len(beacons)):
                if ((beacons[i].major == major) and (beacons[i].minor == minor)):
                    bcn_index = i
            if bcn_index == -1:
                # only update beacons that are ours and that we care about
                continue
                # beacons.append(beacon.Beacon(major, minor))
            beacons[bcn_index].new_rcv(packetrcv.PacketRcv(node, timestamp, rssi))

def find_closest_node(bcn):
    closest_node = bcn._rcvs[0]
    if len(bcn._rcvs) < 1:
        return closest_node

    for rcv in bcn._rcvs:
        if rcv.distance < closest_node.distance:
            closest_node = rcv

    return closest_node

def animate(i, nodes, beacons, node_colours):
    node_scan(beacons, nodes)

    ax.clear()

    # Draw the map
    ax.hlines([-3, 3], [-3, 3], [15, 15], colors='lightgray')
    ax.vlines([-3, 3], [-3, 3], [15, 15], colors='lightgray')
    # Draw the nodes
    for i in range(len(nodes)):
        circle = plt.Circle((nodes[i].xpos, nodes[i].ypos), 2, color=node_colours[i])
        ax.add_artist(circle)
        plt.text(nodes[i].xpos, nodes[i].ypos+2, nodes[i].ip, size = 10, ha="center", va="center",
            bbox=dict(boxstyle="round", ec=(1., 0.5, 0.5), fc=(1., 0.8, 0.8)))


    # Draw the beacons
    for bcn in beacons:
        if len(bcn._rcvs) < 1:
            continue
        closest_node = find_closest_node(bcn)
        circle = plt.Circle((closest_node.node.xpos, closest_node.node.ypos), 0.5, color='teal')
        ax.add_artist(circle)
        plt.text(closest_node.node.xpos, closest_node.node.ypos-2, 
            "Major: " + bcn.major + "\nMinor: " + bcn.minor, size = 10, ha="center", va="center",
            bbox=dict(boxstyle="round", ec=(1., 0.5, 0.5), fc=(1., 0.8, 0.8)))

    # Set up the axes
    ax.set_xlim((-15, 15))
    ax.set_ylim((-15, 15))
    ax.set_aspect('equal', 'box')
    ax.set_xlabel("X-Coordinate")
    ax.set_ylabel("Y-Coordinate")
    ax.set_title("Closest Node to Beacon")


if __name__ == "__main__":
    # intialise known node data
    nodes = [blufinode.BlufiNode("192.168.1.69", "24:0a:c4:13:92:cc", 0, 5)]
    nodes += [blufinode.BlufiNode("192.168.1.70", "30:ae:a4:0a:c0:80", 10, 0)]

    beacons = [beacon.Beacon("1","3"), beacon.Beacon("1","1"), beacon.Beacon("1","2")]

    node_colours = ["mediumseagreen", "darkorange", "darkturquoise"]

    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ani = animation.FuncAnimation(fig, animate, fargs=(nodes, beacons, node_colours), interval = 1)
    plt.show()