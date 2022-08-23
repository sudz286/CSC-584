#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
#include <ctime>

using namespace std;


class Graph
{
    public:

        // Graph data structure, unordered map of unordered maps

        unordered_map<int, const unordered_map<int, int> > vertices;
        vector <vector <float>> heuristic;

        void add_vertex(char vertex, const unordered_map<int, int>& edges)
        {
            vertices.insert( unordered_map <int, const unordered_map<int, int>>::value_type(vertex, edges));
        }

        vector<int> path_finder(int first, int final)
        {
            unordered_map<int, int> previous;
            unordered_map<int, int> weights;
            
            vector<int> nodes;
            vector<int> path;
            
            auto comparator = [&] (int left, int right) { return weights[left] > weights[right]; };

            for (auto& vertex : vertices)
            {
                if (vertex.first == first) weights[vertex.first] = 0;
                else weights[vertex.first] = numeric_limits<int>::max();
                
                nodes.push_back(vertex.first);  
                push_heap(begin(nodes), end(nodes), comparator);
            }

            
            while (!nodes.empty())
            {
                pop_heap(begin(nodes), end(nodes), comparator);

                char closest = nodes.back();
                nodes.pop_back();
                
                if (closest == final)
                {
                    while (previous.find(closest) != end(previous))
                    {
                        path.push_back(closest);
                        closest = previous[closest];
                    }
                    
                    break;
                }
                
                if (weights[closest] == numeric_limits<int>::max()) break;
                
                for (auto& neighbor : vertices[closest])
                {
                    // Manhattan distance heuristic
                    int alt = weights[closest] + neighbor.second + (abs(heuristic[closest][0]-heuristic[final][0])*54) + (abs(heuristic[closest][1]-heuristic[final][1])*69 );
                    // Euclidian distance squared
                    // int alt = weights[closest] + neighbor.second + ( (abs(heuristic[closest][0]-heuristic[final][0])*54) * (abs(heuristic[closest][0]-heuristic[final][0])*54) +  abs(heuristic[closest][1]-heuristic[final][1])*69 * (abs(heuristic[closest][1]-heuristic[final][1])*69) );
                    if (alt < weights[neighbor.first])
                    {
                        weights[neighbor.first] = alt;
                        previous[neighbor.first] = closest;
                        make_heap(begin(nodes), end(nodes), comparator);
                    }

                }
            }
              
            return path;
        }
};

int main(int argc, char *argv[])
{
    Graph g;
    ifstream inFile,heuristicFile;
    string state, values;
    vector <vector <int>> vec;
    vector <vector <float>> hvec;
    vector <int> numbers;

    // Command line arguments
    if(argc < 4){ cout<<"CLA 1-> Large or Small Graph, 2->Source Node, 3->Dest Node"<<endl; exit (1);}

    int large = stoi(argv[1]);
    int source = stoi(argv[2]), dest = stoi(argv[3]);
    int max_vertices = 29;
    
    if(large != 1 && large != 0){ cout<<"Enter either 0 or 1 and second CLA"<<endl; exit (1);}

    if(large)
    {
        inFile.open("large_graph.csv");
        heuristicFile.open("large_heuristic.csv");
        max_vertices = 1000;
        if(source < 0 || source > 1000 || dest < 0 || dest > 1000)
        {
            cout<<"Enter source and destination between 0 to 1000"<<endl;
            exit (1);
        }
    }
    else
    {
        inFile.open("small_graph.csv");
        heuristicFile.open("small_heuristic.csv");
        if(source < 0 || source > 29 || dest < 0 || dest > 29)
        {
            cout<<"Enter source and destination between 0 to 29"<<endl;
            exit (1);
        }
    }
    
    int vertex=0;
    int j, count=0;
    while (!inFile.eof()) 
    {
        count++;
        unordered_map<int, int> edges_weights;
        vector <int> vec1;

        getline ( inFile, state, ',' );
        state.erase(remove(state.begin(), state.end(), '\n'), state.end());
        inFile >> values;
        values.erase(remove(values.begin(), values.end(), '\n'), values.end());
        int i = 0, j = 0;
        string temp = "";
        
        while(i<values.length())
        {
            if (values[i] == ',')
            {
                int temp1 = stoi(temp);\
                if (temp1!=0)
                {
                    numbers.push_back(j);
                    int temp1 = stoi(temp);
                    edges_weights.insert(pair<int,int>(numbers[numbers.size()-1],temp1));
                }
                vec1.push_back(temp1);
                temp="";
                j++;
            }
            else temp+=values[i];
            i++;
        }
        int temp1 = stoi(temp);
        
        if(count<=max_vertices)
        {
            numbers.push_back(j);
            edges_weights.insert(pair<int,int>(numbers.size()-1,temp1));
            vec1.push_back(stoi(temp));
            vec.push_back(vec1);
            g.add_vertex(vertex,edges_weights);
            vertex++;
        }
    }

    vec.pop_back();
    inFile.close();


    while (!heuristicFile.eof()) 
    {
        vector <float> vec1;
        getline ( heuristicFile, state, ',' );
        state.erase(remove(state.begin(), state.end(), '\n'), state.end());
        heuristicFile >> values;
        values.erase(remove(values.begin(), values.end(), '\n'), values.end());

        int i = 0;
        string temp = "";
        while(i<values.length())
        {
            if (values[i]==',') {vec1.push_back(stoi(temp)); temp="";}
            else temp+=values[i];
            i++;
        }
        vec1.push_back(stoi(temp));
        hvec.push_back(vec1);
    }
    hvec.pop_back();
    heuristicFile.close();
    g.heuristic=hvec;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // Calling path finding algorithm here
    vector <int> answer = g.path_finder(source, dest);

    end = std::chrono::system_clock::now();
  
    std::chrono::duration <double> elapsed_seconds = end - start;

    for( auto path_node: answer) cout<< path_node << endl;

    cout<<"Time taken: "<<elapsed_seconds.count()<<endl;
    cout<<"Nodes Visited: "<<answer.size()<<endl;
    
    return 0;
}
