'''
README information

CMPUT 275, Assignment 1
Part 2

Aaron Espiritu  |  Nicholas Hoskins
1462698         |  1457911
EB2             |  EB1

server.py can be run from the command line.  It loads data representing a map
of Edmonton into a directed graph.  server.py allows a user to choose a start
and end coordinate with the following syntax: "R start_x start_y end_x end_y"
after the script has been run.  The script will then calculate the shortest
possible route from the desired start point to the desired end point, listing
waypoints along the way.  Each waypoint must be acknowledged by the user by
typing 'A' into the command prompt.

In Part 2, the server.py script was modified to allow communication with an Arduino.
The cs_message module that is included allows for server-client communication.
When prompted by the lcd screen, the user can scroll and select a starting and
end point on the Edmonton map. The shortest between the two path will be displayed.

Other files we modified:
	- client.cpp : used the functions implemented in serial_handling to communicate
		with the server. The code to draw the lines on the map are also found here.
		Client timeouts are handled here.

	- serial_handling.cpp : implemented the srv_get_waypoints and srv_get_pathlen
		functions. The serial_readline function was also modified while implementing
		timeouts. Instead of the function waiting in an infinite loop for serial data to be
		available, it will now have a max wait time before exiting the function.


'''
from cs_message import *
import sys
from adjacencygraph import AdjacencyGraph
import heapq
import math
import csv
import time

lat_coord = dict() # will be used to store lat and lon values (integers)
lon_coord = dict()

def least_cost_path(graph, start, dest, cost):
	'''Find and return a least cost path in graph from start
		vertex to dest vertex.

		Efficiency: If E is the number of edges, the run-time is
		O( E log(E) ).

		Args:
			graph (Graph): The digraph defining the edges between the
			vertices.

			start: The vertex where the path starts. It is assumed
			that start is a vertex of graph.

			dest:  The vertex where the path ends. It is assumed
			that start is a vertex of graph.

			cost:  A function, taking the two vertices of an edge as
			parameters and returning the cost of the edge. For its
			interface, see the definition of cost_distance.
		Returns:
			list: A potentially empty list (if no path can be found) of
			the vertices in the graph. If there was a path, the first
			vertex is always start, the last is always dest in the list.
			Any two consecutive vertices correspond to some
			edge in graph.
	'''

	reached = dict()
	runners = [] # heap
	path_list = []

	# the type of runners is a tuple of events of the following format
	# (current distance, (d_arrive , v_fro, v_to))
	# d_arrive is the sum of the current distance plus the cost to the next vertex
	# we start the first runner at the beginning, and the first event
	# will be the starter runner running to 'itself'
	heapq.heappush(runners, (0,start,start))

	#check to see if end path is start path_list
	if start == dest :
		return [start]

	while runners:
		# Find the runner with the smallest d_arrive and pop it from heap

		(d_arrive, v_from, v_to) = heapq.heappop(runners)

		if v_to == dest: pass #do nothing
		elif v_to in reached:
			# a runner made it there at time <= d_arrive
			# do nothing and go to next loop
			continue

		# first to arrive at v_to came from v_from
		# add to dictionary of reached vertices
		# NOTE: the d_arrive for each vertex in the dict corresponds to the smallest distance
		reached[v_to] = (d_arrive, v_from)

		 # now hand off to the waiting runners
		for v_next in AdjacencyGraph.neighbours(graph, v_to):
			# Add a new runner event for each outgoing edge v_to -> v_next
			# provided that v_next has not been reached already.
			if v_next in reached:
				continue

			#push neighbours onto heap if not visited yet
			# It takes time cost(v_to, v_next) to travel the edge, so
			# the arrival time at neighbouring vertex is:
			heapq.heappush(runners,
				(d_arrive + cost(v_to, v_next), v_to, v_next) )

		#check to see if end vertex was reached
		#if so then back track and add vertices to list
		if v_to == dest:
			#add destination to path list
			path_list = [v_to] + path_list
			while True :
				(distance, v_from) = reached[v_to]
				path_list = [v_from] + path_list
				#if back at start then end loop
				if v_from == start: break
				v_to = v_from


			#break out of outer while loop and return list
			break
	return path_list


def read_city_graph(filename) :
	'''This function will read a comma separated value (CSV) file. Each line in the
	   file is specified as a vertex(V) with an ID, or and edge(E) with the two
	   connecting vertices. This function will create an instance of class
	   UndirectedAdjacencyGraph with all the vertices and edges listed in the csv
	   file

	   Args:
			filename(string) : Enter the name of the file to be read. File
							   may be.txt format or .csv. The name of
							   the file must be enclosed in ''.
							   Example: read_city_graph('example.txt')

	   Returns:
			Instance of class UndirectedAdjacencyGraph with all the vertices
			and edges listed in the read file.

	   Note: Blank lines will be skipped and not read.
	'''
	graph_instance = AdjacencyGraph()
	with open(filename) as csvfile:
		reader = csv.reader(csvfile)
		for line in reader:
			# check to see if it is vertex or edge and add to instance accordingly
			# If there is a blank line, conntinue
			if not line : continue
			if line[0] == 'V' :
				graph_instance.add_vertex(line[1])
				# add coordinate values/ assuming it is not already added
				lat_coord[line[1]] = int(float(line[2]) * 100000)
				lon_coord[line[1]] = int(float(line[3]) * 100000)
				continue
			elif line[0] == 'E' :
				graph_instance.add_edge((line[1],line[2]))
				continue
		return graph_instance

