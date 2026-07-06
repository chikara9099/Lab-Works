#include <iostream>
#include <vector>
#include <iomanip>
#include "puzzle.h"
#include "heuristics.h"
#include "astar.h"
#include <chrono>

using namespace std;

struct TestCase {
    string name;
    vector<int> board;
};

int main() {

    const int n = 4;

    vector<TestCase> tests = {
        {"Case 1", {0, 8, 5, 12, 2, 14, 3, 1, 9, 4, 11, 15, 7, 10, 6, 13}},
        {"Case 2", {4, 11, 8, 2, 0, 9, 12, 15, 7, 5, 1, 13, 6, 3, 10, 14}},
        {"Case 3", {2, 4, 6, 8, 14, 3, 13, 5, 7, 9, 11, 0, 12, 15, 10, 1}},
        {"Case 4", {4, 12, 3, 7, 11, 0, 1, 5, 8, 13, 2, 6, 10, 15, 9, 14}},
        {"Case 5", {4, 8, 10, 13, 11, 6, 1, 9, 7, 12, 15, 2, 5, 0, 14, 3}},
        {"Case 6", {9, 3, 15, 1, 10, 14, 0, 4, 13, 2, 7, 5, 6, 11, 8, 12}},
        {"Case 7", {1, 13, 9, 0, 10, 5, 11, 4, 12, 2, 6, 3, 15, 14, 7, 8}},
        {"Case 8", {1, 8, 6, 15, 4, 12, 14, 0, 3, 7, 13, 5, 10, 9, 2, 11}},
        {"Case 9", {14, 10, 7, 2, 0, 5, 6, 9, 4, 1, 12, 15, 3, 8, 11, 13}},
        {"Case 10",{10, 13, 2, 15, 9, 1, 5, 7, 14, 4, 12, 3, 6, 11, 0, 8}}
    };


    CornerLinearConflict heuristic;

    vector<double> weights = {1.0, 1.2, 2.0, 5.0};

    cout << fixed << setprecision(1);

    cout << left
         << setw(10) << "Case"
         << setw(8)  << "Weight"
         << setw(15) << "Moves"
         << setw(18) << "Nodes Explored"
         << setw(15) << "Runtime(ms)"
         << '\n';

    cout << string(66,'-') << '\n';

    for(const auto& tc : tests){

        if(!Puzzle::solvable(tc.board,n)){
            cout << tc.name << " is UNSOLVABLE!\n";
            continue;
        }

        Puzzle p(n, tc.board);

        for(double w : weights){

            auto start = chrono::high_resolution_clock::now();

            auto result = astar(p, heuristic, w);

            auto stop = chrono::high_resolution_clock::now();

            double ms =
                chrono::duration<double,milli>(stop-start).count();

            cout << left
                 << setw(10) << tc.name
                 << setw(8)  << w
                 << setw(15) << result.cost
                 << setw(18) << result.expanded
                 << setw(15) << ms
                 << '\n';
        }

        cout << string(66,'-') << '\n';
    }

    return 0;
}