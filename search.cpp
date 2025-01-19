#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <fstream>
#include <thread>
#include <chrono>

int main_loop() {
    for(int iteration=0; iteration<3; ++iteration) {
        initGrid();
        auto path = frontierSearchWithPath(0,0);
        savePath(path, "drone_positions.json");
        cout << "Iteration " << iteration+1 << " completed. Path saved.\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    cout << "Simulation loop finished.\n";
    return 0;
}


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

vector<pair<int,int>> frontierSearchWithPath(int startX, int startY) {
    queue<pair<int,int>> q;
    q.push({startX, startY});
    grid[startX][startY].visited = true;

    vector<pair<int,int>> path;

    while(!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        path.push_back({x, y});
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
    return path;
}

void initGrid() {
    for(int i=0; i<rows; ++i) {
        for(int j=0; j<cols; ++j) {
            grid[i][j] = {i, j, false};
        }
    }
}

int main() {
    initGrid();
    cout << "Starting drone frontier search...\n";

    auto path = frontierSearchWithPath(0,0);
    cout << "Drone path completed. Cells visited: " << path.size() << "\n";

    savePath(path);
    cout << "Drone path saved to drone_positions.json\n";
    return 0;
}
