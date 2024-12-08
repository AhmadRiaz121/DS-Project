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

        // Add edge
        Edge *newEdge=new Edge{endIndex, weight, nodes[startIndex].edges};
        nodes[startIndex].edges=newEdge;

        // Add reverse edge
        Edge *reverseEdge=new Edge{startIndex, weight, nodes[endIndex].edges};
        nodes[endIndex].edges=reverseEdge;
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

    void findAllRoutesHelper(int start, int end, bool* visited, int* path, int pathIndex)
    {
        visited[start]=true;
        path[pathIndex]=start;
        pathIndex++;

        if(start==end)
        {
            // Print the current path
            for (int i=0; i<pathIndex; i++)
            {
                cout << nodes[path[i]].id;
                if (i<pathIndex - 1) cout << " -> ";
            }
            cout << endl;
        }
        else
        {
            Edge* current = nodes[start].edges;
            while (current)
            {
                if (!visited[current->end])
                {
                    findAllRoutesHelper(current->end, end, visited, path, pathIndex);
                }
                current = current->next;
            }
        }

        // Backtrack
        visited[start]=false;
    }

    void findAllRoutes(const string &start, const string &end)
    {
        int startIndex=getNodesIndex(start);
        int endIndex=getNodesIndex(end);

        if (startIndex==-1 || endIndex==-1)
        {
            cout<<"Invalid start or end intersection.\n";
            return;
        }

        bool *visited=new bool[nodeCount];
        int *path=new int[nodeCount];
        for(int i=0;i<nodeCount;i++)
        {
            visited[i]=false;
        }

        cout<<"All possible routes from "<<start<<" to "<<end<<":\n";
        findAllRoutesHelper(startIndex, endIndex, visited, path, 0);

        delete[] visited;
        delete[] path;
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

        for (int i=0;i<nodeCount;i++)
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

            for (int i=0;i<nodeCount;i++)
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
            for (int at=endIndex;at != -1;at=previous[at])
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
    Intersection(string id="", int greenTime=0 )
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
    EmergencyVehicle(string vehicleID="", string startIntersection="", string endIntersection="", string priorityLevel="" )
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

    bool comparePriority(string priority1, string priority2)
    {
        return priority1<priority2;
    }
    
    void insertEmergencyVehicleSorted(EmergencyVehicle *newVehicle)
    {
        //Priority Queue
        if(emergencyHead==NULL || comparePriority(newVehicle->priorityLevel, emergencyHead->priorityLevel))
        {
            newVehicle->next=emergencyHead;
            emergencyHead=newVehicle;
        }
        else
        {
            EmergencyVehicle *current=emergencyHead;
            while(current->next!=NULL && !comparePriority(newVehicle->priorityLevel, current->next->priorityLevel))
            {
                current=current->next;
            }
            newVehicle->next=current->next;
            current->next=newVehicle;
        }
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

class RoadSegment
{
    public:
    string startIntersection;
    string endIntersection;
    int travelTime;
    bool isBlocked;
    RoadSegment *next;
    RoadSegment(string start="", string end="", int time=0, bool blocked=false)
    {
        this->startIntersection=start;
        this->endIntersection=end;
        this->travelTime=time;
        this->isBlocked=blocked;
        next=NULL;
    }
};

class RoadNetwork
{
    public:
    RoadSegment *head;
    RoadNetwork()
    {
        head=NULL;
    }

    void addRoadSegment(const string &start, const string &end, int travelTime)
    {
        RoadSegment *newRoad=new RoadSegment(start, end, travelTime, false);
        newRoad->next=head;
        head=newRoad;
    }

    void loadRoadNetwork(const string &filename)
    {
        ifstream file(filename);
        if(!file.is_open())
        {
            cout<<"Error opening file.\n";
            return;
        }

        string line;
        getline(file, line);

        while(getline(file, line))
        {
            string start="", end="", timeStr="";
            int i=0;

            // Extract start intersection
            while(i<line.length() && line[i]!=',')
            {
                start+=line[i];
                i++;
            }
            i++;

            // Extract end intersection
            while(i<line.length() && line[i]!=',')
            {
                end+=line[i];
                i++;
            }
            i++;

            // Extract travel time
            while(i<line.length())
            {
                timeStr+=line[i];
                i++;
            }

            // Convert travel time to integer
            int travelTime=stoi(timeStr);
            addRoadSegment(start, end, travelTime);
        }
        file.close();
    }

    void loadAccidentOrClosures(const string &filename)
    {
        ifstream file(filename);
        if(!file.is_open())
        {
            cout<<"Error opening the file\n";
            return;
        }

        string line;
        getline(file, line);

        while(getline(file, line))
        {
            string start="", end="", status="";
            int i=0;

            //Extract start intersection
            while(i<line.length() && line[i]!=',')
            {
                start+=line[i];
                i++;
            }
            i++;

            //Extract end intersection
            while(i<line.length() && line[i]!=',')
            {
                end=+line[i];
                i++;
            }
            i++;

            //Extract status
            while(i<line.length())
            {
                status+=line[i];
                i++;
            }

            bool isBlocked=(status=="Blocked");
            RoadSegment *current=head;
            while(current !=NULL)
            {
                if(current->startIntersection==start && current->endIntersection==end)
                {
                    current->isBlocked=isBlocked;
                    break;
                }
                current=current->next;
            }
        }
        file.close();
    }

    void monitorCongestion()
    {
        RoadSegment *current=head;
        while(current!=NULL)
        {
            cout<<"Road: "<<current->startIntersection<<" -> "<<current->endIntersection<<", Travel Time: "<<current->travelTime;
            cout<<", Status: "<<(current->isBlocked?"Blocked":"Clear")<<"\n";
            current=current->next;
        }
    }

    void identifyBlockedRoads()
    {
        RoadSegment *current=head;
        cout<<"Blocked Roads:\n";
        while(current!=NULL)
        {
            if(current->isBlocked)
            {
                cout<<"Road: "<<current->startIntersection<<" -> "<<current->endIntersection<<" is blocked"<<"\n";
            }
            current=current->next;
        }
    }

    void rerouteTraffic(string start)
    {
        RoadSegment *current=head;
        cout<<"Rerouting traffic from: "<<start<<"\n";
        while(current!=NULL)
        {
            if(current->startIntersection==start && !current->isBlocked)
            {
                cout<<"Traffic rerouted to: "<<current->endIntersection<<"\n";
            }
            current=current->next;
        }
    }

    void blockRoad(string start, string end)
    {
        RoadSegment *current=head;
        bool found=false;
        while(current!=NULL)
        {
            if(current->startIntersection==start && current->endIntersection==end)
            {
                current->isBlocked=true;
                found=true;
                break;
            }
            current=current->next;
        }

        if(!found)
        {
            cout<<"Road from "<<start<<" to "<<end<<" not found in the network.\n";
            return;
        }

        ofstream file("road_closures.csv", ios::app);
        if(!file.is_open())
        {
            cout<<"Error opening the file\n";
            return;
        }
        
        file<<start<<","<<end<<",Blocked\n";
        file.close();

        cout<<"Road from "<<start<<" to "<<end<<" has been blocked.\n";
    }

    void dijkstraEmergency(const string &start, const string &end, Node *nodes, int nodeCount, int infinity) 
    {
        int startIndex=getNodesIndex(start, nodes, nodeCount);
        int endIndex=getNodesIndex(end, nodes, nodeCount);
    
        if (startIndex==-1 || endIndex==-1)
        {
            cout<<"Invalid start or end intersection.\n";
            return;
        }
    
        double *distances=new double[nodeCount];
        bool *visited=new bool[nodeCount];
        int *previous=new int[nodeCount];
    
        for (int i=0;i<nodeCount;i++) 
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
    
            for (int i=0;i<nodeCount;i++) 
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
    
                // Check if the road is blocked
                if (!visited[neighbor] && !isRoadBlocked(nodes[minIndex].id, nodes[neighbor].id) && distances[minIndex] + weight<distances[neighbor]) 
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
            for (int at=endIndex;at != -1;at=previous[at]) 
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
    
    // Helper function to check if a road is blocked
    bool isRoadBlocked(const string &start, const string &end) 
    {
        RoadSegment *current=head;
        while (current != NULL) 
        {
            if (current->startIntersection==start && current->endIntersection==end) 
            {
                return current->isBlocked;
            }
            current=current->next;
        }
        return false;
    }
    
    // Helper function to get node index
    int getNodesIndex(const string &id, Node *nodes, int nodeCount) 
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
    ~RoadNetwork()
    {
        while(head!=NULL)
        {
            RoadSegment *temp=head;
            head=head->next;
            delete temp;
        }
    }

};

int main()
{
    Graph graph(100);
    TrafficSignalManager tsm;
    RoadNetwork rN;
    tsm.loadTrafficSignalTimings("traffic_signals.csv");
    graph.loadRoadNetwork("road_network.csv");
    rN.loadRoadNetwork("road_network.csv");
    rN.loadAccidentOrClosures("road_closures.csv");
    tsm.loadTrafficSignalTimings("traffic_signals.csv");
    tsm.loadEmergencyVehicles("emergency_vehicles.csv");

    while (true)
    {
        cout<<"------ Simulation Dashboard ------\n";
        cout<<"1. Display City Traffic Network\n";
        cout<<"2. Display Traffic Signal Status \n";
        cout<<"3. Display Congestion Status\n";
        cout<<"4. Display Blocked Roads\n";
        cout<<"5. Emergency Vehicle Routing\n";
        cout<<"6. Block Road due to Accident\n";
        cout<<"7. Reroute traffic\n";
        cout<<"8. Shortest path\n";
        cout<<"9. Route vehicles\n";
        cout<<"10. Simulate Vehicle Routing\n";
        cout<<"11. Exit\n";
        cout<<"Enter your choice: ";
        int choice;
        cin>>choice;
        if (choice==1)
        {
            cout<<endl;
            graph.displayGraph();
            cout<<endl;
        }
        else if (choice==2)
        {
            cout<<endl;
            tsm.manageTraffic();
            cout<<endl;
        }
        else if (choice==3)
        {
            cout<<endl;
            rN.monitorCongestion();
            cout<<endl;
        }
        else if (choice==4)
        {
            cout<<endl;
            rN.identifyBlockedRoads();
            cout<<endl;
        }
        else if (choice==5)
        {
            string start, end;
            cout<<"Enter start intersection: ";
            cin>>start;
            cout<<"Enter end intersection: ";
            cin>>end;
            cout<<endl;
            rN.dijkstraEmergency(start, end, graph.nodes, graph.nodeCount, graph.infinity);
            cout<<endl;
        }
        else if (choice==6)
        {
            string start, end;
            cout<<"Enter start intersection: ";
            cin>>start;
            cout<<"Enter end intersection: ";
            cin>>end;
            cout<<endl;
            rN.blockRoad(start, end);
            cout<<endl;
        }
        else if (choice==7)
        {
            string start;
            cout<<"Enter start intersection: ";
            cin>>start;
            cout<<endl;
            rN.rerouteTraffic(start);
            cout<<endl;
        }
        else if (choice==8)
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
        else if (choice==9)
        {
            VehicleRoutingSystem vrs(graph);
            vrs.loadVehicles("vehicles.csv");
            cout<<endl;
            vrs.routeVehicles();
            cout<<endl;
        }
        else if (choice==10)
        {
            string start,end;
            cout<<"Enter start intersection: ";
            cin>>start;
            cout<<"Enter end intersection: ";
            cin>>end;
            cout<<endl;
            graph.findAllRoutes(start, end);
            cout<<endl;
        }
        else if (choice==11)
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
