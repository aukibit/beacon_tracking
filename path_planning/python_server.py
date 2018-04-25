#!/usr/bin/env python

import urllib2

class BlufiNode:
    """ A class describing an Aukibit BluFi node """
    def __init__(self, ip, mac):
        self.ip = ip
        self.mac = mac

    def __repr__(self):
        return "MAC: " + self.mac + ", IP: " + self.ip

class PacketRcv:
    """ A class containing information on a packet read event. """
    def __init__(self, node, timestamp, rssi):
        self.node = node
        self.timestamp = timestamp
        self.rssi = rssi

    def __repr__(self):
        return self.node.__repr__() + ", RSSI: " + self.rssi + ", Timestamp: " + self.timestamp

class Beacon:
    """ A class describing any Bluetooth beacon. """    
    def __init__(self, major, minor):
        self.major = major
        self.minor = minor
    
    def __repr__(self):
        return "Major: " + self.major + ", Minor: " + self.minor + ", Receives ->\n\r" + self.read_rcvs()

    def new_rcv(self, packet):
        self._rcvs.append(packet)

    def read_rcvs(self):
        ret = ""
        for rcv in self._rcvs:
            ret += rcv.__repr__() + "\n\r"
        return ret

    _rcvs = []

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
                beacons.append(Beacon(major, minor))
            beacons[-1].new_rcv(PacketRcv(node, timestamp, rssi))


if __name__ == "__main__":
    beacons = []
    nodes = [BlufiNode("192.168.0.14", "24:a:c4:13:92:cc")]

    node_scan(beacons, nodes)

    for beacon in beacons:
        print beacon