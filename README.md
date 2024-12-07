# DS-Project

Traffic Simulation System
Overview
The Traffic Simulation System is an interactive C++ program designed to simulate and manage a city's traffic network. It provides functionalities such as visualizing the road network, managing traffic signals, rerouting traffic, finding shortest paths, and handling road closures or emergencies. The project leverages advanced algorithms and data structures for efficient simulation and routing.

Features
Display City Traffic Network
Visualize the city’s road network in a graph structure.

Display Traffic Signal Status
Simulate and display the current operational status of traffic signals.

Display Congestion Status
Analyze and display congestion levels across different roads in the network.

Display Blocked Roads
Identify and display roads that are blocked due to accidents or closures.

Emergency Vehicle Routing
Calculate the shortest and safest route for emergency vehicles between intersections using advanced routing algorithms.

Block Road due to Accident
Simulate road closures caused by accidents or maintenance work.

Reroute Traffic
Reconfigure traffic flows to avoid congested or blocked areas dynamically.

Shortest Path
Compute the shortest path between two intersections using Dijkstra's algorithm.

Route Vehicles
Simulate and route vehicles through the network while considering congestion and road conditions.

Simulate Vehicle Routing
Identify all possible routes between two intersections for comprehensive analysis.

Exit
Terminate the simulation program.

File Structure
traffic_signals.csv
Contains configuration data for traffic signals, including timing and location.

road_network.csv
Defines the road network, including intersections, roads, and connectivity.

road_closures.csv
Stores data about roads that are blocked due to accidents or maintenance.

vehicles.csv
Contains details about vehicles, such as their starting points, destinations, and routing requirements.

Algorithms and Data Structures Used
Graph Representation

Adjacency lists are used to represent the road network efficiently.
Dijkstra's Algorithm

Computes the shortest path between intersections.
Utilizes a priority queue (min-heap) for optimal performance.
Depth-First Search (DFS)

Finds all possible routes between two intersections.
Priority Queue

Used in Dijkstra's algorithm to prioritize nodes with the shortest tentative distance.
Dynamic Rerouting

Identifies alternative routes when roads are blocked or traffic conditions change.
Traffic Congestion Analysis

Aggregates data on vehicle density and road capacity to evaluate congestion levels.
