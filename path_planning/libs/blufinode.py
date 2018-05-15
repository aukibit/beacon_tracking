class BlufiNode:
    """ A class describing an Aukibit BluFi node """
    def __init__(self, ip, mac):
        self.ip = ip
        self.mac = mac

    def __repr__(self):
        return "MAC: " + self.mac + ", IP: " + self.ip