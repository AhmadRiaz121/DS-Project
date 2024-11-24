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
            int i=0, j=0;

            // Extract start intersection
            while (line[i]!=',')
            {
                start+=line[i++];
            }
            i++;

            // Extract end intersection
            while (line[i]!=',')
            {
                end+=line[i++];
            }
            i++;

            // Extract weight
            while (i<line.length())
            {
                weightStr+=line[i++];
            }

            // Convert weight to double
            weight=stod(weightStr);

            addEdge(start, end, weight);
        }

        file.close();
    }

    void dijkstra(const string &start, const string &end)
    {
        int startIndex=getNodesIndex(start);
        int endIndex=getNodesIndex(end);

        if (startIndex==-1 || endIndex==-1)
        {
            cout<<"Invalid start or end intersection.\n";
            return;
        }

        double *distances=new double[nodeCount];
        bool *visited=new bool[nodeCount];
        int *previous=new int[nodeCount];

        for (int i=0; i<nodeCount;i++)
        {
            distances[i]=infinity;
            visited[i]=false;
            previous[i]=-1;
        }

        distances[startIndex]=0;

        while (true)
        {
            int minIndex=-1;
            double minDistance=infinity;

            for (int i=0; i<nodeCount;i++)
            {
                if (!visited[i] && distances[i]<minDistance)
                {
                    minDistance=distances[i];
                    minIndex=i;
                }
            }

            if (minIndex==-1)
            {
                break;
            }

            visited[minIndex]=true;

            Edge *current=nodes[minIndex].edges;
            while (current)
            {
                int neighbor=current->end;
                double weight=current->weight;

                if (!visited[neighbor] && distances[minIndex] + weight<distances[neighbor])
                {
                    distances[neighbor]=distances[minIndex] + weight;
                    previous[neighbor]=minIndex;
                }

                current=current->next;
            }
        }

        if (distances[endIndex]==infinity)
        {
            cout<<"No path exists between "<<start<<" and "<<end<<".\n";
        }
        else
        {
            cout<<"Shortest path from "<<start<<" to "<<end<<" is "<<distances[endIndex]<<".\n";

            int path[nodeCount];
            int count=0;
            for (int at=endIndex; at != -1; at=previous[at])
            {
                path[count++]=at;
            }

            cout<<"Path: ";
            for (int i=count-1;i>=0;i--)
            {
                if (i<count-1)
                {
                    cout<<" -> ";
                }
                cout<<nodes[path[i]].id;
            }
            cout<<"\n";
        }

        delete[] distances;
        delete[] visited;
        delete[] previous;
    }


};

class Vehicle
{
    public:
    string vehicleID;
    string startIntersection;
    string endIntersection;
    Vehicle()
    {
        vehicleID="";
        startIntersection="";
        endIntersection="";
    }
    Vehicle(const string &vehicleID, const string &start, const string &end)
    {
        this->vehicleID=vehicleID;
        this->startIntersection=start;
        this->endIntersection=end;
    }
};

class VehicleRoutingSystem
{
    public:
    Graph &graph;
    Vehicle vehicles[100];
    int vehicleCount;
    VehicleRoutingSystem(Graph &graph) : graph(graph), vehicleCount(0) {}

    void loadVehicles(const string &filename)
    {
        ifstream file(filename);
        string line;

        // Skip the header line
        getline(file, line);

        while (getline(file, line))
        {
            string vehicleID, start, end;
            int i=0;

            // Extract vehicle ID
            while (line[i]!=',')
            {
                vehicleID+=line[i++];
            }
            i++;

            // Extract start intersection
            while (line[i]!=',')
            {
                start+=line[i++];
            }
            i++;

            // Extract end intersection
            while (i<line.length())
            {
                end+=line[i++];
            }

            vehicles[vehicleCount++]=Vehicle(vehicleID, start, end);
        }

        file.close();
    }

    void routeVehicles()
    {
        for (int i=0;i<vehicleCount;i++)
        {
            graph.dijkstra(vehicles[i].startIntersection, vehicles[i].endIntersection);
        }
    }
};

class Intersection
{
    public:
    string id;
    int greenTime;
    Intersection *next;
    Intersection(string id = "", int greenTime = 0 )
    {
        this->id=id;
        this->greenTime=greenTime;
        next=NULL;
    }
};

class EmergencyVehicle
{
    public:
    string vehicleID;
    string startIntersection;
    string endIntersection;
    string priorityLevel;
    EmergencyVehicle *next;
    EmergencyVehicle(string vehicleID = "", string startIntersection = "", string endIntersection = "", string priorityLevel = "" )
    {
        this->vehicleID=vehicleID;
        this->startIntersection=startIntersection;
        this->endIntersection=endIntersection;
        this->priorityLevel=priorityLevel;
        next=NULL;
    }
};

