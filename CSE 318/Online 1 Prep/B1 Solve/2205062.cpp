#include <iostream>
#include <vector>
#include <memory>
#include "puzzle.h"
#include "heuristics.h"
#include "astar.h"
using namespace std; 

int main() {
    cout << "Enter board size n: ";
    int n;
    cin >> n;

    int total = n * n;
    vector<int> start(total);

    cout << "Enter " << total << " board values (0 for blank):\n";
    for (int i = 0; i < total; ++i) {
        cin >> start[i];
    }

    Puzzle p(n, start);

    LinearConflict lc;
    ManhattanDistance md;
    CornerLinearConflict cl;
    ToroidalManhattanDistance tmc;
    const Heuristic& chosen = tmc;

    auto result = astar(p, chosen);
    if (!result.success) {
        cout << "No solution found (or unsolvable)\n";
        return 1;
    }

    cout << "Minimum total cost = " << result.cost << ", nodes explored = " << result.expanded << "\n";
    for (auto &board : result.path) {
        for (int i = 0; i < total; ++i) {
            if (i % n == 0) cout << "\n";
            cout << board[i] << ' ';
        }
        cout << "\n";
    }
    return 0;
}
