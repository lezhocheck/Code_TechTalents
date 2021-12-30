#include <iostream>
#include <vector>
#include <algorithm>

const char POSITIVE_ANSWER[] = "YES";
const char NEGATIVE_ANSWER[] = "NO";

struct Query {
    int firstBox;
    int secondBox;
    int difference;
};

class DisjointSetUnion {
public:
    explicit DisjointSetUnion(const size_t size);
    bool unionSets(const Query& query);
    size_t size() const;
    int getDifference(const int index);

private:
    struct Set {
        Set* parent;
        int rank;
        int differenceWithMinimum;
        int differenceWithParent;
        bool isStart;
    };

    std::vector<Set> sets;

    void makeSet(const int index);
    Set* findSet(Set* set);
    bool merge(Set* firstSet, Set* secondSet, const int difference);
};

struct CoinsDistributionData {
    DisjointSetUnion boxes;
    std::vector<Query> notes;
};

struct DistributionPossibilityResult {
    bool isPossible;
    uint64_t notPossibleAfter;
    std::vector<int> result;
};

CoinsDistributionData readCoinsDistributionData(std::istream& istream);

DistributionPossibilityResult getDistributionPossibility(
        const CoinsDistributionData& coinsDistributionData);

void printPossibilityResult(std::ostream& ostream,
    const DistributionPossibilityResult& possibilityResult);

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    const CoinsDistributionData coinsDistributionData = 
        readCoinsDistributionData(std::cin);

    const DistributionPossibilityResult possibilityResult =
            getDistributionPossibility(coinsDistributionData);

    printPossibilityResult(std::cout, possibilityResult);

    return 0;
}
