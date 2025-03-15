#include <iostream>
#include "Table.cpp"

using namespace std;

int main() {

    int gridSize;
    cin >> gridSize;

    Table table(gridSize);
    table.placeSquares();
    table.printResult();

    return 0;
}