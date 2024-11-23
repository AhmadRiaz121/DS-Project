#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Edge
{
public:
    int end;
    double weight;
    Edge *next;
};

class Node
{
public:
    string id;   // Intersection ID (e.g., "A", "B")
    Edge *edges; // Head of the linked list of edges
};

class Graph
{
public:
    Node *nodes; // Dynamic array of nodes
    int nodeCount;
    int maxNodes;
    int infinity;

    Graph(int maxNodes) : nodeCount(0), maxNodes(maxNodes), infinity(1000000)
    {
        nodes = new Node[maxNodes];
        for (int i = 0; i < maxNodes; i++)
        {
            nodes[i].edges = nullptr;
        }
    }

    ~Graph()
    {
        delete[] nodes;
    }

    int getNodeIndex(const string &id)
    {
        for (int i = 0; i < nodeCount; i++)
        {
            if (nodes[i].id == id)
            {
                return i;
            }
        }
        return -1; // Node not found
    }

    void addNode(const string &id)
    {
        if (getNodeIndex(id) == -1)
        {
            nodes[nodeCount].id = id;
            nodes[nodeCount].edges = nullptr;
            nodeCount++;
        }
    }

    void addEdge(const string &start, const string &end, double weight)
    {
        int startIndex = getNodeIndex(start);
        int endIndex = getNodeIndex(end);

        if (startIndex == -1)
        {
            addNode(start);
            startIndex = nodeCount - 1;
        }

        if (endIndex == -1)
        {
            addNode(end);
            endIndex = nodeCount - 1;
        }

        Edge *newEdge = new Edge{endIndex, weight, nodes[startIndex].edges};
        nodes[startIndex].edges = newEdge;
    }

    void dijkstra(const string &startId, const string &endId)
    {
        int startIndex = getNodeIndex(startId);
        int endIndex = getNodeIndex(endId);

        if (startIndex == -1 || endIndex == -1)
        {
            cout << "Invalid start or end intersection!" << endl;
            return;
        }

        int *distances = new int[maxNodes];
        bool *visited = new bool[maxNodes]{false};
        int *previous = new int[maxNodes];

        for (int i = 0; i < maxNodes; i++)
        {
            distances[i] = infinity;
            previous[i] = -1;
        }
        distances[startIndex] = 0;

        for (int i = 0; i < nodeCount; i++)
        {
            int minIndex = -1;

            // Find the unvisited node with the smallest distance
            for (int j = 0; j < nodeCount; j++)
            {
                if (!visited[j] && (minIndex == -1 || distances[j] < distances[minIndex]))
                {
                    minIndex = j;
                }
            }

            if (distances[minIndex] == infinity)
            {
                break;
            }

            visited[minIndex] = true;
            Edge *edge = nodes[minIndex].edges;

            while (edge)
            {
                int neighbor = edge->end;
                double weight = edge->weight;

                if (distances[minIndex] + weight < distances[neighbor])
                {
                    distances[neighbor] = distances[minIndex] + weight;
                    previous[neighbor] = minIndex;
                }

                edge = edge->next;
            }
        }

        // Output the shortest path
        if (distances[endIndex] == infinity)
        {
            cout << "No path exists between " << startId << " and " << endId << "!" << endl;
        }
        else
        {
            cout << "Shortest path from " << startId << " to " << endId << " is " << distances[endIndex] << endl;

            // Trace the path
            int current = endIndex;
            cout << "Path: ";
            while (current != -1)
            {
                cout << nodes[current].id;
                current = previous[current];
                if (current != -1)
                {
                    cout << " <- ";
                }
            }
            cout << endl;
        }

        delete[] distances;
        delete[] visited;
        delete[] previous;
    }

    void displayGraph() const
    {
        for (int i = 0; i < nodeCount; i++)
        {
            cout << "Intersection " << nodes[i].id << ":\n";
            Edge *current = nodes[i].edges;
            while (current)
            {
                cout << "  -> " << nodes[current->end].id
                     << " (Travel Time: " << current->weight << ")\n";
                current = current->next;
            }
        }
    }
};

void loadRoadNetwork(Graph &graph, const string &filename)
{
    ifstream file(filename);
    string line;

    // Skip the header line
    getline(file, line);

    while (getline(file, line))
    {
        char start[10], end[10];
        char weightStr[10];
        int i = 0, j = 0;

        while (line[i] != ',')
        {
            start[j++] = line[i++];
        }
        start[j] = '\0';
        i++;
        j = 0;

        while (line[i] != ',')
        {
            end[j++] = line[i++];
        }
        end[j] = '\0';
        i++;
        j = 0;

        while (i < line.length())
        {
            weightStr[j++] = line[i++];
        }
        weightStr[j] = '\0';

        // Convert weight to double manually
        double weight = 0.0;
        int k = 0;
        bool isFraction = false;
        double fractionFactor = 0.1;

        while (weightStr[k] != '\0')
        {
            if (weightStr[k] == '.')
            {
                isFraction = true;
                k++;
                continue;
            }
            if (!isFraction)
            {
                weight = weight * 10 + (weightStr[k] - '0');
            }
            else
            {
                weight += (weightStr[k] - '0') * fractionFactor;
                fractionFactor *= 0.1;
            }
            k++;
        }

        graph.addEdge(start, end, weight);
    }
}

int main()
{
    Graph graph(100);

    // Sample data
    graph.addEdge("A", "B", 5);
    graph.addEdge("B", "C", 10);
    graph.addEdge("A", "C", 15);
    graph.addEdge("C", "D", 7);
    graph.addEdge("D", "E", 3);
    graph.addEdge("B", "D", 9);
    graph.displayGraph();
    string start, end;
    cout << "Enter start intersection: ";
    cin >> start;
    cout << "Enter end intersection: ";
    cin >> end;

    graph.dijkstra(start, end);

    return 0;
}
