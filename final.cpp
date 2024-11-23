#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
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
    string id;
    Edge *edges;
    Node()
    {
        edges=NULL;
    }
};

class Graph
{
public:
    Node *nodes;
    int nodeCount;
    int maxNodes;
    int infinity;

    Graph(int maxNodes)
    {
        this->maxNodes=maxNodes;
        nodeCount=0;
        infinity=1000000;
        nodes=new Node[maxNodes];
    }

    ~Graph()
    {
        delete[] nodes;
    }

    int getNodesIndex(const string &id)
    {
        for (int i=0;i<nodeCount;i++)
        {
            if (nodes[i].id==id)
            {
                return i;
            }
        }
        return -1;
    }

    void addNode(const string &id)
    {
        if (getNodesIndex(id)==-1)
        {
            nodes[nodeCount].id=id;
            nodes[nodeCount].edges=NULL;
            nodeCount++;
        }
    }

    void addEdge(const string &start, const string &end, double weight)
    {
        int startIndex=getNodesIndex(start);
        int endIndex=getNodesIndex(end);

        if (startIndex==-1)
        {
            addNode(start);
            startIndex=nodeCount-1;
        }
        if (endIndex==-1)
        {
            addNode(end);
            endIndex=nodeCount-1;
        }

        Edge *newEdge=new Edge{endIndex, weight, nodes[startIndex].edges};
        nodes[startIndex].edges=newEdge;
    }

    void displayGraph()
    {
        for (int i=0;i<nodeCount;i++)
        {
            cout<<"Intersection "<<nodes[i].id<<":\n";
            Edge *current=nodes[i].edges;
            while (current)
            {
                cout<<"  -> "<<nodes[current->end].id<<" (Travel Time: "<<current->weight<<")\n";
                current=current->next;
            }
        }
    }

    void loadRoadNetwork(Graph &graph, const string &filename)
    {
        ifstream file(filename);
        string line;

        // Skip the header line
        getline(file, line);

        while (getline(file, line))
        {
            string start, end;
            double weight;
            stringstream ss(line);

            // Read the start intersection, end intersection, and weight
            getline(ss, start, ',');
            getline(ss, end, ',');
            ss>>weight;

            graph.addEdge(start, end, weight);
        }

        file.close();
    }
};

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

    return 0;
}
