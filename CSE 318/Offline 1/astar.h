#pragma once
#include "puzzle.h"
#include "heuristics.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <vector>
#include <limits>
using namespace std;

struct AStarResult {
    bool success = false;
    int expanded = 0;
    int cost = 0;
    vector<vector<int>> path;
};

AStarResult astar(const Puzzle& start, const Heuristic& h) {
    AStarResult res;
    int k = start.k;
    if (!Puzzle::solvable(start.board, k)) return res;

    vector<int> goal_board = start.board;
    for (int i = 0; i < k*k - 1; ++i) goal_board[i] = i+1;
    goal_board.back() = 0;
    string goal_key;
    ostringstream oss;
    for (int v : goal_board) oss << v << ',';
    goal_key = oss.str();

    typedef tuple<int, int, string> PQT; //f,g,key tuple
    struct Cmp {
        bool operator()(const PQT& a, const PQT& b) const {
            return get<0>(a) > get<0>(b);
        }
    };
    priority_queue<PQT, vector<PQT>, Cmp> open;

    unordered_map<string,int> gscore;
    unordered_map<string,string> came_from;

    string start_key = start.key();
    int h0 = h(start.board, k);
    open.emplace(h0, 0, start_key);
    gscore[start_key] = 0;

    unordered_map<string,vector<int>> key_to_board;
    key_to_board[start_key] = start.board;

    while (!open.empty()) {
        auto [f, g, key] = open.top(); 
        open.pop();
        ++res.expanded;
        if (key == goal_key) {
            res.success = true;
            res.cost = g;
            vector<vector<int>> path;
            string cur = key;
            while (true) {
                path.push_back(key_to_board[cur]);
                auto it = came_from.find(cur);
                if (it==came_from.end()) break;
                cur = it->second;
            }
            reverse(path.begin(), path.end());
            res.path = move(path);
            return res;
        }

        vector<int> board = key_to_board[key];
        Puzzle p(k, board);
        for (auto&& nb : p.neighbors()) {
            string nbk = nb.key();
            int tentative_g = g + 1;
            auto itg = gscore.find(nbk);
            if (itg==gscore.end() || tentative_g < itg->second) {
                gscore[nbk] = tentative_g;
                key_to_board[nbk] = nb.board;
                int hn = h(nb.board, k);
                int fn = tentative_g + hn;
                open.emplace(fn, tentative_g, nbk);
                came_from[nbk] = key;
            }
        }
    }
    return res;
}
