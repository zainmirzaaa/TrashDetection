#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <fstream>

// Save drone path to a JSON file
void savePath(const vector<pair<int,int>>& path, const string& filename="drone_positions.json") {
    ofstream out(filename);
    out << "[\n";
    for(size_t i = 0; i < path.size(); ++i) {
        auto [x, y] = path[i];
        out << "{ \"x\": " << x << ", \"y\": " << y << "}";
        if(i != path.size()-1) out << ",";
        out << "\n";
    }
    out << "]";
    out.close();
}

using namespace std;

struct Cell {
    int x, y;
    bool visited;
};

int rows = 10;
int cols = 10;
vector<vector<Cell>> grid(rows, vector<Cell>(cols));

vector<pair<int,int>> directions = {{0,1},{1,0},{0,-1},{-1,0}};

bool isValid(int x, int y) {
    return x >= 0 && x < rows && y >= 0 && y < cols && !grid[x][y].visited;
}

void frontierSearch(int startX, int startY) {
    queue<pair<int,int>> q;
    q.push({startX, startY});
    grid[startX][startY].visited = true;

    while(!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        cout << "Drone visiting: (" << x << "," << y << ")\n";

        for(auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            if(isValid(nx, ny)) {
                grid[nx][ny].visited = true;
                q.push({nx, ny});
            }
        }
    }
}

int main() {
    frontierSearch(0,0);
    return 0;
}
