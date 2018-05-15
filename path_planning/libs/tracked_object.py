import numpy as np

from kalman_filter import KalmanFilter

class TrackedObject:
    def __init__(self):
        self.kf = KalmanFilter()

    def configure_kalman_filter(self, ndim = 1, P = None, Q = None, R = None,
                                F = None, H = None, x = None):
        
        if x is None:
            self.kf.x = np.matrix(np.zeros(2*ndim)).getT()
        else:
            self.kf.x = x
            
        if H is None:
            self.kf.H = np.matrix(np.zeros((ndim, 2*ndim)))
            for i in range(ndim):
                self.kf.H[i, i] = 1
        else:
            self.kf.H = H

        if P is None:
            self.kf.P = np.identity(2*ndim)
        else:
            self.kf.P = P

        if Q is None:
            self.kf.Q = np.identity(2*ndim)
        else:
            self.kf.Q = Q

        if R is None:
            self.kf.R = np.identity(ndim)
        else:
            self.kf.R = R

        if F is None:
            self.kf.F = np.matrix(np.identity(2*ndim))
        else:
            self.kf.F = F