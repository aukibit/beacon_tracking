class PacketRcv:
    """ A class containing information on a packet read event. """
    def __init__(self, node, timestamp, rssi):
        self.node = node
        self.timestamp = timestamp
        self.rssi = rssi

    def __repr__(self):
        return self.node.__repr__() + ", RSSI: " + self.rssi + ", Timestamp: " + self.timestamp
