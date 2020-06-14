# Driving Route Finder

Created by: Charles Ancheta and Poulomi Ganguly

An Arduino/CMPUT 275 VM application that finds the shortest driving route between two locations using Dijkstra's algorithm

## How it Works

The Arduino functions as a client that displays a map of Edmonton and prompts the user for start and end points. The client then sends the coordinates to the Ubuntu virtual machine server and finds the shortest route between the two coordinates. This is done using Dijkstra's Algorithm in a weighted graph of the city roads. The server then sends the waypoints one by one to the client who then prints out the path on the displayed map.

### Limitations

Due to memory constraints, the Arduino can only store up to 500 waypoints. The route may not be displayed if it is a long path with many turns and curves.

[Download Link for CMPUT 274/275 VMWare Ubuntu Virtual Machine](https://drive.google.com/file/d/1D0Kh16pOu5PJLu1d-Ff9V2u6g6mYQT2j/view?usp=sharing)
