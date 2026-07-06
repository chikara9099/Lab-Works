#pragma once
#include <vector>
#include <cmath>
using namespace std;

struct Heuristic {
    virtual int operator()(const vector<int>& state, int k) const = 0;
    virtual ~Heuristic() = default;
};

struct ManhattanDistance : Heuristic {
    int operator()(const vector<int>& state, int k) const override {
        int sum = 0;
        int N = (int)state.size();
        for (int i = 0; i < N; i++) {
            int val = state[i];
            if (val == 0) continue;
            int target = val - 1;
            int r1 = i / k; 
            int c1 = i % k;
            int r2 = target / k; 
            int c2 = target % k;
            sum += abs(r1 - r2) + abs(c1 - c2);
        }
        return sum;
    }
};

struct LinearConflict : Heuristic {
    static int countConflicts(const vector<int>& tiles, int k, bool rowwise) {
        vector<bool> used(tiles.size(), false);
        int count = 0;

        for (int i = 0; i < (int)tiles.size(); i++) {
            if (used[i]) continue;

            for (int j = i + 1; j < (int)tiles.size(); j++) {
                if (used[j]) continue;

                int a = tiles[i];
                int b = tiles[j];
                bool isConflict = rowwise
                    ? ((a - 1) % k > (b - 1) % k)
                    : ((a - 1) / k > (b - 1) / k);

                if (isConflict) {
                    used[i] = true;
                    used[j] = true;
                    ++count;
                    break;
                }
            }
        }

        return count;
    }

    int operator()(const vector<int>& state, int k) const override {
        ManhattanDistance manhattan;
        int h = manhattan(state, k);
        int conflicts = 0;

        for (int row = 0; row < k; row++) {
            vector<int> tiles;
            for (int col = 0; col < k; col++) {
                int idx = row * k + col;
                int val = state[idx];
                if (val != 0 && (val - 1) / k == row) {
                    tiles.push_back(val);
                }
            }
            conflicts += countConflicts(tiles, k, true);
        }

        for (int col = 0; col < k; col++) {
            vector<int> tiles;
            for (int row = 0; row < k; row++) {
                int idx = row * k + col;
                int val = state[idx];
                if (val != 0 && (val - 1) % k == col) {
                    tiles.push_back(val);
                }
            }
            conflicts += countConflicts(tiles, k, false);
        }

        return h + 2 * conflicts;
    }
};

struct CornerLinearConflict : Heuristic {

    static bool isCornerGoal(int val, int k) {
        int r = (val - 1) / k;
        int c = (val - 1) % k;

        return (r == 0 && c == 0) ||
               (r == 0 && c == k - 1) ||
               (r == k - 1 && c == 0) ||
               (r == k - 1 && c == k - 1);
    }

    static int countCornerConflicts(const vector<int>& tiles, int k) {
        vector<bool> used(tiles.size(), false);
        int count = 0;

        for (int i = 0; i < (int)tiles.size(); i++) {
            if (used[i]) continue;

            int a = tiles[i];

            if (!isCornerGoal(a, k)) continue;

            for (int j = i + 1; j < (int)tiles.size(); j++) {
                if (used[j]) continue;

                int b = tiles[j];

                if (!isCornerGoal(b, k)) continue;

                int ra = (a - 1) / k;
                int rb = (b - 1) / k;
                int ca = (a - 1) % k;
                int cb = (b - 1) % k;

                bool conflict =
                    (ra == rb && ca > cb) ||
                    (ca == cb && ra > rb);

                if (conflict) {
                    used[i] = true;
                    used[j] = true;
                    count++;
                    break;
                }
            }
        }

        return count;
    }

    int operator()(const vector<int>& state, int k) const override {
        ManhattanDistance manhattan;
        LinearConflict lc;

        int h = manhattan(state, k);

        int lcHeuristic = lc(state, k);

        int M = h;
        int lcCount = (lcHeuristic - M) / 2;

        int cornerConflicts = 0;

        for (int row = 0; row < k; row++) {
            vector<int> tiles;
            for (int col = 0; col < k; col++) {
                int idx = row * k + col;
                int val = state[idx];
                if (val != 0 && (val - 1) / k == row) {
                    tiles.push_back(val);
                }
            }
            cornerConflicts += countCornerConflicts(tiles, k);
        }

        for (int col = 0; col < k; col++) {
            vector<int> tiles;
            for (int row = 0; row < k; row++) {
                int idx = row * k + col;
                int val = state[idx];
                if (val != 0 && (val - 1) % k == col) {
                    tiles.push_back(val);
                }
            }
            cornerConflicts += countCornerConflicts(tiles, k);
        }

        return M + 2 * (lcCount + cornerConflicts);
    }
};