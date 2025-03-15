#include <vector>
#include "Square.cpp"
#include <iostream>

using namespace std;

class Table {

private:

    int squareSize;
    int gridSize;
    int bestCount;
    vector<Square> bestSolution;

public:

    // Place squares in the grid
    void placeSquares() {

        simplifyGrid();
        int startX = gridSize / 2;
        int startY = (gridSize + 1) / 2;

        vector<Square> squares = { Square(0, 0, startY), Square(0, startY, startX), Square(startY, 0, startX) };

        int occupiedArea = pow(startY, 2) + 2 * pow(startX, 2);

        backtrack(squares, occupiedArea, 3, startX, startY);
    }

    // Print the result (best solution)
    void printResult() {

        cout << bestCount << endl;
        for (const auto& square : bestSolution) {
            cout << square.x * squareSize + 1 << " " << square.y * squareSize + 1 << " "
                << square.size * squareSize << endl;
        }
    }

    Table(int gridSize) : gridSize(gridSize), bestCount(gridSize* gridSize + 1) {}

private:

    // Explore All other possible position and deteremin the best solution
    void backtrack(vector<Square> currentSquares, int occupiedArea, int currentCount, int startX, int startY) {

        // Base case: If the entire grid is occupied, we check if this solution is better than the current best.
        if (occupiedArea == gridSize * gridSize) {
            if (currentCount < bestCount) {
                // Update the best solution if the current solution uses fewer squares.
                bestCount = currentCount;
                bestSolution = currentSquares;
            }
            return; // Stop further exploration for this branch.
        }

        // Iterate over all possible positions (x, y) in the grid.
        for (int x = startX; x < gridSize; x++) {
            for (int y = startY; y < gridSize; y++) {
                // Skip this position if placing a square here would overlap with existing squares.
                if (isOverlap(currentSquares, x, y))
                    continue;

                // Find the maximum possible size of a square that can be placed at (x, y).
                int maxSizeOfSquare = findMaxSizeSquare(currentSquares, x, y);

                // If no square can be placed at (x, y), skip this position.
                if (maxSizeOfSquare <= 0)
                    continue;

                // Try placing squares of all possible sizes, starting from the largest.
                for (int size = maxSizeOfSquare; size >= 1; size--) {
                    // Create a new square at (x, y) with the current size.
                    Square newSquare(x, y, size);

                    // Calculate the new occupied area after placing this square.
                    int newOccupiedArea = occupiedArea + size * size;

                    // Calculate the remaining area that still needs to be covered.
                    int remainingArea = gridSize * gridSize - newOccupiedArea;

                    // If there is remaining area, estimate the minimum number of squares needed to cover it.
                    if (remainingArea > 0) {
                        // The maximum possible size of a square that can fit in the remaining area.
                        int maxPossibleSize = min(gridSize - x, gridSize - y);

                        // Estimate the minimum number of squares needed to cover the remaining area.
                        int minSquaresNeeded =
                            (remainingArea + (maxPossibleSize * maxPossibleSize) - 1) /
                            (maxPossibleSize * maxPossibleSize);

                        // If the current solution cannot be better than the best solution, skip this branch.
                        if (currentCount + 1 + minSquaresNeeded >= bestCount) {
                            continue;
                        }
                    }

                    // Add the new square to the current solution.
                    currentSquares.push_back(newSquare);

                    // If the grid is fully occupied, check if this solution is better than the best solution.
                    if (newOccupiedArea == gridSize * gridSize) {
                        if (currentCount + 1 < bestCount) {
                            // Update the best solution.
                            bestCount = currentCount + 1;
                            bestSolution = currentSquares;
                        }
                        // Remove the last square and continue exploring other possibilities.
                        currentSquares.pop_back();
                        continue;
                    }

                    // If the current solution can still be improved, recursively explore further.
                    if (currentCount + 1 < bestCount) {
                        backtrack(currentSquares, newOccupiedArea, currentCount + 1, x, y);
                    }

                    // Remove the last square to backtrack and try other possibilities.
                    currentSquares.pop_back();
                }

                // After trying all sizes at (x, y), move to the next position.
                return;
            }

            // Reset startY to 0 after the first row to ensure all positions are explored.
            startY = 0;
        }
    }

    // Check if the position (x,y) overlap with existing squares
    bool isOverlap(const vector<Square>& squares, int x, int y) {

        for (const auto& square : squares) {
            if (x >= square.x && x < square.trailing && y >= square.y && y < square.bottom) {
                return true;
            }
        }
        return false;
    }

    // Find the possible max size for a square
    int findMaxSizeSquare(const vector<Square>& squares, int x, int y) {

        // Initialize the maximum possible size of a square that can be placed at (x, y).
        // This is constrained by the grid boundaries: the square cannot extend beyond the grid.
        int maxSizeOfSquare = min(gridSize - x, gridSize - y);

        // Iterate through all existing squares to check if they restrict the size of the new square.
        for (const auto& square : squares) {
            // Check if the existing square is to the right and below the position (x, y).
            // If so, the new square's size is restricted by the distance between y and the existing square's top edge.
            if (square.trailing > x && square.y > y) {
                maxSizeOfSquare = min(maxSizeOfSquare, square.y - y);
            }
            // Check if the existing square is below and to the right of the position (x, y).
            // If so, the new square's size is restricted by the distance between x and the existing square's left edge.
            else if (square.bottom > y && square.x > x) {
                maxSizeOfSquare = min(maxSizeOfSquare, square.x - x);
            }
        }

        // Return the maximum possible size of the new square that can be placed at (x, y)
        // without overlapping any existing squares or exceeding the grid boundaries.
        return maxSizeOfSquare;
    }

    void simplifyGrid() {
        int maxDivisor = 1;

        for (int i = gridSize / 2; i >= 1; --i) {
            if (gridSize % i == 0) {
                maxDivisor = i;
                break;
            }
        }

        squareSize = maxDivisor;
        gridSize = gridSize / maxDivisor;
    }
};