class TrafficSignalManager
{
    public:
    Intersection *head;
    EmergencyVehicle *emergencyHead;
    TrafficSignalManager()
    {
        head=NULL;
        emergencyHead=NULL;
    }

    void loadTrafficSignalTimings(const string &filename)
    {
        ifstream file(filename);
        string line;
        getline(file, line);

        while(getline(file, line))
        {
            string intersectionID;
            int greenTime=0;
            int i=0;

            // Extract intersectionID
            while(line[i]!=',' && i<line.length())
            {
                intersectionID+=line[i];
                i++;
            }
            i++;

            // Extract greenTime
            while(i<line.length())
            {
                greenTime=greenTime*10+(line[i]-'0');
                i++;
            }

            Intersection *newIntersection=new Intersection(intersectionID, greenTime);
            newIntersection->next=head;
            head=newIntersection;
        }
        file.close();
    }

    void loadEmergencyVehicles(const string &filename)
    {
        ifstream file(filename);
        string line;
        getline(file, line);

        while(getline(file, line))
        {
            string vehicleID, startIntersection, endIntersection, priorityLevel;
            int i=0;

            // Extract vehicleID
            while(line[i]!=',' && i<line.length())
            {
                vehicleID+=line[i];
                i++;
            }
            i++;

            // Extract startIntersection
            while(line[i]!=',' && i<line.length())
            {
                startIntersection+=line[i];
                i++;
            }
            i++;

            // Extract endIntersection
            while(line[i]!=',' && i<line.length())
            {
                endIntersection+=line[i];
                i++;
            }
            i++;

            // Extract priorityLevel
            while(i<line.length())
            {
                priorityLevel+=line[i];
                i++;
            }

            EmergencyVehicle *newEmergencyVehicle=new EmergencyVehicle(vehicleID, startIntersection, endIntersection, priorityLevel);
            newEmergencyVehicle->next=emergencyHead;
            emergencyHead=newEmergencyVehicle;
        }
        file.close();
    }

    void manageTraffic()
    {
        Intersection *current=head;
        while(current!=NULL)
        {
            cout<<"Green signal at Intersection "<<current->id<<" for "<<current->greenTime<<" seconds.\n";
            current=current->next;
        }

        while(emergencyHead!=NULL)
        {
            EmergencyVehicle *ev=emergencyHead;
            emergencyHead=emergencyHead->next;
            cout<<"Emergency Override for Vehicle "<<ev->vehicleID<<" from "<<ev->startIntersection<<" to "<<ev->endIntersection<<"\n";
            delete ev;
        }
    }

    ~TrafficSignalManager()
    {
        while(head!=NULL)
        {
            Intersection *temp=head;
            head=head->next;
            delete temp;
        }

        while(emergencyHead!=NULL)
        {
            EmergencyVehicle *temp=emergencyHead;
            emergencyHead=emergencyHead->next;
            delete temp;
        }
    }
};
int main()
{
    Graph graph(100);
    TrafficSignalManager tsm;
    tsm.loadTrafficSignalTimings("traffic_signal_timings.csv");
    tsm.loadEmergencyVehicles("emergency_vehicles.csv");
    graph.loadRoadNetwork("road_network.csv");
    while (true)
    {
        cout<<"Menu:\n";
        cout<<"1. Add edge\n";
        cout<<"2. Display graph\n";
        cout<<"3. Shortest path\n";
        cout<<"4. Route vehicles\n";
        cout<<"5. Manage Traffic\n";
        cout<<"6. Exit\n";
        cout<<"Enter your choice: ";
        int choice;
        cin>>choice;

        if (choice==1)
        {
            string start, end;
            double weight;

            cout<<"Enter start intersection: ";
            cin>>start;
            cout<<"Enter end intersection: ";
            cin>>end;
            cout<<"Enter travel time: ";
            cin>>weight;

            graph.addEdge(start, end, weight);

            // Append the edge to the file
            ofstream file("road_network.csv", ios::app);
            file<<start<<","<<end<<","<<weight<<"\n";
            file.close();

            cout<<"Edge added and saved to file.\n";
        }
        else if (choice==2)
        {
            graph.displayGraph();
        }
        else if (choice==3)
        {
            string start, end;
            cout<<"Enter start intersection: ";
            cin>>start;
            cout<<"Enter end intersection: ";
            cin>>end;
            cout<<endl;
            graph.dijkstra(start, end);
            cout<<endl;
        }
        else if (choice==4)
        {
            VehicleRoutingSystem vrs(graph);
            vrs.loadVehicles("vehicles.csv");
            cout<<endl;
            vrs.routeVehicles();
            cout<<endl;
        }
        else if (choice==5)
        {
            tsm.manageTraffic();
        }
        else if (choice==6)
        {
            break;
        }
        else
        {
            cout<<"Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
