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
    explicit SegmentTree2D(const std::vector<std::vector<int16_t>>& initialMatrix) {
        matrix = initialMatrix;
        segmentTree2D.resize(4 * columnsSize(),
            std::vector<int16_t>(4 * rowsSize(), 0));
        build2D(1, Range(0, columnsSize() - 1));
    }

    int16_t query(const int16_t fromColumn, const int16_t toColumn,
        const int16_t fromRow, const int16_t toRow) const {

        const Range range(0, columnsSize() - 1);
        const Rectangle rectangle = {Point(fromColumn, fromRow),
            Point(toColumn, toRow)};
        const int16_t result = query2D(1, range, rectangle);

        return result;
    }

    void update(const int16_t column,
        const int16_t row, const int16_t newValue) {

        const Range range(0, columnsSize() - 1);
        const Point point(column, row);
        update2D(1, range, point, newValue);
    }

    int16_t columnsSize() const {
        const int16_t columns = matrix.size();
        return columns;
    }

    int16_t rowsSize() const {
        if (columnsSize() == 0) {
            return 0;
        }
        const int16_t rows = matrix.at(0).size();
        return rows;
    }

 private:
    std::vector<std::vector<int16_t>> segmentTree2D;
    std::vector<std::vector<int16_t>> matrix;

    class Point {
     public:
        int16_t x;
        int16_t y;

        Point(const int16_t xValue,
              const int16_t yValue) {
            x = xValue + SHIFT;
            y = yValue + SHIFT;
        }
    };

    struct Rectangle {
        Point bottomLeft;
        Point topRight;
    };

    class Range {
     public:
        int16_t lowerBound;
        int16_t upperBound;

        Range(const int16_t lower, const int16_t upper) {
            lowerBound = lower;
            upperBound = upper;
        }

        bool checkForBoundsEquality() const {
            const bool result = lowerBound == upperBound;
            return result;
        }

        int16_t getMedium() const {
            const int16_t result = (lowerBound +
                static_cast<int>(upperBound)) / 2;
            return result;
        }
    };

    // builds segment tree along the first (x) axis.
    void build(std::vector<int16_t>* segmentTree,
        const std::vector<int16_t>& array,
        const int16_t index, const Range& range) {

        if (range.checkForBoundsEquality()) {
            segmentTree->at(index) = array.at(range.lowerBound);
        } else {
            const int16_t medium = range.getMedium();

            build(segmentTree, array, 2 * index,
                Range(range.lowerBound, medium));
            build(segmentTree, array, 2 * index + 1,
                Range(medium + 1, range.upperBound));

            segmentTree->at(index) = std::max(segmentTree->at(2 * index),
                segmentTree->at(2 * index + 1));
        }
    }

    // builds final version of segment tree along the second (y) axis.
    // call this function to build segment tree.
    void build2D(const int16_t index, const Range& range) {
        if (range.checkForBoundsEquality()) {
            build(&segmentTree2D.at(index),
                  matrix.at(range.lowerBound), 1,
                  Range(0, rowsSize() - 1));
        } else {
            const int16_t medium = range.getMedium();
            build2D(2 * index, Range(range.lowerBound, medium));
            build2D(2 * index + 1, Range(medium + 1, range.upperBound));

            const int16_t rowSize =  segmentTree2D.at(index).size();

            for (int16_t column = 0; column < rowSize; ++column) {
                segmentTree2D.at(index).at(column) =
                    std::max(segmentTree2D.at(2 * index).at(column),
                    segmentTree2D.at(2 * index + 1).at(column));
            }
        }
    }

    // finds maximum value in segment tree along the first (x) axis.
    int16_t query(const std::vector<int16_t>& segmentTree, const int16_t index,
                  const Range& queryRange, const Range& fixedRange) const {
        if (fixedRange.lowerBound > queryRange.upperBound ||
            fixedRange.upperBound < queryRange.lowerBound) {
            return -INFINITY;
        }

        if (queryRange.lowerBound >= fixedRange.lowerBound &&
            queryRange.upperBound <= fixedRange.upperBound) {
            return segmentTree.at(index);
        }

        const int16_t medium = queryRange.getMedium();
        const int16_t leftQuery = query(segmentTree, 2 * index,
            Range(queryRange.lowerBound, medium), fixedRange);
        const int16_t rightQuery = query(segmentTree, 2 * index + 1,
            Range(medium + 1, queryRange.upperBound), fixedRange);

        const int16_t maximum = std::max(leftQuery, rightQuery);
        return maximum;
    }

    // finds maximum value in segment tree
    // along the second (y) axis and returns final result.
    // call this function to get maximum value in the rectangle.
    int16_t query2D(const int16_t index,
                    const Range& columnsRange,
                    const Rectangle& rect) const {
        if (columnsRange.lowerBound > rect.topRight.x ||
            columnsRange.upperBound < rect.bottomLeft.x) {
            return -INFINITY;
        }

        if (columnsRange.lowerBound >= rect.bottomLeft.x &&
            columnsRange.upperBound <= rect.topRight.x) {
            const int16_t queryResult = query(segmentTree2D[index], 1,
                Range(0, rowsSize() - 1),
                Range(rect.bottomLeft.y, rect.topRight.y));
            return queryResult;
        }

        const  int16_t medium = columnsRange.getMedium();
        const int16_t leftQuery = query2D(2 * index,
            Range(columnsRange.lowerBound, medium), rect);
        const int16_t rightQuery = query2D(2 * index + 1,
            Range(medium + 1, columnsRange.upperBound), rect);
        const int16_t maximum = std::max(leftQuery, rightQuery);

        return maximum;
    }

    // updates segment tree along the first (x) axis.
    void update(const Range& columnRange,
                const Range& rowRange, const int16_t indexX,
                const int16_t indexY,
                const Point& point, const int16_t value) {

        if (rowRange.checkForBoundsEquality()) {
            if (columnRange.checkForBoundsEquality()) {
                segmentTree2D.at(indexX).at(indexY) = value;
            } else {
                segmentTree2D.at(indexX).at(indexY) =
                    std::max(segmentTree2D.at(indexX * 2).at(indexY),
                    segmentTree2D.at(indexX * 2 + 1).at(indexY));
            }
        } else {
            const int16_t mediumRow = rowRange.getMedium();
            if (point.y <= mediumRow) {
                update(columnRange,
                    Range(rowRange.lowerBound, mediumRow),
                    indexX, indexY * 2, point, value);
            } else {
                update(columnRange,
                    Range(mediumRow + 1, rowRange.upperBound),
                       indexX, indexY * 2 + 1, point, value);
            }
            segmentTree2D.at(indexX).at(indexY) =
                std::max(segmentTree2D.at(indexX).at(indexY * 2),
                segmentTree2D.at(indexX).at(indexY * 2 + 1));
        }
    }

    // updates segment tree along the second (y) axis.
    // call this function to update value in the tree.
    void update2D(const int16_t index, const Range& columnRange,
                  const Point& point, const int16_t value) {
        if (!columnRange.checkForBoundsEquality()) {
            const int16_t medium = columnRange.getMedium();
            if (point.x <= medium){
                update2D(index * 2,
                    Range(columnRange.lowerBound, medium), point, value);
            } else {
                update2D(index * 2 + 1,
                    Range(medium + 1, columnRange.upperBound), point, value);
            }
        }

        update(columnRange, Range(0, rowsSize() - 1),
            index, 1, point, value);
    }
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
                 const int16_t fromRowValue, const int16_t toRowValue) {
        fromColumn = fromColumnValue;
        fromRow = fromRowValue;
        toColumn = toColumnValue;
        toRow = toRowValue;
    }

    QueryType getType() override {
        return QueryType::MAX;
    }
};

