#!/usr/bin/env python
import urllib2
from libs import beacon, blufinode, packetrcv

def node_scan(beacons, nodes):
    for node in nodes:
        res = urllib2.urlopen("http://" + node.ip).read().split("<br />");
        mac = res[0].split(" ")[-1]
        res_bcns = res[1:]
        print res

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
    while (True):
        beacons = []
        
        # intialise known node data
        nodes = [blufinode.BlufiNode("192.168.0.18", "24:a:c4:13:92:cc", 10, 12)]

        # get the beacon data from the known nodes
        node_scan(beacons, nodes)

        # debug, more or less
        print "~*~*~*~ RAW BEACON DATA ~*~*~*~"
        for beacon in beacons:
            print beacon

        # given known node positions, give positions of beacons