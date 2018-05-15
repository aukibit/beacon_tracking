#!/usr/bin/env python
import urllib2
from libs import beacon, blufinode, packetrcv

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
                    if bcn_index > -1:
                        print "ERROR: BEACON ALREADY EXISTS"
                    bcn_index = i
            if bcn_index == -1:
                beacons.append(beacon.Beacon(major, minor))
            beacons[-1].new_rcv(packetrcv.PacketRcv(node, timestamp, rssi))


if __name__ == "__main__":
    beacons = []
    nodes = [blufinode.BlufiNode("192.168.0.12", "24:a:c4:13:92:cc")]

    node_scan(beacons, nodes)

    for beacon in beacons:
        print beacon