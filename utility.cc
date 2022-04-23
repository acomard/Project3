
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//given a file name and the range of years, will compute the length of the vertex array for the file.
int getVertexCount(string filename, int startYear, int endYear){
    ifstream vertexStream(filename, ios::in);
    string line, labelStr, dateStr;
    int vertexLabel, date;

    int length = 0;
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
    length++;
    while(date <= endYear && getline(vertexStream, line)){
        istringstream parseVertex(line);
        parseVertex >> vertexLabel;
        parseVertex >> date;

        //process data
        length++;
    }
    return length;
}

//get the edge count in a file for the range
int getEdgeCount(string filename, int vertices[], int n){
    ifstream edgeStream(filename, ios::in);
    string line;
    int from, to;

    int length = 0;
    //loop through the file
    while(getline(edgeStream, line)){
        bool fromInRange = false;
        bool toInRange = false;
        istringstream parseEdge(line);
        parseEdge >> from;
        parseEdge >> to;
        //check if the first vertex is in the range
        for(int i = 0; i < n; i++){
            if(vertices[i] == from){
                fromInRange = true;
            }
            if(vertices[i] == to){
                toInRange = true;
            }
        }
        if(fromInRange && toInRange){
            length++;
        }
    }
    return length;
}

void insertionSort(int array[], int size){
    int i, j, key;
    for(i = 1; i < size; i++){
        key = array[i];
        j = i - 1;

        while(j >= 0 && array[j] > key){
            array[j+1] = array[j];
            j = j - 1;
        }
        array[j+1] = key;
    }
}