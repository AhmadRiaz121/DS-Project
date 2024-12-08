# DS-Project

Traffic Simulation System
Overview
The Traffic Simulation System incorporates real-time data handling for traffic signals, road closures, vehicle routing, and congestion analysis. It also includes features for rerouting traffic during emergencies and accidents, making it a comprehensive tool for studying urban traffic management.

The simulation supports a dynamic network of roads and vehicles, leveraging algorithms like Dijkstra’s Algorithm, Depth-First Search (DFS), and Priority Queue to solve complex routing and traffic flow problems efficiently.

Features
1. Display City Traffic Network
Visualize the city’s road network graphically by displaying the intersections and connections between them.
This view helps simulate the traffic flow and manage resources in real-time.

2. Display Traffic Signal Status
The system can manage and display the operational status of traffic signals across the city.
Signals can be adjusted dynamically based on the current traffic conditions.


3. Display Congestion Status
Displays the congestion levels of roads based on vehicle density.
Provides an overview of which roads are the most congested, helping city planners manage traffic effectively.


4. Display Blocked Roads
Identify and display the roads that are blocked due to accidents or closures.
Alerts users about potential obstacles to avoid.


5. Emergency Vehicle Routing
This feature uses Dijkstra’s Algorithm to calculate the shortest and safest route for emergency vehicles, taking into account blocked roads and traffic congestion.
The system automatically reroutes emergency vehicles to minimize response time.


6. Block Road due to Accident
Simulate road closures caused by accidents, maintenance, or other incidents.
Mark affected roads and reroute vehicles dynamically to avoid traffic jams.


7. Reroute Traffic
The system dynamically reroutes traffic based on real-time conditions.
It takes into account blocked roads, accidents, and congestion data to find optimal paths.


8. Shortest Path
Implements Dijkstra’s Algorithm to calculate the shortest path between two intersections.
Supports the real-time update of traffic conditions, ensuring that the route remains optimal even if traffic conditions change.


9. Route Vehicles
Allows the system to simulate the routing of multiple vehicles across the network.
Traffic conditions, road closures, and emergency routes are considered to ensure the vehicles follow the most efficient paths.


10. Simulate Vehicle Routing
Simulate all possible routes between two intersections.
This feature provides insights into how vehicles can be routed under various conditions and helps in understanding traffic flow in a given area.


11. Exit
Terminates the simulation and closes the program.


Data Files
1. traffic_signals.csv
Contains data for each traffic signal, including its location and timing information.
This file is used to simulate and manage traffic signals across the network.


2. road_network.csv
Defines the city’s road network with intersections, roads, and their connectivity.
Used to create a graph structure for the road network.


3. road_closures.csv
Contains details about blocked roads due to accidents, construction, or other disruptions.
Used to simulate road closures and reroute traffic dynamically.


4. vehicles.csv
Contains information about vehicles, such as their starting locations, destinations, and current status.
This file helps simulate the routing of vehicles across the network based on traffic conditions.



Algorithms and Data Structures Used
1. Graph Representation
The road network is represented as an undirected weighted graph using an adjacency list.
Intersections are represented as nodes, and roads as edges between these nodes.

![output 1](https://github.com/user-attachments/assets/638db7a7-2589-4dbc-8625-c7a62b6412fb)



2. Dijkstra’s Algorithm
Dijkstra’s Algorithm is employed to find the shortest path between two intersections.
It uses a priority queue (min-heap) to efficiently select the next node with the smallest tentative distance from the source node.


3. Depth-First Search (DFS)
DFS is used to find all possible routes between two intersections, making it useful for simulating vehicle routing in a dynamic environment.
DFS helps explore the entire network to identify alternative paths when one is blocked or congested.


4. Priority Queue (Min-Heap)
A priority queue (implemented using a min-heap) is used to optimize the pathfinding in Dijkstra's Algorithm.
It allows for fast extraction of the node with the smallest distance, which is crucial for the algorithm's efficiency.


5. Dynamic Rerouting
When a road becomes blocked, the system automatically calculates new routes for affected vehicles.
The rerouting algorithm considers traffic congestion and road closures.


6. Traffic Congestion Analysis
The congestion status is calculated based on vehicle density and road capacity.
The system dynamically updates congestion levels to optimize traffic management.