class UpdateQueryData : public QueryData {
 public:
    int16_t column, row;
    int16_t newValue;

    UpdateQueryData(const int16_t columnValue, const int16_t rowValue,
                    const int16_t value) {
        column = columnValue;
        row = rowValue;
        newValue = value;
    }

    QueryType getType() override {
        return QueryType::UPDATE;
    }
};

class MatrixQueryData {
 public:
    std::vector<std::vector<int16_t>> matrix;
    std::vector<QueryData*> queryData;

    MatrixQueryData(const int16_t columnsSize,
                    const int16_t rowsSize, const int queriesSize) {
        matrix.resize(columnsSize, std::vector<int16_t>(rowsSize));
        queryData.resize(queriesSize);
    }
};

MatrixQueryData readMatrixQueryData(std::istream& istream){
    int16_t columnsSize, rowsSize;
    int queriesSize;
    istream >> columnsSize >> rowsSize >> queriesSize;

    MatrixQueryData data(columnsSize, rowsSize, queriesSize);

    for (int16_t column = 0; column < columnsSize; ++column) {
        for (int16_t row = 0; row < rowsSize; ++row) {
            istream >> data.matrix[column][row];
        }
    }

    for (int query = 0; query < queriesSize; ++query) {
        char querySign;
        istream >> querySign;

        if (querySign == QueryType::UPDATE) {
            int16_t column, row;
            int16_t newValue;
            istream >> column >> row >> newValue;
            data.queryData[query] =
                new UpdateQueryData(column, row, newValue);
        } else if (querySign == QueryType::MAX) {
            int16_t fromColumn, toColumn;
            int16_t fromRow, toRow;
            istream >> fromColumn >> toColumn >> fromRow >> toRow;
            data.queryData[query] =
                new MaxQueryData(fromColumn, toColumn, fromRow, toRow);
        }
    }

    return data;
}

std::vector<int16_t> findMaxValues(const MatrixQueryData& data) {
    std::vector<int16_t> result;
    SegmentTree2D tree(data.matrix);

    for (const auto& query : data.queryData) {
        if (query->getType() == QueryType::UPDATE) {
            UpdateQueryData* queryData =
                dynamic_cast<UpdateQueryData*>(query);
            tree.update(queryData->column, queryData->row, queryData->newValue);
        } else if (query->getType() == QueryType::MAX) {
            MaxQueryData* queryData =
                dynamic_cast<MaxQueryData*>(query);
            result.push_back(
                tree.query(queryData->fromColumn, queryData->toColumn,
                queryData->fromRow, queryData->toRow));
        }
    }

    return result;
}

void writeMaxValues(std::ostream& ostream, const std::vector<int16_t>& values) {
    for (int16_t value : values) {
        ostream << value << "\n";
    }
}

int main(){
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    const MatrixQueryData data = readMatrixQueryData(std::cin);
    const std::vector<int16_t> maxValues = findMaxValues(data);
    writeMaxValues(std::cout, maxValues);
    return 0;
}
