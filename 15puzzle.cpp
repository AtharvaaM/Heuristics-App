#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <cmath>     
#include <algorithm>
#include <iomanip>
using namespace std;

//starting config
vector<vector<int>> initial = {
    {12, 1, 10, 2},
    {7, 11, 4, 14},
    {5, 0, 9, 15},
    {8, 13, 6, 3}
};
//desired final state
vector<vector<int>> final = {
    {1,  2,  3,  4},
    {5,  6,  7, 8},
    {9,  10, 11,  12},
    {13, 14, 15, 0}
};

vector<vector<int>> curr = initial;

typedef struct node 
{
    vector<vector<int>> board;
    int moves;
    int mandist;
    string path;

    //chosen heuristic: number of moves to reach this state + manhatten distance
    bool operator>(const node& other) const {
        return (moves + mandist) > (other.moves + other.mandist); 
    }
} Node;

int man_dist(const vector<vector<int>>& curr_b)
{
    //sum of number of moves taken by blocks to reach desired config without hindrance
    int dist = 0;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            int x = curr_b[i][j];
            if (x == 0) continue;
            dist += abs(i - (x-1)/4);
            dist += abs(j - (x-1)%4);
        }
    }
    return dist;
}


vector<Node> neighbours(Node& curr)
{
    vector<Node> answer;

    int x = 0, y = 0;
    bool found = false;
    for (int i = 0; i< 4 && !found; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            if (curr.board[i][j] == 0) 
            {
                x =i; y =j;
                found = true;
                break;
            }
        }
    }

    //four possible neighbours 

    if (x != 0) 
    {
        Node next;
        next.board= curr.board;
        next.board[x][y]= next.board[x-1][y];
        next.board[x-1][y]= 0;
        next.path = curr.path +'U';
        next.moves =curr.moves + 1;
        next.mandist =man_dist(next.board);
        answer.push_back(next);
    }
    
    if (x != 3) 
    {
        Node next;
        next.board = curr.board;
        next.board[x][y] = next.board[x+1][y];
        next.board[x+1][y]= 0;
        next.path = curr.path +'D';
        next.moves = curr.moves + 1;
        next.mandist = man_dist(next.board);
        answer.push_back(next);
    }

    if (y != 0) 
    {
        Node next;
        next.board = curr.board;
        next.board[x][y] = next.board[x][y-1];
        next.board[x][y-1] = 0;
        next.path= curr.path + 'L';
        next.moves = curr.moves + 1;
        next.mandist = man_dist(next.board);
        answer.push_back(next);
    }

    if (y != 3) 
    {
        Node next;
        next.board = curr.board;
        next.board[x][y]= next.board[x][y+1];
        next.board[x][y+1] = 0;
        next.path = curr.path + 'R';
        next.moves = curr.moves +1;
        next.mandist = man_dist(next.board);
        answer.push_back(next);
    }

    return answer;
}

void printboards(vector<vector<int>> curr, string path)
{
    //print all states the puzzle goes through via backtracking using the stored path
    if (path.empty()) {
        cout << "INITIAL STATE:\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) cout << setw(2)<<curr[i][j] << " ";
            cout << "\n";
        }
        return;
    }

    int x, y;
    bool found = false;
    for (int i = 0; i < 4 && !found; i++) {
        for (int j = 0; j < 4; j++) {
            if (curr[i][j] == 0) 
            { 
                x = i; 
                y = j; 
                found = true; 
                break; 
            }
        }
    }

    vector<vector<int>> prev = curr;
    char last_move = path.back();
    string remaining_path = path.substr(0, path.size() - 1);
    if (last_move == 'U') swap(prev[x][y], prev[x +1][y]); 
    else if (last_move== 'D') swap(prev[x][y], prev[x -1][y]); 
    else if (last_move == 'L') swap(prev[x][y],prev[x][y+1]); 
    else if (last_move =='R') swap(prev[x][y], prev[x][y -1]); 

    printboards(prev, remaining_path);

    cout << "\nMove: " << last_move << "\n";
    cout << "------------------------------\n";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << setw(2)<<curr[i][j] << " ";
        }
        cout << "\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    priority_queue <Node , vector<Node>, greater<Node>> pq;
    set<vector<vector<int>>> visited;
    Node start;
    start.board = initial;
    visited.insert(start.board);
    start.mandist = man_dist(initial);
    start.moves = 0;
    start.path = "";
    pq.push(start);
    while(!pq.empty())
    {
        Node temp = pq.top();
        if (temp.mandist == 0) 
        {
            cout << "\n---------- SOLUTION FOUND ----------\n";
            printboards(temp.board, temp.path);
            cout << "Moves: " <<temp.moves<<"\n";
            cout << "Path: "<<temp.path <<"\n";
            break;
        }
        pq.pop();
        for (auto it: neighbours(temp))
        {
            if (visited.find(it.board) ==visited.end())
            {
                pq.push(it);
                visited.insert(it.board);
            }
        }

    }

}

