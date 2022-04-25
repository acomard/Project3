#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "utility.cc"
#include "Graph.cc"
#include <limits>

using namespace std;

int main(int argc, char* argv[]){
    //extract the file names from the command line
    string edgeFile, dateFile, commandFile;
    edgeFile = argv[1];
    dateFile = argv[2];
    commandFile = argv[3];

    ifstream commandStream(commandFile, ios::in);

    //get the start-graph command and place it in startCommand string
    string startCommand;
    getline(commandStream, startCommand);

    int startYear, endYear, range, edgeCount;
    string startYearStr, endYearStr;

    //echo the start command
    cout << "Command: " + startCommand + "\n\n";

    istringstream parseStart(startCommand);
    parseStart >> startYearStr;
    parseStart >> startYearStr;
    parseStart >> endYearStr;

    startYear = stoi(startYearStr);
    endYear = stoi(endYearStr);

    //get the range
    range = getVertexCount(dateFile, startYear, endYear);
    

    //print the number of vertices
    cout << "|V| = " + to_string(range) + " vertices\n";

    //initialize the array of vertex labels for the range
    int V[range];

    
    //process the vertices from the file data
    ifstream vertexStream(dateFile, ios::in);
    string line, labelStr, dateStr;
    int vertexLabel, date;

    int index = 0;
    getline(vertexStream, line);
    istringstream parseVertex(line);
    parseVertex >> vertexLabel;
    parseVertex >> date;
    while(date < startYear){
        getline(vertexStream, line);
        istringstream parseVertex(line);
        parseVertex >> vertexLabel;
        parseVertex >> date;
    }
    //process data
    V[index] = vertexLabel;
    index++;
    while(date <= endYear && getline(vertexStream, line)){
        istringstream parseVertex(line);
        parseVertex >> vertexLabel;
        parseVertex >> date;

        //process data
        V[index] = vertexLabel;
        index++;
    }
    
    //sort the array of vertex labels
    insertionSort(V, range);

    //get the number of edges in the range
    edgeCount = getEdgeCount(edgeFile, V, range);

    //print the number of edges    
    cout << "|E| = " + to_string(edgeCount) + " edges\n\n";
    
    //initialize the array of edge pairs
    graphEdge E[edgeCount];

    
    //construct the edge pair graph
    ifstream edgeStream(edgeFile, ios::in);
    string edgeStr;
    int from, to;

    int count = 0;
    int length = 0;
    //loop through the file
    while(getline(edgeStream, edgeStr)){
        bool fromInRange = false;
        bool toInRange = false;
        int toIndex, fromIndex;
        istringstream parseEdge(edgeStr);
        parseEdge >> from;
        parseEdge >> to;
        //check if the first vertex is in the range
        for(int i = 0; i < range; i++){
            if(V[i] == from){
                fromInRange = true;
                fromIndex = i;
            }
            if(V[i] == to){
                toInRange = true;
                toIndex = i;
            }
        }
        if(fromInRange && toInRange){
            E[count].start = fromIndex;
            E[count].end = toIndex;
            count++;
        }
    }
    
    //construct the graph
    Graph G(E, edgeCount, range);
    
    string command;
    while(getline(commandStream, command)){
        //cout << command;
        if(command == "out-degree"){
            //echo the command
            cout << "Command: out-degree\n\n";

            double average = double(edgeCount)/double(range);

            cout << "The graph G has average out-degree " + to_string(edgeCount) + "/" + to_string(range) + " = " + to_string(average) + "\n";
            cout << "The out-degree distribution is:\n";
            cout << "\tOut-degree : Number of vertices\n";

            int longestDegree = getLongestDegree(G, range);
            int degrees[longestDegree+1] = {0};
            for(int i = 0; i < range; i++){
                int count = 0;
                adjNode* ptr = G.head[i];
                while(ptr != nullptr){
                    count++;
                    ptr = ptr->next;
                }
                degrees[count]++;
            }
            for(int n = 0; n <= longestDegree; n++){
                cout << "\t";
                cout << n;
                cout << " : ";
                cout << degrees[n];
                cout << "\n";
            }
            cout << "\n";
        }
        if(command == "diameter"){
            //echo the command
            cout << "Command: diameter\n\n";

            //setup the adjacency matrix
            int adjacencyMatrix[range][range];
            for(int i = 0; i < range; i++){
                for(int j = 0; j < range; j++){
                    adjacencyMatrix[i][j] = std::numeric_limits<int>::max();
                }
                adjNode* ptr = G.head[i];
                while(ptr != nullptr){
                    adjacencyMatrix[i][ptr->index] = 1;
                    ptr = ptr->next;
                }
                adjacencyMatrix[i][i] = 0;
            }
            //setup complete

            int dist[range][range], l, m, n;

            for(l = 0; l < range; l++){
                for(m = 0; m < range; m++){
                    dist[l][m] = adjacencyMatrix[l][m];
                }
            }
            for(n = 0; n < range; n++){
                for(l = 0; l < range; l++){
                    for(m = 0; m < range; m++){
                        if(dist[l][m] > (dist[l][n] + dist[n][m]) &&(dist[n][m] != std::numeric_limits<int>::max() && dist[l][n] != std::numeric_limits<int>::max())){
                            dist[l][m] = dist[l][n] + dist[n][m];
                        }
                    }
                }
            }
            //end of floyd warshall

            //find the longest shortest path
            int longest = 0;
            for (int r = 0; r < range; r++){
                for(int c = 0; c < range; c++){
                    if(dist[r][c] > longest && (dist[r][c] != std::numeric_limits<int>::max())){
                        longest = dist[r][c];
                    }
                }
            }
            //print the resulting diameter = longest
            cout << "The graph G has diameter ";
            cout << longest;
            cout << ".\n\n";
        }
        if(command == "scc"){
            //echo the command
            cout << "Command: scc\n\n";
        }
    }

    

    //cout << longestDegree;

    for(int i = 0; i < range; i++){
        //displayAdjacencyList(G.head[i], i);
    }

    //testing
    //cout << vertexLabel;
    //cout << "\n";
    //cout << date;
    //cout << edgeCount;
    //cout << endYear;

    //set up an array of vertices where each index is a "vertex label", sort the array, then build an array of edges with the pairs being the indexes that correspod to the vertex label
    //arrays should only have the edges/vertices in the range startYear to endYear
    
}