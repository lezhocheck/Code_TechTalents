#include <iostream>
#include <vector>

const int16_t INFINITY = 32000;
const int16_t SHIFT = -1;

enum QueryType {
    UPDATE = '=',
    MAX = '?'
};

class SegmentTree2D{
public:
    explicit SegmentTree2D(const std::vector<std::vector<int16_t>>& initialMatrix);

    int16_t query(const int16_t fromColumn, const int16_t toColumn,
    const int16_t fromRow, const int16_t toRow) const;

    void update(const int16_t column,
    const int16_t row, const int16_t newValue);

    int16_t columnsSize() const;
    int16_t rowsSize() const;

private:
    std::vector<std::vector<int16_t>> segmentTree2D;
    std::vector<std::vector<int16_t>> matrix;

    class Point {
    public:
        int16_t x;
        int16_t y;

        Point(const int16_t xValue, const int16_t yValue);
    };

    struct Rectangle {
        Point bottomLeft;
        Point topRight;
    };

    class Range {
    public:
        int16_t lowerBound;
        int16_t upperBound;

        Range(const int16_t lower, const int16_t upper);
        bool checkForBoundsEquality() const;
        int16_t getMid() const;
    };

    // builds segment tree along the first (x) axis.
    void build(std::vector<int16_t>* segmentTree,
               const std::vector<int16_t>& array,
               const int16_t index, const Range& range);

    // builds final version of segment tree along the second (y) axis.
    // call this function to build segment tree.
    void build2D(const int16_t index, const Range& range);

    // finds maximum value in segment tree along the first (x) axis.
    int16_t query(const std::vector<int16_t>& segmentTree, const int16_t index,
                  const Range& queryRange, const Range& fixedRange) const;

    // finds maximum value in segment tree along the second (y) axis and returns final result.
    // call this function to get maximum value in the rectangle.
    int16_t query2D(const int16_t index,
                    const Range& columnsRange, const Rectangle& rect) const;

    // updates segment tree along the first (x) axis.
    void update(const Range& columnRange,
                const Range& rowRange, const int16_t indexX,
                const int16_t indexY,
                const Point& point, const int16_t value);

    // updates segment tree along the second (y) axis.
    // call this function to update value in the tree.
    void update2D(const int16_t index, const Range& columnRange,
                  const Point& point, const int16_t value);
};

class QueryData {
public:
    virtual QueryType getType() = 0;
};

class MaxQueryData : public QueryData {
public:
    int16_t fromColumn, toColumn;
    int16_t fromRow, toRow;

    MaxQueryData(const int16_t fromColumnValue, const int16_t toColumnValue,
    const int16_t fromRowValue, const int16_t toRowValue);

    QueryType getType() override;
};

class UpdateQueryData : public QueryData {
public:
    int16_t column, row;
    int16_t newValue;

    UpdateQueryData(const int16_t columnValue, const int16_t rowValue,
    const int16_t value);

    QueryType getType() override;
};

class MatrixQueryData {
public:
    std::vector<std::vector<int16_t>> matrix;
    std::vector<QueryData*> queryData;

    MatrixQueryData(const int16_t columnsSize,
    const int16_t rowsSize, const int queriesSize);
};

MatrixQueryData readMatrixQueryData(std::istream& istream);
std::vector<int16_t> findMaxValues(const MatrixQueryData& data);
void writeMaxValues(std::ostream& ostream, const std::vector<int16_t>& values);

int main(){
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    const MatrixQueryData data = readMatrixQueryData(std::cin);
    const std::vector<int16_t> maxValues = findMaxValues(data);
    writeMaxValues(std::cout, maxValues);
    return 0;
}
