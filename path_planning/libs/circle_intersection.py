import math
import matplotlib.pyplot as plt

class Position:
    def __init__(self, x, y, r):
        self.x = x
        self.y = y
        self.r = r
        
    def __repr__(self):
        return "x: " + str(self.x) + ", y: " + str(self.y) + ", r: " + str(self.r)
    
    def circle(self, colorstr='b', fillbool=False):
        return plt.Circle((self.x, self.y), self.r, color=colorstr, fill=fillbool)
    
def intersectTwoCircles(pos1, pos2):
    # assuming that the circles have two intersection points...
    bigR = math.sqrt((pos1.x-pos2.x)**2 + (pos1.y-pos2.y)**2)
    a = (pos1.r**2-pos2.r**2)/(2*(bigR**2))
    sqrt_arg = (2 * (pos1.r**2 + pos2.r**2)) / (bigR**2) - ((pos1.r**2 - pos2.r**2)**2)/(bigR**4) - 1
    if sqrt_arg <= 0:
        return 0,0,0,0
    c = math.sqrt(sqrt_arg)
    
    fx = (pos1.x + pos2.x) / 2 + a * (pos2.x - pos1.x)
    gx = c * (pos2.y - pos1.y) / 2
    ix1 = fx + gx
    ix2 = fx - gx
    
    fy = (pos1.y + pos2.y) / 2 + a * (pos2.y - pos1.y)
    gy = c * (pos1.x - pos2.x) / 2
    iy1 = fy + gy
    iy2 = fy - gy
    
    return ix1, ix2, iy1, iy2