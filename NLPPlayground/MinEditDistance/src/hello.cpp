/* Author: German Shabanets Enik
 * This algorithm gives the minimum edit distance between two words.
 * Just out of curiosity, I've implemented the MED function both
 * iteratively (as described in the textbook) and recursively.
 * I then used the chrono library to check which one runs faster.
 * After thorough testing, I've concluded that iterative method is
 * exponentially more effective than the recursive one!
 */
#include <iostream>
#include "hello.h"
#include <vector>
#include <chrono>
using namespace std;

int MED(string source, string target);
int MED_Rescursive(string source, string target);
int min2_0(int a, int b, int c);
int subCost(char s, char t);
void printGrid(vector<vector<int> >& grid);

int main() {
    string w1 = " ", w2 = " ";
    cout << "Enter \"/stop/\" to exit." << endl;

    while(true){
        cout << "Please enter a word: " << flush;
        cin >> w1;

        if(w1 == "/stop/"){
            break;
        }

        cout << "Please enter another one: " << flush;
        cin >> w2;

        auto start = chrono::high_resolution_clock::now();
        int cost1 = MED(w1, w2);
        auto finish = chrono::high_resolution_clock::now();
        chrono::duration<double> time = finish - start;

        cout << "Loopy: " << cost1 << endl;
        cout << "Ran in " << time.count() * 1000 << "ms" << endl;

        start = chrono::high_resolution_clock::now();
        int cost2 = MED_Rescursive(w1, w2);
        finish = chrono::high_resolution_clock::now();
        time = finish - start;

        cout << "Recursive: " << cost2 << endl;
        cout << "Ran in " << time.count() * 1000 << "ms" << endl;
    }

    return 0;
}

int MED(string source, string target){

    size_t lengthS = source.length() + 1;
    size_t lengthT = target.length() + 1;

    //initialzed 2d vector with 0s
    vector<vector<int> > grid (lengthS, vector<int>(lengthT));

    //set the defaults
    for(size_t row = 1; row < grid.size(); row++){
        //deletion cost
        grid[row][0] = grid[row - 1][0] + 1;
    }
    for(size_t col = 1; col < grid[0].size(); col++){
        grid[0][col] = grid[0][col - 1] + 1;
    }

    for(size_t row = 1; row < grid.size(); row++){
        for(size_t col = 1; col < grid[row].size(); col++){
            grid[row][col] = min2_0(grid[row - 1][col] + 1, //del
                                    grid[row - 1][col - 1] + subCost(source[row - 1], target[col - 1]),
                                    grid[row][col - 1] + 1 //ins
                                    );
        }
    }

    int cost = grid.back().back();

    return cost;
}

int min2_0(int a, int b, int c){
    return min(min(a, b), c);
}

int subCost(char s, char t){
    return (s == t) ? 0 : 2;
}

void printGrid(vector<vector<int> >& grid){
    for(auto iterR = grid.begin(); iterR != grid.end(); ++iterR){
        for(auto iterC = (*iterR).begin(); iterC != (*iterR).end(); ++iterC){
            cout << (*iterC) << " ";
        }
        cout << endl;
    }
}

int MED_Rescursive(string source, string target){ //i, i
    if(source.empty() && target.empty()){
        return 0;
    } else {
        //can either delete, substitute, or add
        int del = INT_MAX, sub = INT_MAX, ins = INT_MAX;

        if(!source.empty()){
            string _source = source.substr(1);
            del = 1 + MED_Rescursive(_source, target);
        }

        if(!source.empty() && !target.empty()){
            sub = subCost(source[0], target[0]) + MED_Rescursive(source.substr(1), target.substr(1));
        }

        if(!target.empty()){
            string _target = target.substr(1);
            ins = 1 + MED_Rescursive(source, _target);
        }

        return min2_0(del, sub, ins);
    }
}
