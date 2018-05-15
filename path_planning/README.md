# Beacon Tracking

## Introduction
This aim of this project is to be able to track a beacon in a known structured
environment using multiple sensor nodes, identify beginning and end points, then
suggest an optimal path if a path was taken that was not optimal.

For an introduction to the technical aspects of how
the project works, the Jupyter notebook are the best way to see the performance
and use cases for the code here.

In order of least to most abstract:

The [Kalman Filter class](https://github.com/aukibit/beacon_tracking/blob/master/kalmanfilter_class_example.ipynb) 
does what it says on the box. It's an extremely rudimentary implementation of
the linear Kalman Filter and requires prior knowledge of Kalman filtering.

The [Tracked Object class](https://github.com/aukibit/beacon_tracking/blob/master/trackedobject_class_example.ipynb)
requires hard coding a lot of the Kalman Filter class behaviour but leaves some
room to add extra filtering or internal initialisation capability where 
required. It's made obvious in the [Path Tracking simulation](https://github.com/aukibit/beacon_tracking/blob/master/simulation_path_tracking.ipynb)
that this class can be used (with technical Kalman Filtering knowledge) to
accurately track 2D object movement, even on non-linear paths.

With a known path, it is then possible to use the [Shortest Path](https://github.com/aukibit/beacon_tracking/blob/master/shortest_path.ipynb)
class to determine if there is a better path available.