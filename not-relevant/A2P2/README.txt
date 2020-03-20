'''CMPUT 275, Assignment 1
Part 2

Aaron Espiritu  |  Nicholas Hoskins
1462698         |  1457911
EB2             |  EB1

server.py can be run from the command line.  It loads data representing a map
of Edmonton into a directed graph, and waits for a connection with a client.
When loaded onto an Arduino, client.cpp displays a map of Edmonton on an
Adafruit display.  The map can be scrolled and zoomed with a joystick, which
is also on the Arduino.  Selecting a start and end point on the map
sends a request to the server to find a route from the start to the end point.
The server determines the shortest route between points, and sends each waypoint
back to the client.  The client then displays the shortest route on the map,
by drawing lines along the shortest path.

Another path can be selected after a path is being displayed.  Additionally,
the user can scroll around the map, and the shortest path will still be
displayed.
'''
