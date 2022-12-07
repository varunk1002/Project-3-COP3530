#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <map>
#include <queue>
#include <stack>
#include <thread>

using namespace std;
using namespace std::chrono;

class House {
public:
    double latitude;
    double longitude;
    int numOfRooms;
    int rent;
    int sqrtFt;
};
//Adjacency List class cited from PageRank project
class AdjacencyList {
    struct home {
        string rooms;
        string living;
        int weight;
        home(){

        }
        home(string rooms, string living, int weight) :
                rooms(rooms), living(living), weight(weight) {}
    };
private:
    map<string, vector<home> > graph;
public:

    void insert(string from, string to, string living);
    void operateSearch(string vertex, string optimal, int option);
    bool inGraph(string houseType, string campus);
    struct comp{
        bool operator()(const pair<string, int>& lhs, const pair<string, int>& rhs) const {
            if(lhs.second != rhs.second) {
                return lhs.second > rhs.second;
            }
            else {
                return lhs.first > rhs.first;

            }
        }
    };
};


bool AdjacencyList::inGraph(string houseType, string campus){

    bool one = graph.find(houseType)==graph.end();
    bool two = graph.find(campus)==graph.end();
    if(!one && !two){
        return true;
    }
    else{
        return false;
    }
}
void AdjacencyList::insert(string from, string to, string living){
    //INSERT FUNCTION CITED BY PROFESSOR AMANPREET KAPOOR'S LECTURES AND PROJECT 2 (PAGERANK)
    home houseType;
    houseType.rooms = from;
    houseType.living = living;
    houseType.weight = 1;
    home campus;
    campus.rooms = to;
    campus.living = living;
    campus.weight = 2;
    if(graph.find(from) != graph.end()){
        graph[from].push_back(campus);
    }
    else{
        vector<home> temp;
        temp.push_back(campus);
        graph[from] = temp;
    }
    if(graph.find(to) != graph.end()){
        graph[to].push_back(houseType);
    }
    else{
        vector<home> temp;
        temp.push_back(houseType);
        graph[to] = temp;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdjacencyList::operateSearch(string vertex, string optimal, int option ){
    switch(option){
        case 1: {
            //initialize variables
            stack <string> sta;
            sta.push(vertex);
            map<string, bool> visited;
            map<string, home> parents;
            bool found = false;

            home vertexHome;
            home optimalHome;

            vertexHome.rooms = vertex;
            optimalHome.rooms = optimal;
            //iterate through stack
            if(!sta.empty() && !found){
                string firsthome = sta.top();
                sta.pop();
                for(auto currentAdjacent : graph[firsthome])
                {
                    if(!visited[currentAdjacent.rooms] && currentAdjacent.rooms != vertex)
                    {
                        sta.push(currentAdjacent.rooms);
                        visited[currentAdjacent.rooms] = true;
                        parents[currentAdjacent.rooms].rooms = firsthome;
                        parents[currentAdjacent.rooms].living = currentAdjacent.living;
                        if(currentAdjacent.rooms == optimal){
                            optimalHome = currentAdjacent;
                            found = true;
                            break;
                        }
                    }
                }
            }
            stack<home> s;
            string currentVal = optimal;
            home currenthome = optimalHome;
            while(parents[currentVal].rooms != ""){
                currenthome.living = parents[currentVal].living;
                s.push(currenthome);
                currenthome = parents[currentVal];
                currentVal = parents[currentVal].rooms;
                
            }
            cout << vertex;
            while(!s.empty()){
                auto current = s.top();
                transform(current.living.begin(), current.living.end(),current.living.begin(), ::toupper);
                cout << "->"  <<   "(" << current.living << ")" << std::flush;
                cout << "->"  << current.rooms << std::flush;
                s.pop();
            }
            cout << endl;
        }
            break;
        //Djikstra's Algorithm Search
        case 2: {
            priority_queue<pair<string, int>, vector<pair<string, int> >, comp> pq;
            pq.push(make_pair(vertex,0));
            unordered_map<string, int> dist;
            unordered_map<string, pair<string, string> > prev;
            unordered_map<string, bool> done;
            for(const auto& elem : graph){
                auto adjacentHomes = elem.second;
                for(auto current : adjacentHomes){
                    int initialDist = current.rooms == vertex ? 0 : INT_MAX;
                    dist[current.rooms] = initialDist;
                }
            }
            bool found = false;
            while(!pq.empty()){
                auto top = pq.top();
                pq.pop();
                for(auto adjacentHome : graph[top.first]){
                    int weight = adjacentHome.weight;
                    int furthest = dist[top.first];
                    if(!done[adjacentHome.rooms] && ((furthest + weight) < dist[adjacentHome.rooms])){
                        dist[adjacentHome.rooms] = furthest + weight;

                        prev[adjacentHome.rooms] = make_pair(top.first, adjacentHome.living);

                        if(adjacentHome.rooms == optimal) found = true;
                        pq.push(make_pair(adjacentHome.rooms, furthest + weight));
                    }
                }
                done[top.first] = true;
            }
            if(!found){
                return;
            }
            string val = optimal;
            stack<home> s;
            while(val != vertex){
                auto currentPrev = prev[val];
                s.push(home(val, currentPrev.second, dist[val]));
                val = currentPrev.first;
            }
            int cost = 0;
            cout << vertex;
            while(!s.empty()){
                auto top = s.top();
                s.pop();
            }
        }
            break;
    }
}
int main() {
    //
    AdjacencyList graph;
    vector<vector<string> > content;
    vector<string> row;
    string line, word;

    //reach file
    ifstream file;
    file.open("collegeHousing.csv", ios::in);

    //read through file to obtain parameter values + assign to objects
    int count = 0;
    int i = 0;
    if(file.is_open())
    {
        while(getline(file, line))
        {
            row.clear();
            stringstream str(line);
            while(getline(str, word, ',')){
                if(row.size() < 3 && word.find(","))
                    while(word.find(",") != string::npos)
                        word.erase(word.begin() + word.find(","));
                row.push_back(word);
            }
            content.push_back(row);

            graph.insert(row[0],row[1],row[2]);
            count++;
        }
        file.close();
    }
    else{

        return -1;
    }
    //console interface to be run in terminal
    cout << endl;
    bool run =  true;
    cout << "Hi, welcome to Flillow, a real estate solution made specifically for Florida college students lke you!" << endl;
    cout << "It's a pleasure to meet you, what is your name? ";
    string name;
    cin >> name;
    cout<<endl;
    cout<< "Ok " << name << ", please input below and press enter to submit!" << endl<<endl;
    // UI in terminal (SCOPED DOWN DUE TO LAST-MINUTE LOSS OF THIRD MEMBER) - APPROVED BY PROF. KAPOOR
    while(run){

        cout << "Tell us your preferred housing style: ";
        string housingStyles;
        cin >> housingStyles;
        cout << endl;

        cout << "Please pick one of the following colleges (UF, UCF, UM, USF): " ;
        string college;
        cin >> college;
        cout << endl;

        if(college == "UF" | college == "UCF" | college == "USF" | college == "UM" ){
            cout<<endl;
            continue;
        }
        else {
            cout<< "Invalid entry, please  over."<< endl;
            cout << "Please pick one of the following colleges (UF, UCF, UM, USF): " ;
            cin >> college;
        }
        cout<<endl;

        char yesNo;

        cout << "So you want a " << housingStyles << " near " << college << "? (y/n) ";
        cin>>yesNo;
        cout << endl;
        if(yesNo == 'y'){
            cout << "1 - Depth First Search (DFS)" << endl;
            cout << "2 - Djikstra's Algorithm Search" << endl<<endl;
            cout << "Pick a searching algorithm to find your next home! (Option 1 or 2): ";
            int search;
            cin >> search;
            cout << endl;
            bool valid = false;
            if(search == 1 | search == 2){
                valid = true;
            } else{
                while(!valid){
                    cout << "Please input a search valid value: " << endl;
                    cin >> search;
                    if(search == 1){
                        valid = true;
                    }
                    if (search == 2){
                        valid = true;
                    }
                }
            }
            if(search == 1){
                cout << "Depth-First Search" << endl;
                auto start = high_resolution_clock::now();
                graph.operateSearch(housingStyles, college, 1);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(stop - start);
                cout << "Time taken to perform DFS: " << duration.count() << " ms" << endl;
                //std::cout << houses.size() << " ARE LOADED " << std::endl;

            }
            if(search == 2){
                cout << "Dijkstra's Algorithm Search" << endl;
                auto start = high_resolution_clock::now();
                graph.operateSearch(housingStyles, college, 2);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(stop - start);
                cout << "Time taken to perform Djikstra's Algorithm: " << duration.count() << " ms" << endl;

            }

            cout << "Would you like to search again? (y/n):  " ;
            char searchAgain;
            cin >> searchAgain;
            cout << endl;
            cin.ignore();
            if(searchAgain == 'y'){
                continue;
            }
            if(searchAgain == 'n'){
                cout << "Glad I could help, please come again!" << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                terminate();

            }
        } else {
            cout << "Sorry for my mistake, please come again!" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            terminate();
        }

    }

    return 0;
}


