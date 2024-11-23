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

    void loadRoadNetwork(const string &filename)
    {
        ifstream file(filename);
        string line;

        // Skip the header line
        getline(file, line);

        while (getline(file, line))
        {
            string start, end, weightStr;
            double weight;
            int i = 0, j = 0;

            // Extract start intersection
            while (line[i] != ',')
            {
                start += line[i++];
            }
            i++;

            // Extract end intersection
            while (line[i] != ',')
            {
                end += line[i++];
            }
            i++;

            // Extract weight
            while (i < line.length())
            {
                weightStr += line[i++];
            }

            // Convert weight to double
            weight = stod(weightStr);
            
            addEdge(start, end, weight);
        }

        file.close();
    }

};

int main()
{
    Graph graph(100);

    
    graph.loadRoadNetwork("road_network.csv");
    graph.displayGraph();

    return 0;
}
