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
    explicit DisjointSetUnion(const size_t size) {
        sets = std::vector<Set>(size);
        for (int i = 0; i < size; i++) {
            makeSet(i);
        }
        if (size > 0) {
            sets[0].isStart = true;
        }
    }

    bool unionSets(const Query& query) {
        Set* firstSetParent = findSet(&sets[query.firstBox]);
        Set* secondSetParent = findSet(&sets[query.secondBox]);
        int secondBoxDifferenceWithParent =
            sets[query.secondBox].differenceWithParent;
        int firstBoxDifferenceWithParent =
            sets[query.firstBox].differenceWithParent;
        if (firstSetParent == secondSetParent &&
            secondBoxDifferenceWithParent + query.difference !=
            firstBoxDifferenceWithParent) {
            return false;
        }

        int difference = query.difference +
            secondBoxDifferenceWithParent -
            firstBoxDifferenceWithParent;

        bool mergeResult = merge(firstSetParent, secondSetParent, difference);
        return mergeResult;
    }

    size_t size() const {
        size_t size = sets.size();
        return size;
    }

    int getDifference(const int index) {
        int findSetResult =
            findSet(&sets.at(index))->differenceWithMinimum -
            sets.at(index).differenceWithParent;
        return findSetResult;
    }

 private:
    struct Set {
        Set* parent;
        int rank;
        int differenceWithMinimum;
        int differenceWithParent;
        bool isStart;
    };

    std::vector<Set> sets;

    void makeSet(const int index) {
        Set* temp = &sets[index];
        temp->parent = &sets[index];
        temp->rank = 0;
        temp->differenceWithMinimum = 0;
        temp->differenceWithParent = 0;
        temp->isStart = false;
    }

    Set* findSet(Set* set) {
        if (set != set->parent) {
            Set* parent = findSet(set->parent);
            set->differenceWithParent += set->parent->differenceWithParent;
            set->parent = parent;
        }
        return set->parent;
    }

    bool merge(Set* firstSet, Set* secondSet, int difference) {
        if (firstSet->rank > secondSet->rank) {
            std::swap(firstSet, secondSet);
            difference = -difference;
        }

        firstSet->parent = secondSet;
        if (firstSet->rank == secondSet->rank) {
            secondSet->rank++;
        }
        firstSet->differenceWithParent = difference;
        secondSet->differenceWithMinimum =
            std::max(secondSet->differenceWithMinimum,
            difference + firstSet->differenceWithMinimum);

        if (firstSet->isStart) {
            secondSet->isStart = true;
        }

        if (secondSet->isStart) {
            Set* first = &sets[0];
            findSet(first);
            if (first->differenceWithParent -
                first->parent->differenceWithMinimum < 0) {
                return false;
            }
        }
        return true;
    }
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

CoinsDistributionData readCoinsDistributionData(std::istream& istream) {
    int boxCount;
    int queriesCount;
    istream >> boxCount >> queriesCount;
    CoinsDistributionData coinsDistributionData =
        {DisjointSetUnion(boxCount),
        std::vector<Query>(queriesCount)};
    for (int queryIndex = 0; queryIndex < queriesCount; queryIndex++) {
        int firstBox, secondBox, difference;
        istream >> firstBox >> secondBox >> difference;
        coinsDistributionData.notes[queryIndex] =
            {firstBox, secondBox, difference};
    }
    return coinsDistributionData;
}

DistributionPossibilityResult getDistributionPossibility(
    const CoinsDistributionData& coinsDistributionData) {
    std::vector<Query> queries = coinsDistributionData.notes;
    DisjointSetUnion boxes = coinsDistributionData.boxes;
    for (size_t queryIndex = 0; queryIndex < queries.size(); ++queryIndex) {
        if (!boxes.unionSets({queries[queryIndex].secondBox,
            queries[queryIndex].firstBox, queries[queryIndex].difference})) {
            return {false, queryIndex + 1};
        }
    }

    DistributionPossibilityResult possibilityResult{true, 0,
        std::vector<int>(boxes.size())};
    for (size_t boxIndex = 0; boxIndex < boxes.size(); ++boxIndex) {
        possibilityResult.result[boxIndex] = boxes.getDifference(boxIndex);
    }
    return possibilityResult;
}

void printPossibilityResult(std::ostream& ostream,
    const DistributionPossibilityResult& possibilityResult) {
    if (possibilityResult.isPossible) {
        ostream << POSITIVE_ANSWER << "\n";
        for (int result : possibilityResult.result) {
            ostream << result << " ";
        }
        ostream << "\n";
    } else {
        ostream << NEGATIVE_ANSWER << "\n";
        ostream << possibilityResult.notPossibleAfter << "\n";
    }
}

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
