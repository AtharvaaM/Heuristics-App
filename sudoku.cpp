#include <bits/stdc++.h>
using namespace std;

//---------------------------------------
vector<vector<int>> grid = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};
//---------------------------------------

vector<vector<int>> temp_mat = grid;
vector<vector<int>> curr = grid;

int unchanged_itr = 0;

void initialise_board(vector<vector<int>>& curr)
{
    for (int a = 0; a<9; a++)
    {
        vector <int> v;
        for (int i = 1; i<=9; i++) v.push_back(i);

        for (int i = 0; i<9; i++)
        {
            int x = curr[3*(a / 3) + (i / 3)][3*(a % 3)+ (i % 3)]; 
            if (x != 0) 
            {
                auto it = std::find(v.begin(), v.end(), x);
                if (it != v.end()) v.erase(it);
            }
        }
        //we have out list of elements not used in a given 3*3 box
        for (int i = 0; i<9; i++)
        {
            int &x = curr[3*(a / 3) + (i / 3)][3*(a % 3)+ (i % 3)];
            if (x == 0 && !v.empty()) 
            {
                int idx = rand() % v.size();
                x = v[idx];
                v.erase(v.begin() + idx);
            }
        }
        //randomly assign these elements some position
    }
}

int energy(vector<vector<int>>& curr)
{
    //number of duplicates in rows + columns
    int x = 0;
    for (int i = 0; i<9; i++)
    {
        map<int,int> mp;
        for (int j = 0; j<9; j++) mp[curr[i][j]]++;
        for (auto it:mp) x += it.second-1;
    }
    for (int i = 0; i<9; i++)
    {
        map<int,int> mp;
        for (int j = 0; j<9; j++) mp[curr[j][i]]++;
        for (auto it:mp) x += it.second-1;
    }
    return x;
}

void swap_cells(vector<vector<int>>& curr)
{
    //choose a 3*3 grid at random first
    int x = rand()%9;
    vector<pair<int,int>> temps;
    for (int i = 0; i<9; i++)
    {
        if (grid[3*(x / 3) + i / 3][3*(x % 3) + (i%3)] == 0)  
        {
            temps.push_back({3*(x / 3) + i / 3,3*(x % 3) + (i%3)});
        }
    }
    //if less than 2 elements then continue
    if (temps.size() < 2) return; 
    //else swap any 2 elements that were added by us
    int index = rand()%temps.size();
    auto [a1,b1] = temps[index];
    temps.erase(temps.begin()+index);
    auto [a2,b2] = temps[rand()%temps.size()];
    int temp = curr[a1][b1];
    curr[a1][b1] = curr[a2][b2];
    curr[a2][b2] = temp;

}

int main()
{
    srand(time(NULL));
    initialise_board(curr);
/*----------MODIFY PARAMETERS----------*/
    double starting_temp = 1; //big variation at first to speed up process
    double alpha = 0.995; //how to decrease temperature
    double min_temp = 0.0001; //to avoid infinite loop after getting stuck in local minima
    int iterations_per_temp =1; //why not
/*-------------------------------------*/
    
    double temp = starting_temp;
    int curr_score = energy(curr);
    int best_score = curr_score;

    int max_iterations = 100;
    int iteration_number = 1;
    while (iteration_number < max_iterations) //if temp<min_temp or stuck in local minima thern restart
    {
        cout<<"---------- ROUND "<<setw(2)<<iteration_number<<" ----------\n";
        temp = starting_temp;
        iteration_number ++;
        initialise_board(curr);
        temp_mat = curr;
        
        int itr_no = 0;
        while (temp >= min_temp)
        {
            itr_no++;
            cout<<"----------"<<itr_no<<": "<<best_score<<"----------"<<"\n";
            for (int i = 0; i< iterations_per_temp; i++)
            {
                cout<<"     "<<itr_no<<"."<<setw(3)<<i+1<<": "<<best_score<<"\n"; //keeping track of progress
                swap_cells(curr);
                int e1 = energy(curr); //after swappuing
                if (e1 == 0)
                {
                    best_score = curr_score = 0;
                    break;
                }
                int e2 = energy(temp_mat); //old
                int dE = e2 - e1;
                //if new is better, then exp temp always greater, else take worse with probability as given
                if (((float)rand()/float(RAND_MAX)) < exp(dE/temp)) 
                {
                    temp_mat = curr;
                    curr_score = e1;
                    best_score = min(best_score, curr_score);
                }
                else //keep old board
                {
                    curr = temp_mat;
                }
            }
            if (best_score == 0) break;
            temp *= alpha; //modify temperature
        }
        if (best_score == 0) break;
    }

    if (best_score != 0) cout<<"FAILED\n"; 
    else
    {
        for (int i = 0; i < 9; i++)
        {
            if (i == 3 || i == 6) cout << "  ---------+---------+---------\n";
            for (int j = 0; j < 9; j++)
            {
                if (j == 3 || j == 6) cout << "| ";
                cout << setw(2) << curr[i][j] << " ";
            }
            cout << "\n";
        }
    }

    return 0;
}