def protocol(serial_in, serial_out) :
	# read in graph from csv file. Load vertices, edges and coordinates
	edmonton_graph = read_city_graph('Edmonton_City_Graph.txt')

	#this point marks the beginning of a request. If there is a timeout
	#revert back to this point
	while(True):
		def cost_distance(u, v):
			'''Computes and returns the straight-line distance between the two
			vertices u and v.

			Args:
				u, v: The ids for two vertices that are the start and
				end of a valid edge in the graph.

			Returns:
				numeric value: the distance between the two vertices.
			'''
			x_component = lon_coord[u] - lon_coord[v]
			y_component = lat_coord[u] - lat_coord[v]
			distance = math.sqrt(x_component*x_component
						+ y_component * y_component)
			return distance

		#server will wait until first request from client
		while True:
			user_input = receive_msg_from_client(serial_in)
			log_msg(user_input)
			if user_input[0] == 'R':
				break


		user_input.strip()
		user_args = user_input.split(" ")
		if(user_args[0] != 'R'):
			sys.stdout.write("Invalid input - does not start with 'R'.")
			exit()

		# calculate closest vertex to start and end points
		closest_distance = float('inf')
		start_vertex = 0
		for vertex in lat_coord:
			x_component = lat_coord[vertex] - int(user_args[1])
			y_component = lon_coord[vertex] - int(user_args[2])
			if(math.sqrt(x_component*x_component + y_component*y_component) < closest_distance):
				closest_distance = math.sqrt(x_component*x_component + y_component*y_component)
				start_vertex = vertex

		closest_distance = float('inf')
		end_vertex = 0
		for vertex in lon_coord:
			x_component = lat_coord[vertex] - int(user_args[3])
			y_component = lon_coord[vertex] - int(user_args[4])
			if(math.sqrt(x_component*x_component + y_component*y_component) < closest_distance):
				closest_distance = math.sqrt(x_component*x_component + y_component*y_component)
				end_vertex = vertex

		# print the number of waypoints
		path = least_cost_path(edmonton_graph, start_vertex, end_vertex, cost_distance)
		send_msg_to_client(serial_out,"N {}\n".format(len(path)))

		# check for no path (when "path" is an empty list)
		if not path:
			continue

		# wait until server acknowledges and needs waypoints
		start_waypoint_time = time.time() * 1000
		timeout = 1
		reset = 0
		# timeout after 1 second
		while(time.time() * 1000 - start_waypoint_time < 1000):
			user_input = receive_msg_from_client(serial_in)
			#if client timed out "T" is returned
			if  user_input[0] == "T" :
				reset = 1
			elif user_input[0] == "A":
				timeout = 0
				break

		#if client timed out then revert back to beginning
		if reset : continue


		if(timeout):
			sys.stdout.write("Timed out after 1 second - expected 'A'.")
			continue


		# print number of waypoints in path, wait for acknowledgement
		log_msg(user_input)
		user_input.strip()
		if(user_input[0] != 'A'):
			sys.stdout.write("Invalid acknowledgement - expected 'A'.")
			exit()

		# print each waypoint and get acknowledgement after each one
		for waypoint in path:

			send_msg_to_client(serial_out,'W {} {}'.format(lat_coord[waypoint],
												lon_coord[waypoint]))

			#delay until acknowledgement
			start_waypoint_time = time.time() * 1000
			timeout = 1
			# timeout after 1 second
			while(time.time() * 1000 - start_waypoint_time < 1000):
				user_input = receive_msg_from_client(serial_in)
				user_input.strip()
				if(user_input[0] == 'A'): #acknowledgment received
					timeout = 0
					break

				#client may have timed out
				elif (user_input[0] == 'T') :
					timeout = 1
					break


			if(timeout):
				#sys.stdout.write("Timed out after 1 second - expected 'A'.")
				break

		if(timeout):
			continue

		#request completed, go to beginning and wait for new request
		send_msg_to_client(serial_out,"E")
		continue


def main():

    import argparse
    parser = argparse.ArgumentParser(
        description='Client-server message test.',
        formatter_class=argparse.RawTextHelpFormatter,
        )

    parser.add_argument("-d0",
        help="Debug off",
        action="store_false",
        dest="debug")

    parser.add_argument("-s",
        help="Set serial port for protocol",
        nargs="?",
        type=str,
        dest="serial_port_name",
        default="/dev/ttyACM0")

    args = parser.parse_args()

    debug = args.debug

    set_logging(debug)

    # this imports serial, and provides a useful wrapper around it
    import textserial

    serial_port_name = args.serial_port_name;
    log_msg("Opening serial port: {}".format(serial_port_name))

    # Open up the connection
    baudrate = 9600  # [bit/seconds] 115200 also works

    # Run the server protocol forever

    # The with statment ensures that if things go bad, then ser
    # will still be closed properly.

    with textserial.TextSerial(
        serial_port_name, baudrate, newline=None) as ser:
        protocol(ser, ser)


# main function of the program
if __name__ == "__main__":
	main()
