#include <iostream>
#include <vector>

const int16_t INF = 32000;
const int16_t SIZE_MULTIPLIER = 4;
const int16_t SHIFT = -1;

enum QueryType {
    UPDATE = '=',
    MAX = '?'
};

struct Point {
    int16_t column;
    int16_t row;

    Point();
    Point(const int16_t columnValue, const int16_t rowValue);

    void addShift() {
        column += SHIFT;
        row += SHIFT;
    }
};

struct Rect {
    Point start;
    Point end;
    Rect();
};

std::istream& operator>>(std::istream& istream, Point* point);
std::istream& operator>>(std::istream& istream, Rect* rect);

class SegmentTree2D{
public:
    explicit SegmentTree2D(const std::vector<std::vector<int16_t>>& initialMatrix);
    int16_t query(const Rect& rect) const;
    void update(const Point& point, const int16_t newValue);
    int16_t columnsSize() const;
    int16_t rowsSize() const;
private:
    std::vector<std::vector<int16_t>> segmentTree2D;
    std::vector<std::vector<int16_t>> matrix;

    struct Range {
        int16_t lowBound;
        int16_t upBound;

        Range(const int16_t low, const int16_t up);
        bool isEqual() const;
        int16_t getMid() const;
    };

    void build(std::vector<int16_t>* segmentTree,
        const std::vector<int16_t>& array,
        const int16_t index, const Range& range);

    void build2D(const int16_t index, const Range& range);

    int16_t query(const std::vector<int16_t>& segmentTree, const int16_t index,
        const Range& queryRange, const Range& fixedRange) const;

    int16_t query2D(const int16_t index,
        const Range& columnsRange, const Rect& rect) const;

    void update(const Range& columnRange,
        const Range& rowRange, const Point& indexes,
        const Point& point, const int16_t value);

    void update2D(const int16_t index, const Range& columnRange,
        const Point& point, const int16_t value);
};

struct AskQueryData {
    Rect rect;

    AskQueryData();
    explicit AskQueryData(const Rect& rectValue);
};

struct UpdateQueryData {
    Point point;
    int16_t newValue;

    UpdateQueryData();
    UpdateQueryData(const Point& pointValue, const int16_t value);
};

struct QueryData {
    QueryType type;
    AskQueryData askQueryData;
    UpdateQueryData updateQueryData;
};

struct MatrixQueryData {
    std::vector<std::vector<int16_t>> matrix;
    std::vector<QueryData> queryData;

    MatrixQueryData(const int16_t columnsSize,
        const int16_t rowsSize, const int queriesSize);
};

MatrixQueryData readMatrixQueryData(std::istream& istream);
std::vector<int16_t> findMaxValues(const MatrixQueryData& data);
void writeMaxValues(std::ostream& ostream, const std::vector<int16_t>& values);

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    const MatrixQueryData data = readMatrixQueryData(std::cin);
    const std::vector<int16_t> maxValues = findMaxValues(data);
    writeMaxValues(std::cout, maxValues);
    return 0;
}
