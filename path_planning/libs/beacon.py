class Beacon:
    """ A class describing any Bluetooth beacon. """    
    def __init__(self, major, minor):
        self.major = major
        self.minor = minor
        self._rcvs = []

    def __repr__(self):
        return "Major: " + self.major + ", Minor: " + self.minor + ", Receives ->\n\r" + self.read_rcvs()

    def new_rcv(self, packet):
        for i in range(len(self._rcvs)):
            if packet.node.mac == self._rcvs[i].node.mac:
                self._rcvs[i] = packet
                return
        self._rcvs.append(packet)

    def read_rcvs(self):
        ret = ""
        for rcv in self._rcvs:
            ret += rcv.__repr__() + "\n\r"
        return ret