#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
class Puzzle {
public:
    int k;
    vector<int> board;

    Puzzle(){
        k = 0;
    }
    Puzzle(int k, const vector<int>& board){
        this->k = k;
        this->board = board;
    }

    bool isGoal() const {
        for (int i = 0; i < k * k - 1; i++)
            if (board[i] != i + 1) return false;
        return board.back() == 0;
    }

    string key() const {
        ostringstream oss;
        for (int v : board) {
            oss << v << ',';
        }
        return oss.str();
    }

    int blankIndex() const {
        for (int i = 0; i < (int)board.size(); i++) if (board[i] == 0) return i;
        return -1;
    }

    vector<Puzzle> neighbors() const {
        vector<Puzzle> out;
        int idx = blankIndex();
        int r = idx / k; 
        int c = idx % k;
        const int dr[4] = {-1,1,0,0};
        const int dc[4] = {0,0,-1,1};
        for (int i=0;i<4;i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr<0 || nr>=k || nc<0 || nc>=k) continue;
            int nidx = nr*k + nc;
            Puzzle p = *this;
            swap(p.board[idx], p.board[nidx]);
            out.push_back(move(p));
        }
        return out;
    }

    static bool solvable(const vector<int>& board, int k) {
        vector<int> tiles;
        for (int v : board) {
            if (v != 0) tiles.push_back(v);
        }

        int inv = 0;
        int n = (int)tiles.size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (tiles[i] > tiles[j]) ++inv;
            }
        }

        if (k % 2 == 1) return (inv % 2) == 0;

        int blank_pos = find(board.begin(), board.end(), 0) - board.begin();
        int row_from_top = blank_pos / k;
        int row_from_bottom = k - row_from_top;

        if (row_from_bottom % 2 == 0) return (inv % 2) == 1;
        return (inv % 2) == 0;
    }
};
