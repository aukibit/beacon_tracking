#!/usr/bin/env python
import urllib2
from libs import beacon, blufinode, packetrcv, circle_intersection

import matplotlib.pyplot as plt

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
                beacons.append(beacon.Beacon(major, minor))
            beacons[bcn_index].new_rcv(packetrcv.PacketRcv(node, timestamp, rssi))

if __name__ == "__main__":
    plt.ion()
    fig = plt.figure()
    ax = fig.add_subplot(111)
    plt.xlabel ("X-Distance (cm)")
    plt.ylabel ("Y-Distance (cm)")
    plt.title ("Estimated Beacon Position with Known Node Positions (2)")


    while (True):
        beacons = []
        
        # intialise known node data
        nodes = [blufinode.BlufiNode("192.168.0.69", "24:a:c4:13:92:cc", 55, 5)]
        nodes += [blufinode.BlufiNode("192.168.0.70", "24:a:c4:13:92:cc", 0, 0)]

        # get the beacon data from the known nodes
        node_scan(beacons, nodes)

        if len(beacons) == 0:
            continue

        for bcn in beacons:
            if len(bcn._rcvs) > 1:
                pos0 = circle_intersection.Position(bcn._rcvs[0].node.xpos, bcn._rcvs[0].node.ypos, bcn._rcvs[0].distance)
                pos1 = circle_intersection.Position(bcn._rcvs[1].node.xpos, bcn._rcvs[1].node.ypos, bcn._rcvs[1].distance)
                x1, x2, y1, y2 = circle_intersection.intersectTwoCircles(pos0, pos1)
                if x1 == x2 == y1 == y2 == 0:
                    x1, x2, y1, y2 = circle_intersection.intersectTwoCircles(pos1, pos0)
                    if x1 == x2 == y1 == y2 == 0:
                        continue

                ax.add_artist(pos0.circle(colorstr='b'))
                ax.add_artist(pos1.circle(colorstr='r'))
                ax.plot([x1, x2], [y1, y2], 'ko')
                
                ax.set_xlim((-100, 100))
                ax.set_ylim((-100, 100))
                ax.set_aspect(1)

                fig.canvas.flush_events()

