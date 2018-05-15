class PacketRcv:
    """ A class containing information on a packet read event. """
    def __init__(self, node, timestamp, rssi):
        self.node = node
        self.timestamp = timestamp
        self.rssi = rssi
        self.distance = self.calculate_distance()

    def __repr__(self):
        return self.node.__repr__() + ", RSSI: " + self.rssi + \
               str(", Distance: %.2f" % self.distance) + "m, Timestamp: " + \
               self.timestamp

    def calculate_distance(self):
        # https://stackoverflow.com/a/20434019
        if self.rssi < 0:
            return -1

        tx_power = -59 #dbm, defined in the beacon's firmware
        ratio = float(self.rssi) / tx_power

        if ratio < 1:
            return ratio ** 10

        return (0.89976)*(ratio**7.7095) + 0.111;    