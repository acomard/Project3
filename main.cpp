#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "utility.cc"
#include "Graph.cc"

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
        istringstream parseEdge(edgeStr);
        parseEdge >> from;
        parseEdge >> to;
        //check if the first vertex is in the range
        for(int i = 0; i < range; i++){
            if(V[i] == from){
                fromInRange = true;
            }
            if(V[i] == to){
                toInRange = true;
            }
        }
        if(fromInRange && toInRange){
            E[count].start = from;
            E[count].end = to;
            count++;
        }
    }

    for(int i = 0; i < edgeCount; i++){
        cout << E[i].start;
        cout << " ";
        cout << E[i].end;
        cout << "\n";
    }

    //testing
    //cout << vertexLabel;
    cout << "\n";
    //cout << date;
    cout << edgeCount;
    //cout << endYear;

    //set up an array of vertices where each index is a "vertex label", sort the array, then build an array of edges with the pairs being the indexes that correspod to the vertex label
    //arrays should only have the edges/vertices in the range startYear to endYear
}