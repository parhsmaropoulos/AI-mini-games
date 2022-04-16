#include <iostream>
#include <string>
using namespace std;
#include <stack>

int moveNumber = 1;
int currentDisk;

// Algorithm runs on O(2^n) time where n is the number of disks

// Applies a change in the stacks representing the towers
void move(int n, stack<int> &from_tower, stack<int> &to_tower, string from, string to)
{
    // Get current top disk
    currentDisk = from_tower.top();
    // Remove from that tower
    from_tower.pop();
    // Pass is to the target tower
    to_tower.push(currentDisk);
    // Print move
    cout << "move number " << moveNumber << " : Move disk " << n << " from tower " << from << " to tower " << to << endl;
    // Number of move to solve the problem
    moveNumber++;
}

void HanoiTower(int n, stack<int> &from_tower, stack<int> &to_tower, stack<int> &help_tower, string from, string to, string help)
{
    // End condition
    if (n == 1)
    {
        // Apply the last change
        move(1, from_tower, to_tower, from, to);
        return;
    }
    // Recursive call of the main function
    HanoiTower(n - 1, from_tower, help_tower, to_tower, from, help, to);
    // Apply change on disks
    move(n, from_tower, to_tower, from, to);
    HanoiTower(n - 1, help_tower, to_tower, from_tower, help, to, from);
}

void PrintStack(stack<int> s)
{
    // If stack is empty then return
    if (s.empty())
        return;

    int x = s.top();

    // Pop the top element of the stack
    s.pop();

    // Recursively call the function PrintStack
    PrintStack(s);

    // Print the stack element starting
    // from the bottom
    cout << x << " ";

    // Push the same element onto the stack
    // to preserve the order
    s.push(x);
}

int main()
{
    // Initialize the three lists.
    stack<int> Start;
    stack<int> Mid;
    stack<int> End;
    // Get from user number of disks
    int disc_num; // Number of disks
    cout << "enter the number of discs you want";
    cin >> disc_num;

    // Append all the disc with descending order in the first list
    for (int i = 0; i < disc_num; i++)
    {
        Start.push(disc_num - i);
    };
    // Print starting state of the stacks
    cout << "Stack Printing.. \n";
    cout << "Start tower: \n";
    PrintStack(Start);
    cout << "Target tower: \n";
    PrintStack(End);
    cout << "Help tower: \n";
    PrintStack(Mid);

    // Start the recursive algorithm
    HanoiTower(disc_num, Start, End, Mid, "Start", "End", "Mid");
    // Print end state of the stacks
    cout << "Stack Printing.. \n";
    cout << "Start tower: \n";
    PrintStack(Start);
    cout << "Target tower: \n";
    PrintStack(End);
    cout << "Help tower: \n";
    PrintStack(Mid);
    return 0;
}
