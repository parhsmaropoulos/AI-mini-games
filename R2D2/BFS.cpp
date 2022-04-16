
#include <iostream>
#include <queue>
#include <vector>
#include <list>
using namespace std;

//Map size
const int X = 5;
const int Y = 5;

// Map
int map[5][5] = {{1, 1, 1, 1, 2}, {1, 2, 2, 2, 2}, {3, 3, 4, 4, 3}, {1, 1, 4, 3, 3}, {3, 1, 1, 1, 1}};
// Starting and Ending points
int start_x, start_y;
int end_x, end_y;
int x, y;

// Total steps
int move_count = 0;
// Total cost
int total_cost = 0;

// Win condition
bool finished = false;

// Matrix for visited cells
bool visited[X][Y];

// Available move vectors
vector<int> move_x = {-1, +1, 0, 0};
vector<int> move_y = {0, 0, -1, +1};

// Next node row/col indecies  to be visited
queue<int> row_q;
queue<int> col_q;

void get_best_adjacency(int x, int y)
{
    int xx, yy, min_cost, min_x, min_y;
    for (int i = 0; i < 4; i++)
    {
        xx = x + move_x[i];
        yy = y + move_y[i];
        // Out of bounds
        if (xx < 0 || yy < 0)
            continue;
        // Out of bounds
        if (xx >= X || yy >= Y)
            continue;
        // Already visited
        if (visited[xx][yy])
            continue;
        // Get the min cost node
        if (map[xx][yy] < min_cost)
        {
            min_cost = map[xx][yy];
            min_x = xx;
            min_y = yy;
        }
        else if (map[xx][yy] == min_cost)
        {
            // If we have a tie in cost get the lowest x
            if (xx != min_x)
            {
                if (xx < min_x)
                {
                    min_x = xx;
                    min_y = yy;
                    continue;
                }
                continue;
            }
            if (yy != min_y)
            {
                if (yy < min_y)
                {
                    min_x = xx;
                    min_y = yy;
                    continue;
                }
                continue;
            }
        }
    }
    //Min x and Min y are the coords with the lowest cost
    row_q.push(min_x);
    col_q.push(min_y);
    visited[x][y] = true;
    total_cost += map[x][y];
    printf("We are at point with X: %i and Y: %i \n Best point is point with x: %i and y: %i \n", x, y, min_x, min_y);
}

void BFS(int start_x, int start_y)
{
    //Push starting point
    row_q.push(start_x);
    col_q.push(start_y);
    visited[start_x][start_y] = true;

    while (row_q.size() > 0)
    {
        x = row_q.front();
        y = col_q.front();
        row_q.pop();
        col_q.pop();
        if (x == end_x && y == end_y)
        {
            finished = true;
            break;
        }
        get_best_adjacency(x, y);
        move_count++;
    }
    if (finished)
        printf("\nWe reached our goal!\n Total moves: %i\n Total cost: %i", move_count, total_cost);
    else
        printf("\nUnfortunatly we did not reached our goal :( \n Total moves: %i\n Total cost: %i", move_count, total_cost);
}

int main()
{
    // Initialize map
    // int map[5][5] = {{1, 1, 1, 1, 2}, {1, 2, 2, 2, 2}, {3, 3, 4, 4, 3}, {1, 1, 4, 3, 3}, {3, 1, 1, 1, 1}};
    for (int i = 0; i < X; ++i)
    {
        for (int j = 0; j < Y; ++j)
        {
            visited[i][j] = false;
        }
    }

    printf("Input start X:");
    cin >> start_x;
    printf("Input start Y:");
    cin >> start_y;
    cout << endl;
    printf("Input end X:");
    cin >> end_x;
    printf("Input end Y:");
    cin >> end_y;

    BFS(start_x, start_y);
}