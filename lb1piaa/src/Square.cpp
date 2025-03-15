struct Square {

public:

    const int trailing; // right boundary (edge)
    const int bottom; // bottom boundary (edge)
    int x, y, size; // x-axis / y-axis positions / length of a square

    // Constructor
    Square(int x, int y, int size) : x(x), y(y), size(size), trailing(x + size), bottom(y + size) {}

    // A copy constructor
    Square(const Square& other)
        : x(other.x), y(other.y), size(other.size), trailing(other.trailing), bottom(other.bottom) {
    }

    Square& operator=(const Square& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            size = other.size;
        }
        return *this;
    }
};