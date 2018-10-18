class BlufiNode:
    """ A class describing an Aukibit BluFi node """
    def __init__(self, ip, mac, xpos, ypos):
        self.ip = ip
        self.mac = mac
        self.xpos = xpos
        self.ypos = ypos

    def __repr__(self):
        return "MAC: " + self.mac + ", IP: " + self.ip