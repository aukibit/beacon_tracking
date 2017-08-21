import numpy as np

class KalmanFilter:

    def __init__(self, initialise_to_trivial_case = False):
        ''' Constructor for generic Kalman Filter class.
        Initialises the Kalman Filter parameters to a trivial case for
        demonstration purposes. 

        For an example on how to set up this class, simply follow the
        init_trivial_default function. An example of running using this class
        for the trivial case is included in the Jupyter notebook that ought
        to be stored in the same directory as this class.
        '''
        if initialise_to_trivial_case:
            self.init_trivial_default()
        else:
            # Everything needs to be initialised to something.
            self.x = np.matrix([[0]])
            self.F = np.matrix([[0]])
            self.B = np.matrix([[0]])
            self.Q = np.matrix([[0]])
            self.P = np.matrix([[0]])
            self.R = np.matrix([[0]])
            self.H = np.matrix([[0]])

    def init_trivial_default(self):
        sigma_1 = 20
        sigma_2 = 5
        self.x = np.matrix([[0], [1]])
        self.F = np.matrix([[1, 1], [0, 1]])
        self.B = np.matrix([[1], [1]])
        self.Q = np.matrix([[sigma_1**2, 0], [0, sigma_2**2]])
        self.P = self.Q
        self.R = np.matrix([[sigma_1**4]])
        self.H = np.matrix([1, 0])

    def update(self, u, z):
        # A priori step

        self.x = self.F * self.x + self.B * u
        self.P = self.F * self.P * self.F.getT() + self.Q
        # A posteriori step
        y = z - self.H * self.x
        S = self.R + self.H * self.P * self.H.getT()
        K = self.P * self.H.getT() * S.getI()
        self.x = self.x + K*y
        self.P = (np.identity(self.P.shape[0]) - (K * self.H)) * self.P
        return self.x