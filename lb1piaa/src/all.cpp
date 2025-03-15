#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct Square {
  public:
    const int trailing, bottom;
    int x, y, size;

    Square(int x, int y, int size) : x(x), y(y), size(size), trailing(x + size), bottom(y + size) {}

    Square(const Square &other)
        : x(other.x), y(other.y), size(other.size), trailing(other.trailing), bottom(other.bottom) {
    }

    Square &operator=(const Square &other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            size = other.size;
        }
        return *this;
    }
};

class Table {
  private:
    int squareSize;
    int gridSize;
    int bestCount;
    std::vector<Square> bestSolution;

  public:
    void placeSquares() {
        setGridRatio();
        int startX = gridSize / 2;
        int startY = (gridSize + 1) / 2;
        int occupiedArea = pow(startY, 2) + 2 * pow(startX, 2);
        std::vector<Square> squares = {Square(0, 0, startY), Square(0, startY, startX),
                                       Square(startY, 0, startX)};

        backtrack(squares, occupiedArea, 3, startX, startY);
    }

    void printResult() {
        std::cout << bestCount << std::endl;
        for (const auto &square : bestSolution) {
            std::cout << square.x * squareSize + 1 << " " << square.y * squareSize + 1 << " "
                      << square.size * squareSize << std::endl;
        }
    }

    Table(int gridSize) : gridSize(gridSize), bestCount(gridSize * gridSize + 1) {}

  private:
    void backtrack(std::vector<Square> currentSquares, int occupiedArea, int currentCount,
                   int startX, int startY) {
        if (occupiedArea == gridSize * gridSize) {
            if (currentCount < bestCount) {
                bestCount = currentCount;
                bestSolution = currentSquares;
            }
            return;
        }

        for (int x = startX; x < gridSize; ++x) {
            for (int y = startY; y < gridSize; ++y) {
                if (isOverlap(currentSquares, x, y))
                    continue;

                int maxSize = findMaxSizeSquare(currentSquares, x, y);
                if (maxSize <= 0)
                    continue;

                for (int size = maxSize; size >= 1; --size) {
                    Square newSquare(x, y, size);
                    int newOccupiedArea = occupiedArea + size * size;

                    int remainingArea = gridSize * gridSize - newOccupiedArea;
                    if (remainingArea > 0) {
                        int maxPossibleSize = std::min(gridSize - x, gridSize - y);
                        int minSquaresNeeded =
                            (remainingArea + (maxPossibleSize * maxPossibleSize) - 1) /
                            (maxPossibleSize * maxPossibleSize);
                        if (currentCount + 1 + minSquaresNeeded >= bestCount) {
                            continue;
                        }
                    }

                    currentSquares.push_back(newSquare);
                    if (newOccupiedArea == gridSize * gridSize) {
                        if (currentCount + 1 < bestCount) {
                            bestCount = currentCount + 1;
                            bestSolution = currentSquares;
                        }
                        currentSquares.pop_back();
                        continue;
                    }

                    if (currentCount + 1 < bestCount) {
                        backtrack(currentSquares, newOccupiedArea, currentCount + 1, x, y);
                    }
                    currentSquares.pop_back();
                }
                return;
            }
            startY = 0;
        }
    }

    bool isOverlap(const std::vector<Square> &squares, int x, int y) {
        for (const auto &square : squares) {
            if (x >= square.x && x < square.trailing && y >= square.y && y < square.bottom) {
                return true;
            }
        }
        return false;
    }

    int findMaxSizeSquare(const std::vector<Square> &squares, int x, int y) {
        int maxSize = std::min(gridSize - x, gridSize - y);
        for (const auto &square : squares) {
            if (square.trailing > x && square.y > y) {
                maxSize = std::min(maxSize, square.y - y);
            } else if (square.bottom > y && square.x > x) {
                maxSize = std::min(maxSize, square.x - x);
            }
        }
        return maxSize;
    }

    void setGridRatio() {
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

int main() {
    int gridSize;
    std::cin >> gridSize;

    Table table(gridSize);
    table.placeSquares();
    table.printResult();

    return 0;
}